// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "BaseGravityCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
UCLASS()
class GRAVITY_API ABaseGravityCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseGravityCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;


	//Components
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	//InputActions/Mappings
	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere)
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere)
	UInputAction* RotateAction;
	UPROPERTY(EditAnywhere)
	UInputMappingContext* CharacterMovementMapping;

	//Input Functions
	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);
	void JumpButtonPressed(const FInputActionValue& ActionValue);
	void CrouchButtonPressed(const FInputActionValue& ActionValue);
	void RotateToGravity(const FInputActionValue& ActionValue);
	
private:
	
public:	
	

};
