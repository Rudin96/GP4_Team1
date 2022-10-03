// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include <GP4_Team1/Player/GP4_PlayerCharacter.h>

// Sets default values
AGP4_CameraActor::AGP4_CameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGP4_CameraActor::BeginPlay()
{
	Super::BeginPlay();
	if (!bFollowPlayers)
	{
		PrimaryActorTick.bCanEverTick = false;
		bActiveLookingForPlayers = false;
	}
	/*GEngine->AddOnScreenDebugMessage(
		INDEX_NONE,
		0.f,
		FColor::Red,
		FString::Printf(TEXT("tick tock "))
	);*/
}

// Called every frame
void AGP4_CameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Players.Num() == NrPlayers && bActiveLookingForPlayers)
	{
		// Camera location
		Target = GetTarget(Locations);
		FVector Location = FMath::Lerp(GetActorLocation(), Target, FollowSpeed * DeltaTime);
		SetActorLocation(Location);
		// FOV
		FieldOfView = GetFieldOfView(Distance, DeltaTime);
		Camera->SetFieldOfView(FieldOfView);
	}
	else
	{
		if (!SetActors())
		{
			GEngine->AddOnScreenDebugMessage(
				INDEX_NONE,
				0.f,
				FColor::Red,
				FString::Printf(TEXT("Error: Not enough players found for camera!! Panic! ...called from CameraActor::Tick"))
			);
		}
	}
}

bool AGP4_CameraActor::SetActors()
{	
	// Find and set actors as Players
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_PlayerCharacter::StaticClass(), Players);
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetViewTarget(this);

	bool bFoundEveryActor = true;

	NrPlayers = Players.Num();
	for (int32 i = 0; i < NrPlayers; i++)
	{
		Locations.Add(Players[i]->GetActorLocation());
		if (!Players[i])
		{
			bFoundEveryActor = false;
		}
	}
	return bFoundEveryActor;
}

FVector AGP4_CameraActor::GetTarget(TArray<FVector> Vectors)
{
	NrPlayers = Players.Num();
	FVector VecSum = FVector::ZeroVector;
	for (int32 i = 0; i < NrPlayers; i++)
	{
		// Check if a player with index i is spawning at this frame
		if (IndexOfSpawningPlayer.Contains(i))
		{
			int32 Index = -1;
			for (int32 j = 0; j < IndexOfSpawningPlayer.Num(); j++)
			{
				if (IndexOfSpawningPlayer[j] == i)
				{
					Index = j;
					break;
				} 
			}
			// this check is necessary, dont ask me why...
			if (Index < Locations.Num())
			{
				VecSum += Locations[Index];
			}
		}
		else if (Players[i])
		{
			FVector Vec = Players[i]->GetActorLocation();
			VecSum += Vec;
			Vectors[i] = Vec;
		}
	}
	Distance = GetBiggestDistance(Vectors);

	return VecSum / NrPlayers;
}

float AGP4_CameraActor::GetBiggestDistance(TArray<FVector> Vectors)
{
	if (Vectors.Num() < 2)
	{
		return Distance;
	}

	float MaxDistance = 0;
	for (int32 i = 1; i < Vectors.Num(); i++)
	{
		float NewDist = FVector::Distance(Vectors[i - 1], Vectors[i]);
		if (MaxDistance < NewDist)
		{
			MaxDistance = NewDist;
		}
	}
	return MaxDistance;
}

float AGP4_CameraActor::GetFieldOfView(float ActorDistance, float Delta)
{
	// Calculate FOV for camera and lerp for smoothness
	// Incl. a safe play if Distance sensitivity is set as zero for some weird reason
	FieldOfView = FMath::Lerp(FieldOfView, ActorDistance / (DistanceSensitivity + SMALL_NUMBER),
		Delta * ZoomSpeed);
	// Clamping to be safe
	return FMath::Clamp(FieldOfView, FOVMin, FOVMax);
}

bool AGP4_CameraActor::SetLocationManually(int32 Index, FVector Location)
{
	if (Locations.Num() > Index)
	{
		IndexOfSpawningPlayer.Add(Index);
		Locations[Index] = Location;
		return true;
	}
	return false;
}

void AGP4_CameraActor::ResetSpawnIndex(int32 Index)
{
	if (IndexOfSpawningPlayer.Contains(Index))
	{
		IndexOfSpawningPlayer.Remove(Index);
	}
}

