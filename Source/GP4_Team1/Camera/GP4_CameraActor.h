// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP4_CameraActor.generated.h"


class UCameraComponent;

UCLASS()
class GP4_TEAM1_API AGP4_CameraActor : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, meta = (ToolTip = "This is set in the game, NOTE: dont have it at zero!"))
		int32 NrPlayers = 2;
	UPROPERTY(VisibleAnywhere)
		TArray<AActor*> Players;
	UPROPERTY(EditAnywhere)
		TArray<FVector> Locations;
		bool bActiveLookingForPlayers = true;
	UPROPERTY(EditAnywhere)
		TArray<int32> IndexOfSpawningPlayer;

	UPROPERTY(VisibleAnywhere, Category = Default)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category = Default)
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = Follow)
		int32 FollowSpeed = 5;

	UPROPERTY(EditAnywhere, Category = Follow)
		bool bFollowPlayers =false;
	UPROPERTY(VisibleAnywhere, Category = Follow)
		FVector Target;
	UPROPERTY(EditAnywhere, Category = Follow, meta = (ToolTip = "The speed the camera zooms in"))
		float ZoomSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = Follow)
		int16 FOVMax = 100;
	UPROPERTY(EditAnywhere, Category = Follow)
		int16 FOVMin = 60;

	UPROPERTY(VisibleAnywhere, Category = Follow, meta = (ToolTip = "Default FOV for camera"))
		float FieldOfView = 70.0f;

	UPROPERTY(VisibleAnywhere, Category = DistanceSettings)
		float Distance = 0.0f;

	UPROPERTY(EditAnywhere, Category = DistanceSettings, meta = (ToolTip = 
		"How sensitive the camera zooms depending on distance between players. Should be (Current Distance between the players / Field of View) - a small offset | the smaller the more sensitive"))
		float DistanceSensitivity = 29.0f;

public:	
	// Sets default values for this actor's properties
	AGP4_CameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool SetActors();
	UFUNCTION()
	FVector GetTarget(TArray<FVector> Vectors);

	UFUNCTION()
	float GetBiggestDistance(TArray<FVector> Vectors);
	UFUNCTION()
	float GetFieldOfView(float ActorDistance, float Delta);

	UFUNCTION(BlueprintCallable, meta = (ToolTip = 
		"Set location for the camera at a specific index when player with the same index dies. Returns false if the index is not found"))
	bool SetLocationManually(int32 Index, FVector Location);

	UFUNCTION(BlueprintCallable, meta = (ToolTip =
		"Should be called when player with this index respawns after dying."))
	void ResetSpawnIndex(int32 Index);
};
