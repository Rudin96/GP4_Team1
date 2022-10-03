// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.h"
#include "GP4_Shrink.generated.h"

UCLASS()
class GP4_TEAM1_API AGP4_Shrink : public AActor, public IPowerup
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
		float Timer = 5.0f;
	// Created to get access of the Player variable (IPowerup) from blueprints
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* BPAccessPlayer;

	// Sets default values for this actor's properties
	AGP4_Shrink();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate_Implementation() override;

	virtual void DeActivate_Implementation() override;
};
