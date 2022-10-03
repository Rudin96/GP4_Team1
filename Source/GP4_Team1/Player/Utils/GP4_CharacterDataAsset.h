// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GP4_CharacterDataAsset.generated.h"

/**
 * 
 */

class UGP4_WeaponBase;

UCLASS()
class GP4_TEAM1_API UGP4_CharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float MovementSpeed = 700.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		int StartingHealth = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		TArray<TSubclassOf<UGP4_WeaponBase>> WeaponClasses;

	UPROPERTY(EditDefaultsOnly)
		float DashDistanceTolerance = 120.f;

	UPROPERTY(EditDefaultsOnly)
		float DashZOffset = 70.f;

	UPROPERTY(EditDefaultsOnly)
		float DashIntoPhysicsStrength = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DashCooldown = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		UCurveFloat* DefaultDashCurve;

	UPROPERTY(EditDefaultsOnly)
		float DashWallAngleTolerance = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DefaultStunDuration = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool ShouldStunWhenDashIntoWall = false;
};
