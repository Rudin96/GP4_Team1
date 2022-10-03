// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GP4_Team1/Game/GP4_Team1GameModeBase.h"
#include "Utils/GP4_CharacterDataAsset.h"
#include "GP4_PlayerCharacter.generated.h"

class UGP4_WeaponBase;
class UArrowComponent;
class UTextRenderComponent;

UCLASS(Blueprintable)
class GP4_TEAM1_API AGP4_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		ETeam AssignedTeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UArrowComponent* Arrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTextRenderComponent* DebuffText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UForceFeedbackEffect* KillForceFeedBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UGP4_CharacterDataAsset* CharacterStats;

	UPROPERTY(BlueprintReadOnly)
		bool bIsStunned = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bCanDash = true;

	UPROPERTY(BlueprintReadOnly)
		int PlayerID = 0;

	UPROPERTY()
		TArray<UGP4_WeaponBase*> EquippedWeapons;

private:
	void PerformDash(UCurveFloat* Curve);
	bool CheckIfHitWall(float WallAngle, const FHitResult& HitRes);
	//AGP4_PlayerController* GetOwnerController() { return Cast<AGP4_PlayerController>(GetController()); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AGP4_PlayerCharacter();

	virtual void LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ);

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void FirePrimaryWeapon();
	void FireSecondaryWeapon();

	UFUNCTION(BlueprintCallable)
		float GetStunTimerRemaining();

	UFUNCTION(BlueprintImplementableEvent)
		void OnStunned();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnStunned();

	UFUNCTION(BlueprintCallable)
		void RechargeDash();

	void ResetPrimaryWeapon();

	UFUNCTION(BlueprintCallable)
		void SetDashCurve(UCurveFloat* NewCurve);

	UFUNCTION(BlueprintCallable)
		void SetStunnedIntoWall(bool bNewStun);
	
	UFUNCTION(BlueprintCallable)
		void Stun(float Duration = 0.f);

	UFUNCTION(BlueprintCallable)
		void UnStun();

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
		void OnDeath();

	UFUNCTION(BlueprintCallable, Category = "Player | Health")
		float GetCurrentHealth() { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Death")
		void Kill(AController* KillerController, AActor* KillerActor);

	UFUNCTION(BlueprintCallable)
		void Dash(FVector EndPos, UCurveFloat* Curve, AActor* DestActor, float DashDistanceTolerance = 0.f, bool bStunOverride = false, bool bGrappleOverride = false);

	UFUNCTION(BlueprintCallable)
		void Grapple(FVector EndPos, UCurveFloat* Curve, AActor* DestActor, float DashDistanceTolerance = 0.f);


private:
	UPROPERTY()
		UCurveFloat* DashOverrideCurve;
	UPROPERTY()
		bool bWantsToDash = false;
	UPROPERTY()
		bool bIsDashing = false;
	UPROPERTY()
		AActor* DashToActor;
	UPROPERTY()
		float DashDistToleranceOverride = 0.f;

	bool bStunnedWhenHittingWall = false;

	FVector DashStart;
	FVector DashEnd;

	float CurrentDashTime = 0.f;

	int Health = 0;

	FTimerHandle StunTimerHandle;
	FTimerHandle DashTimerHandle;
};
