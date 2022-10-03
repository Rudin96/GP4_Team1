// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_Powerup.h"
#include "GP4_HookForce.generated.h"

UCLASS()
class GP4_TEAM1_API AGP4_HookForce : public AActor, public IPowerup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Timer = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* BPAccessPlayer;
public:	
	// Sets default values for this actor's properties
	AGP4_HookForce();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate_Implementation() override;

	virtual void DeActivate_Implementation() override;

};
