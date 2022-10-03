// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_FlingableProjectile.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"
#include "GP4_Team1/Player/GP4_PlayerController.h"

// Sets default values
AGP4_FlingableProjectile::AGP4_FlingableProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP4_FlingableProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

AGP4_PlayerCharacter* AGP4_FlingableProjectile::GetCurrentOwner()
{
	if (CurrentOwner != nullptr)
		return CurrentOwner;
	else
		return nullptr;
}

void AGP4_FlingableProjectile::SetCurrentOwner(AGP4_PlayerCharacter* NewOwner)
{
	CurrentOwner = NewOwner;
}

AGP4_PlayerController* AGP4_FlingableProjectile::GetCurrentOwnerController()
{
	if (CurrentOwnerController != nullptr)
		return CurrentOwnerController;
	else
		return nullptr;
}

void AGP4_FlingableProjectile::SetCurrentOwnerController(AGP4_PlayerController* NewOwnerController)
{
	CurrentOwnerController = NewOwnerController;
}

// Called every frame
void AGP4_FlingableProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

