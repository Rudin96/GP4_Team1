// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GP4_PlayerCharacter.h"
#include "GP4_PlayerController.generated.h"

UCLASS()
class GP4_TEAM1_API AGP4_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FVector InputRotationVector;

protected:
	void SetupInputComponent() override;

private:
	void HandleMoveForward(float Val);
	void HandleMoveRight(float Val);
	void HandleLookUp(float Val);
	void HandleLookRight(float Val);

	void HandlePrimaryAction();
	void HandleSecondaryAction();
	void HandleThirdAction();
	void HandleFourthAction();

	void HandleResetWeapon1();

	void HandleJump();

	FVector InputDirection;

	AGP4_PlayerCharacter* GetOwnedCharacter() { return Cast<AGP4_PlayerCharacter>(GetPawn()); }
};
