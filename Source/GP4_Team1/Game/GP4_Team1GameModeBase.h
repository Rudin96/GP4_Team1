// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GP4_Team1GameModeBase.generated.h"

/**
 * 
 */
UENUM(Blueprintable)
enum class ETeam : uint8
{
	TEAM_RED,
	TEAM_BLUE
};

UENUM(Blueprintable)
enum class ERespawnState : uint8
{
	NO_PLAYERS,
	PLAYER0,
	PLAYER1,
	BOTH_PLAYERS
};

class UGP4_PlayerStats;
class UGP4_CameraShake;

DECLARE_DELEGATE_OneParam(FShakeDelegate, float);

UCLASS(Blueprintable)
class GP4_TEAM1_API AGP4_Team1GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int NumberOfPlayers = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int ScoreToWin = 6;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int GameLengthSeconds = 60;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AGP4_PlayerCharacter> PlayerCharClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AGP4_PlayerCharacter*> Players;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AGP4_PlayerCharacter* Player0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AGP4_PlayerCharacter* Player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AGP4_CameraActor> CameraClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPoints;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<class AActor*> SpawnLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnDelay = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AGP4_CameraActor* Camera;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMatineeCameraShake> CameraShakyShaky;

	UPROPERTY()
	TArray<UGP4_PlayerStats*> PlayerStats;


private:
	UPROPERTY(VisibleAnywhere)
		ERespawnState RespawnState = ERespawnState::NO_PLAYERS;

	UPROPERTY(VisibleAnywhere)
		int32 RandIndex;
	UPROPERTY(VisibleAnywhere)
	int32 Index = -1;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> FoundControllers;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UInputComponent> Player0Input;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UInputComponent> Player1Input;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGP4_RespawnShield> RespawnShield;

protected:
	virtual void StartPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		AGP4_PlayerCharacter* SpawnPlayer(int PlayerIndex, FVector Location, FRotator Rotation);
	
	UFUNCTION()
		AGP4_CameraActor* FindCamera();

public:
	AGP4_Team1GameModeBase();
	UFUNCTION(BlueprintCallable)
	void StartRespawnPlayer(AGP4_PlayerCharacter* Player);
	UFUNCTION(BlueprintCallable)
		bool RemoveController(AGP4_PlayerCharacter* FromPlayer, int PlayerIndex);
	UFUNCTION(BlueprintCallable)
	void AddScore(int playerIndex, int value);

	UFUNCTION(BlueprintCallable)
		int CheckIfAPlayerWon();

	UFUNCTION(BlueprintCallable)
		void EndGame();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		uint8 GetGameTimeRemainingSeconds();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetGameTimeRemainingMinutes();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		uint8 GetGameTimeElapsedSeconds();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetGameTimeElapsedMinutes();

	UFUNCTION(BlueprintImplementableEvent)
		void OnGameEnd(int PlayerWonindex);
private:
	UFUNCTION()
	void ReSpawnPlayer();
	UFUNCTION()
	TArray<AActor*> UpdateSpawnLocations(TArray<AActor*> OldLocs);

	UFUNCTION()
		void BothPlayerDied();
	UFUNCTION()
		void RespawnBothPlayers();

	FTimerHandle EndGameHandle;
};
