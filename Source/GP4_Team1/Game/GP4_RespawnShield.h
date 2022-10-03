// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_RespawnShield.generated.h"

class USphereComponent;

UCLASS()
class GP4_TEAM1_API AGP4_RespawnShield : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
		USphereComponent* TempShield;
	UPROPERTY(EditAnywhere)
		int Radius = 210;
	UPROPERTY(EditAnywhere)
		float LifeSpan = 10.0f;

	// Sets default values for this actor's properties
	AGP4_RespawnShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void AttachToPlayer(AActor* Player);
	UFUNCTION()
		void RemoveFromPlayer();
};
