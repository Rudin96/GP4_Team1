// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.h"
#include "GP4_Blackhole.generated.h"

class USphereComponent;

UCLASS()
class GP4_TEAM1_API AGP4_Blackhole : public AActor, public IPowerup
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
		float Timer = 5.0f;
	UPROPERTY(EditAnywhere)
	float Radius = 650.0f;
	UPROPERTY(EditAnywhere)
		int32 DropDistance = 800;
	UPROPERTY(EditAnywhere)
		USphereComponent* TriggerZone;
	UPROPERTY(EditAnywhere)
	float PullSpeed = 2;
	UPROPERTY(EditAnywhere)
	float ScaleSpeed = 0.3f;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlackHoleMesh;
	UPROPERTY(VisibleAnywhere)
		bool bActivated = false;
	UPROPERTY(VisibleAnywhere)
		bool bCanActivate = true;

	UPROPERTY(EditAnywhere)
	FVector ScaleTarget = FVector(0.3f, 0.3f, 0.3f);
	UPROPERTY(EditAnywhere)
		float PlayerOrginialScale = 1.6f;
	UPROPERTY(EditAnywhere)
	bool bFirstBlackhole = true;
private:
	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> HitActors;

	UPROPERTY(VisibleAnywhere)
		bool bSetInput = false;
	// Read this variable name carefully
	UPROPERTY(VisibleAnywhere)
	bool bIsHit = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootCmp;

	UPROPERTY(VisibleAnywhere)
		AGP4_Blackhole* Other;

	UPROPERTY(VisibleAnywhere)
		AGP4_Powerup* Powerup;

	UPROPERTY(VisibleAnywhere)
	bool FoundCloseHole = false;
	UPROPERTY(VisibleAnywhere)
		float MeshScale = 1.0f;

public:
	// Sets default values for this actor's properties
	AGP4_Blackhole(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	FVector MoveActorCloser(AActor* Actor, float DeltaTime);
	UFUNCTION()
	void MoveCursor();

	virtual void Activate_Implementation() override;

	virtual void DeActivate_Implementation() override;

	UFUNCTION()
	void DropBlackHole();
	UFUNCTION()
	void LookForOtherBlackholes();
	UFUNCTION()
		void DeActivateOtherBlackhole();
	UFUNCTION()
	void IncreaseSize(AGP4_Blackhole* OtherBlackhole);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UFUNCTION()
	AGP4_Powerup* GetFirstPowerupInstance();
};
