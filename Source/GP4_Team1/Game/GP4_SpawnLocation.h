// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_SpawnLocation.generated.h"

UCLASS()
class GP4_TEAM1_API AGP4_SpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CheckForObstaclesDistance = 600;


public:	
	// Sets default values for this actor's properties
	AGP4_SpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool CheckForCloseObstacles(TArray<AActor*> OtherPlayers);
};
