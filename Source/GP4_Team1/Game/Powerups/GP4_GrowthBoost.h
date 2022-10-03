// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.h"
#include "GP4_GrowthBoost.generated.h"

class AGP4_PlayerCharacter;
class USphereComponent;
class UStaticMeshComponent;
class UGP4_GrapplingHook;

UCLASS()
class GP4_TEAM1_API AGP4_GrowthBoost : public AActor, public IPowerup
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		float Timer = 5.0f;
	UPROPERTY(EditAnywhere)
		int HitsShieldCanTake = 3;
	UPROPERTY(EditAnywhere)
		float GrowthScale = 2.5f;
	UPROPERTY(EditAnywhere)
		float OriginalScale = 1.6f;

	UPROPERTY(EditAnywhere)
		float Radius = 120.0f;
	UPROPERTY(EditAnywhere)
		USphereComponent* ShieldZone;
	UPROPERTY(EditAnywhere)
		USphereComponent* TriggerZone;
	UPROPERTY(EditAnywhere)
		int TriggerOffset = 5;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ShieldMesh;
	UPROPERTY(EditAnywhere)
		float MeshScale = 2.0f;
	UPROPERTY(EditAnywhere)
		int PushRadius = 500;
	UPROPERTY(EditAnywhere)
		int PushForce = 600000;

	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		UGP4_GrapplingHook* GrapplingHook;
	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		int OriginalSpeed = 3000;
	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		int OriginalStrength = 1500;
	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		int OriginalActivateDistance = 3000;
	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		int OriginalDistanceObject = 1500;
	UPROPERTY(EditAnywhere, Category = "GrappleHookSettings")
		int OriginalDistancePlayer = 1500;

	UPROPERTY(VisibleAnywhere)
		AGP4_PlayerCharacter* PlayerChar;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* RootCmp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PlayerMesh;
public:
	// Sets default values for this actor's properties
	AGP4_GrowthBoost(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void WaitForPlayerInstance();

	UFUNCTION()
	virtual void Activate_Implementation() override;
	UFUNCTION()
	virtual void DeActivate_Implementation() override;
	UFUNCTION(BlueprintCallable)
	void DamageShield();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
