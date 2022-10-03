// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPowerup : public UInterface
{
	GENERATED_BODY()
};

class IPowerup
{
	GENERATED_BODY()

public:
	AActor* Player;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Activate();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeActivate();
};

class USphereComponent;
class UStaticMeshComponent;
class AGP4_Blackhole;
class AGP4_PlayerCharacter;

UCLASS()
class GP4_TEAM1_API AGP4_Powerup : public AActor 
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = TriggerSphere)
	USphereComponent* Trigger;
	UPROPERTY(EditAnywhere, Category = TriggerSphere)
	float SphereRadius = 100;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = PowerUp)
		TArray<TSubclassOf<AActor>> Powerups;
	UPROPERTY(EditAnywhere, Category = PowerUp)
		float TimerInterval = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = PowerUpMovement)
		float Delta = 0;
	UPROPERTY(VisibleAnywhere, Category = PowerUpMovement)
		float RunningTime = 0;
	UPROPERTY(EditAnywhere, Category = PowerUpMovement)
		float MaxBound = 5;
	UPROPERTY(EditAnywhere, Category = PowerUpMovement)
		FVector Movement = FVector(0, 0, 15);
	UPROPERTY(EditAnywhere, Category = PowerUpMovement)
		FRotator RotationSpeed = FRotator(20, 20, 20);
	UPROPERTY(VisibleAnywhere, Category = PowerUpMovement)
		FVector StartLocation;
	  
	UPROPERTY(VisibleAnywhere, Category = BlackHole)
		TArray<AGP4_Blackhole*> Blackholes;

private:
	bool bIsDone = false;

public:	
	// Sets default values for this actor's properties
	AGP4_Powerup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void SetLocationAndRotation(float DeltaTime);
	UFUNCTION()
		void ResumePowerup();

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void GetPowerup(AActor* Player);
	UFUNCTION()
	void RemovePowerups(AGP4_PlayerCharacter* FromPlayer);
	UFUNCTION()
	void UpdateBlackholeList(AGP4_Blackhole* RemoveItem);
};
