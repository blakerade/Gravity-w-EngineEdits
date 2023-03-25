// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpenColorIOShader.h"

#include "Containers/SortedMap.h"
#include "OpenColorIOModule.h"
#include "OpenColorIOShared.h"
#include "TextureResource.h"

void OpenColorIOBindTextureResources(FOpenColorIOPixelShaderParameters* Parameters, const TSortedMap<int32, FTextureResource*>& InTextureResources)
{
	/**
	* The engine's shader parameter API doesn't provide name/slot-based updates (in the style of legacy OpenGL),
	* which is what OCIOv2 apparently relies on. Knowing that our input texture array follows the shader
	* binding slot order, we set the FOpenColorIOPixelShaderParameters members by copying the relevant
	* pointers using offsets from the initial "0" textures.
	* 
	* @todo: Improve binding logic, maybe using an approach similar to shaders generated by ComputeGraph.
	*/
	const uint8* BaseAddress = (uint8*)Parameters;

	for ( const TPair<int32, FTextureResource*>& Pair : InTextureResources )
	{
		const int32 Index = Pair.Key;
		ensureMsgf(Index >= 0 && Index < (int32)OpenColorIOShader::MaximumTextureSlots, TEXT("Out of range LUT texture slot index."));

		const FTextureResource* Texture = Pair.Value;
		const bool bIsLUT3D = Texture->GetTexture3DResource() != nullptr;

		const uint32 BaseOffset = bIsLUT3D ? offsetof(FOpenColorIOPixelShaderParameters, Ocio_lut3d_0) : offsetof(FOpenColorIOPixelShaderParameters, Ocio_lut1d_0);
		const uint32 ParamOffset = BaseOffset + (sizeof(FRHITexture*) + sizeof(FRHISamplerState*)) * Index;

		*((FRHITexture**)(BaseAddress + ParamOffset)) = Texture->TextureRHI.GetReference();
		*((FRHISamplerState**)(BaseAddress + ParamOffset + sizeof(FRHITexture*))) = Texture->SamplerStateRHI.GetReference();
	}
}

IMPLEMENT_SHADER_TYPE(, FOpenColorIOPixelShader, TEXT("/Plugin/OpenColorIO/Private/OpenColorIOShader.usf"), TEXT("MainPS"), SF_Pixel)