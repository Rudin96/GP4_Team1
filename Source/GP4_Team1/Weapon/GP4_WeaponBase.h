// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <GP4_Team1/Player/GP4_PlayerCharacter.h>
#include "GP4_WeaponBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GP4_TEAM1_API UGP4_WeaponBase : public UObject
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bCanFire = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bIsSecondaryFireReady = true;

public:

	UGP4_WeaponBase();

	UFUNCTION(BlueprintNativeEvent)
		void Fire();

	UFUNCTION(BlueprintNativeEvent)
		void TickWeapon(float DeltaSeconds);

	UFUNCTION(BlueprintNativeEvent)
		void SecondaryFire();

	UFUNCTION(BlueprintNativeEvent)
		void Setup();

	UFUNCTION(BlueprintCallable)
		virtual void ResetWeapon();

	UFUNCTION(Blueprintcallable)
		AGP4_PlayerCharacter* GetOwner() { return Cast<AGP4_PlayerCharacter>(GetOuter()); }
};
