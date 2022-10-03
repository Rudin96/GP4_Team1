// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP4_WeaponBase.h"
#include "GP4_GrapplingHook.generated.h"

/**
 * 
 */

class USphereComponent;
class ACableActor;
class UCableComponent;
class UGP4_GrappleHookDataAsset;
class AGP4_FlingableProjectile;

UCLASS()
class GP4_TEAM1_API UGP4_GrapplingHook : public UGP4_WeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
		int ShotCount = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UGP4_GrappleHookDataAsset* GrappleStats;
public:

	void Fire_Implementation() override;
	void SecondaryFire_Implementation() override;
	void TickWeapon_Implementation(float DeltaSeconds) override;

	void ResetWeapon() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void OnFire(); 
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void OnSecondaryFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void OnProjectileImpact();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void OnResetWeapon();

	UFUNCTION()
		void SpawnCable(AActor* AttachActor, AActor* AttachActor2);

	UFUNCTION(BlueprintCallable)
		void StartResetWeaponTimer(float Duration);

	UFUNCTION()
		void ProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void FlingProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void FlingProjectileOverlap(AActor* OverlappedActor, AActor* OtherActor);

private:
	UPROPERTY()
		TArray<AActor*> HitActors;

	UPROPERTY()
		TArray<AActor*> Projectiles;
	
	UPROPERTY()
		TArray<ACableActor*> Ropes;

	FTimerHandle ResetWeaponHandle;
};
