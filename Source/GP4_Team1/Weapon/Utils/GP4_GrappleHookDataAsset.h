// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GP4_GrappleHookDataAsset.generated.h"

/**
 * 
 */

class UCableComponent;
class ACableActor;

UCLASS()
class GP4_TEAM1_API UGP4_GrappleHookDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team RED")
		TSubclassOf<AActor> ProjectileClassRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team RED")
		TSubclassOf<ACableActor> SlimeStringClassRed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team BLUE")
		TSubclassOf<AActor> ProjectileClassBlue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Team BLUE")
		TSubclassOf<ACableActor> SlimeStringClassBlue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		float ProjectileSpeed = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		float ProjectileScale = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
		float SlimeStringThiccness = 40.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int MaxShotCount = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ProjectileSpawnOffset = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ResetWeaponCooldown = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fling")
		float FlingStrength = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fling")
		float GrappleStopDistanceToObject = 1500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fling")
		float GrappleStopDistanceToPlayer = 1500.f;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fling")
		bool bOverrideXY = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fling")
		bool bOverrideZ = true;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bShouldFireOnMaxShots = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fling | Character")
		UCurveFloat* GrappleCurveSelf;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fling | Character")
		UCurveFloat* GrappleCurveOtherPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fling | Character")
		UCurveFloat* GrappleCurveObjectPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fling | Character")
		float StunDuration = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fling | Character")
		float StunDurationIntoWall = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ActivateDistance = 3000.f;
};
