// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_FlingableProjectile.generated.h"

class AGP4_PlayerCharacter;
class AGP4_PlayerController;

UCLASS()
class GP4_TEAM1_API AGP4_FlingableProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGP4_FlingableProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		AGP4_PlayerCharacter* GetCurrentOwner();

	UFUNCTION(BlueprintCallable)
		void SetCurrentOwner(AGP4_PlayerCharacter* NewOwner);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		AGP4_PlayerController* GetCurrentOwnerController();

	UFUNCTION(BlueprintCallable)
		void SetCurrentOwnerController(AGP4_PlayerController* NewController);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AGP4_PlayerCharacter* CurrentOwner;
	AGP4_PlayerController* CurrentOwnerController;

};
