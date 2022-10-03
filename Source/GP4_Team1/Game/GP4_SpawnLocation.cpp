// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_SpawnLocation.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"
#include "GP4_Team1/Utils/GP4_Macros.h"

// Sets default values
AGP4_SpawnLocation::AGP4_SpawnLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
}

// Called when the game starts or when spawned
void AGP4_SpawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP4_SpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGP4_SpawnLocation::CheckForCloseObstacles(TArray<AActor*> OtherPlayers)
{
	// Set what actors to seek out from it's collision channel
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_PhysicsBody));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_WorldDynamic));
	// Ignore any specific actors
	TArray<AActor*> IgnoreActors;
	// Ignore self or remove this line to not ignore any
	IgnoreActors.Init(this, 1);

	// Array of actors that are inside the radius of the sphere
	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 
		CheckForObstaclesDistance, TraceObjectTypes, nullptr,  IgnoreActors, OutActors);
	
	for (int i = 0; i < OtherPlayers.Num(); i++)
	{
		OutActors.Add(OtherPlayers[i]);
	}

	for (int j = 0; j < OutActors.Num(); j++)
	{
		bool IsObstacleClose = OutActors[j] && CheckForObstaclesDistance > FVector::Distance(
			OutActors[j]->GetActorLocation(), GetActorLocation());
		if (IsObstacleClose)
		{
			return false;
		}
	}

	return true;
}

