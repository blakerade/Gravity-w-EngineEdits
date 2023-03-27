// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGravityCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chaos/Utilities.h"
#include "GameFramework/PawnMovementComponent.h"

ABaseGravityCharacter::ABaseGravityCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh(), FName("HoverAttachPoint"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
}

void ABaseGravityCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(CharacterMovementMapping, 1.f);
		}
	}
}

void ABaseGravityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABaseGravityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
		if (MoveAction && LookAction && JumpAction && CrouchAction && RotateAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::Move);
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::Look);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::JumpButtonPressed);
			PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::CrouchButtonPressed);
			PlayerEnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::RotateToGravity);
		}
	}
}

void ABaseGravityCharacter::Move(const FInputActionValue& ActionValue)
{
	const FVector2D Value = ActionValue.Get<FVector2D>();
	AddMovementInput(GetMesh()->GetForwardVector(), Value.X);
	AddMovementInput(GetMesh()->GetRightVector(), Value.Y);
	
}

void ABaseGravityCharacter::Look(const FInputActionValue& ActionValue)
{
	const FVector2D Value = ActionValue.Get<FVector2D>();
	GetMesh()->AddLocalRotation(FRotator(0.f,Value.X,0.f));
	SpringArm->AddRelativeRotation(FRotator(Value.Y, 0.f, 0.f));
}

void ABaseGravityCharacter::JumpButtonPressed(const FInputActionValue& ActionValue)
{
	Jump();
}

void ABaseGravityCharacter::CrouchButtonPressed(const FInputActionValue& ActionValue)
{
	if(!bIsCrouched)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ABaseGravityCharacter::RotateToGravity(const FInputActionValue& ActionValue)
{
	if(FMath::Abs(FVector::DotProduct(FVector::UpVector, GravityV.GetSafeNormal())) == 1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("1st IF"));
		FVector NormGravity = GravityV;
		NormGravity.Normalize();
		FMatrix TargetRotation = FMatrix(FVector::CrossProduct(NormGravity, FVector::RightVector), FVector::RightVector, -NormGravity, FVector::ZeroVector);
		FRotator OrientToGravity = FMath::RInterpTo(GetActorRotation(), TargetRotation.Rotator(), GetWorld()->DeltaTimeSeconds, 1.f);
		SetActorRotation(OrientToGravity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("2nd IF"));
		FVector NormGravity = GravityV;
		NormGravity.Normalize();
		FMatrix TargetRotation = FMatrix(FVector::CrossProduct(NormGravity, FVector::CrossProduct(NormGravity, FVector::UpVector)),
		FVector::CrossProduct(NormGravity, FVector::UpVector),
		-NormGravity,
		FVector::ZeroVector);
		FRotator OrientToGravity = FMath::RInterpTo(GetActorRotation(), TargetRotation.Rotator(), GetWorld()->DeltaTimeSeconds, 1.f);
		SetActorRotation(OrientToGravity);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Dot Product: %f"), FMath::Abs(FVector::DotProduct(FVector::UpVector, GravityV.GetSafeNormal())));
	
}

