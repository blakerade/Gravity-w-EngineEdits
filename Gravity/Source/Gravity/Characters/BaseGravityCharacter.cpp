// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGravityCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"

ABaseGravityCharacter::ABaseGravityCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
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
	GetGravityDirection();
}

void ABaseGravityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
		if (MoveAction && LookAction && JumpAction && CrouchAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::Move);
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::Look);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::JumpButtonPressed);
			PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABaseGravityCharacter::CrouchButtonPressed);
		}
	}
}

void ABaseGravityCharacter::Move(const FInputActionValue& ActionValue)
{
	const FVector2D Value = ActionValue.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(),  Value.Y);
	AddMovementInput(GetActorRightVector(), Value.X);
	
}

void ABaseGravityCharacter::Look(const FInputActionValue& ActionValue)
{
	const FVector2D Value = ActionValue.Get<FVector2D>();
	AddControllerPitchInput(-Value.Y);
	AddControllerYawInput(Value.X);
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

