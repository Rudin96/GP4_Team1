// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.h"
#include "GP4_ContactDamage.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class GP4_TEAM1_API AGP4_ContactDamage : public AActor, public IPowerup
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
		float Timer = 4.0f;

	UPROPERTY(EditAnywhere)
		int32 Radius = 135;
	UPROPERTY(EditAnywhere)
		USphereComponent* TriggerZone;
	UPROPERTY(EditAnywhere)
		int32 Damage = 1;
	UPROPERTY(EditAnywhere)
		bool bActive = false;
	UPROPERTY(EditAnywhere)
		bool bHasOtherPlayerShield = false;
	UPROPERTY(EditAnywhere)
		bool bPushing = true;
private:
	UPROPERTY(VisibleAnywhere)
	FVector PlayerScale;
	UPROPERTY(VisibleAnywhere)
	FVector MeshScale;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlayerMesh;

public:	
	// Sets default values for this actor's properties
	AGP4_ContactDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void WaitForPlayerInstance();
	UFUNCTION()
	void SetupRelativeScaleValues();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Activate_Implementation() override;

	virtual void DeActivate_Implementation() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
