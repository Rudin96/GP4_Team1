// Copyright Epic Games, Inc. All Rights Reserved.


#include "GP4_Team1GameModeBase.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"
#include "GP4_Team1/Game/GP4_SpawnLocation.h"
#include "GP4_Team1/Game/Powerups/GP4_Powerup.h"
#include "GP4_Team1/Camera/GP4_CameraActor.h"
#include "GP4_Team1/UI/Score/GP4_PlayerStats.h"
#include "GP4_Team1/Utils/GP4_Macros.h"
#include "Camera/CameraComponent.h"
#include "GP4_RespawnShield.h"
#include "MatineeCameraShake.h"

AGP4_Team1GameModeBase::AGP4_Team1GameModeBase()
{
	DefaultPawnClass = nullptr;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGP4_Team1GameModeBase::StartPlay()
{
	Super::StartPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_SpawnLocation::StaticClass(), SpawnLocations);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), FoundControllers);
	
	if (SpawnPoints.Num() >= NumberOfPlayers)
	{
		// Spawn the players
		for (size_t i = 0; i < NumberOfPlayers; i++)
		{
			AGP4_PlayerCharacter* Character = SpawnPlayer(i, SpawnPoints[i]->GetActorLocation(),
				SpawnPoints[i]->GetActorRotation());

			Players.Add(Character);
		}
	}
	else
	{
		LOG(15.0f, FColor::Red, TEXT("Error: no start points located/found in the level, add them if you want to play!"));
	}

	GetWorld()->GetTimerManager().SetTimer(EndGameHandle, this, &AGP4_Team1GameModeBase::EndGame, GameLengthSeconds, false, -1.f);
	Camera = FindCamera();
}

void AGP4_Team1GameModeBase::Tick(float DeltaTime)
{
	// Checks in case a player doesnt respawn properly
	if (Players.Num() > 1 && RespawnState == ERespawnState::NO_PLAYERS)
	{
		if (!Player0 || !Players[1])
		{
			if (!Player0)
			{
				Index = 1;
				ReSpawnPlayer();
				RespawnState = ERespawnState::PLAYER0;
			}
			else
			{
				Index = 0;
				ReSpawnPlayer();
				RespawnState = ERespawnState::PLAYER1;

			}
			
			
		}
	}	
	if (Players.Num() > 0 && RespawnState == ERespawnState::NO_PLAYERS)
	{
		if (!Player1 || !Players[0])
		{
			Index = 0;
			ReSpawnPlayer();
			LOG(3.0f, FColor::Red, TEXT("Error: el, add them if you want to play!"));

		}
	}
	
}

AGP4_PlayerCharacter* AGP4_Team1GameModeBase::SpawnPlayer(int PlayerIndex, FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AGP4_PlayerCharacter* NewCharacter = GetWorld()->SpawnActor<AGP4_PlayerCharacter>(
		PlayerCharClass, Location, Rotation, SpawnParams);
	
	if (PlayerIndex % 2 && NewCharacter)
	{
		NewCharacter->AssignedTeam = ETeam::TEAM_RED;
		Player1 = NewCharacter;
	}
	else if (NewCharacter)
	{
		NewCharacter->AssignedTeam = ETeam::TEAM_BLUE;
		Player0 = NewCharacter;

	}
	// Get and/or spawn a Playercontroller for every player
	APlayerController* PC = nullptr;
	int NumberOfControllers = FoundControllers.Num();
	for (int i = 0; i < NumberOfControllers; i++)
	{
		if (PlayerIndex == i)
		{
			PC = Cast<APlayerController>(FoundControllers[i]);
			
			PC->bAutoManageActiveCameraTarget = false;
			PC->Possess(NewCharacter);
		}
	}
	if (NumberOfControllers < NumberOfPlayers)
	{
		for (size_t i = 0; i < NumberOfPlayers; i++)
		{
			if (PlayerIndex == i)
			{
				PC = UGameplayStatics::CreatePlayer(GetWorld(), i);
				if (PC)
				{
					PC->Possess(NewCharacter);
					FoundControllers.Add(PC);
				}
			}
		}
	}

	if (NewCharacter)
	{
		if (PlayerIndex == 0 && !(NewCharacter->Controller->InputComponent))
		{
			NewCharacter->Controller->InputComponent = Player0Input;
		}
		else if (!(NewCharacter->Controller->InputComponent))
		{
			NewCharacter->Controller->InputComponent = Player1Input;
		}
		
	}
	
	return NewCharacter;
}

int AGP4_Team1GameModeBase::CheckIfAPlayerWon()
{
	TArray<int> Scores;

	for (size_t i = 0; i < PlayerStats.Num(); i++)
	{
		Scores.Add(PlayerStats[i]->score);
		/*if (PlayerStats[i]->score == ScoreToWin)
		{
			LOG(5.f, FColor::Green, FString::Printf(TEXT("Player %i won the game!"), i + 1))
			break;
		}*/
	}

	int MaxScore = 0;
	int PlayerWithMaxScore = 0;

	for (size_t i = 0; i < Scores.Num(); i++)
	{
		if (Scores[i] == ScoreToWin)
		{
			break;
		}
		if (Scores[i] > MaxScore)
		{
			MaxScore = Scores[i];
			PlayerWithMaxScore = i;
		}
	}
	if (MaxScore == 0)
		return 0;
	else
		return PlayerWithMaxScore + 1;

	LOG(5.f, FColor::Green, FString::Printf(TEXT("Player %i won the game!"), PlayerWithMaxScore + 1))
}

uint8 AGP4_Team1GameModeBase::GetGameTimeRemainingSeconds()
{
	//Todo return seconds here
	int RemainingSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(EndGameHandle);
	return RemainingSeconds % 60;
}

int AGP4_Team1GameModeBase::GetGameTimeRemainingMinutes()
{
	//Todo return minutes here
	float RemainingSeconds = GetWorld()->GetTimerManager().GetTimerRemaining(EndGameHandle);
	return RemainingSeconds / 60;
}

uint8 AGP4_Team1GameModeBase::GetGameTimeElapsedSeconds()
{
	int RemainingSeconds = GetWorld()->GetTimerManager().GetTimerElapsed(EndGameHandle);
	return RemainingSeconds % 60;
}

int AGP4_Team1GameModeBase::GetGameTimeElapsedMinutes()
{
	float RemainingSeconds = GetWorld()->GetTimerManager().GetTimerElapsed(EndGameHandle);
	return RemainingSeconds / 60;
}

void AGP4_Team1GameModeBase::EndGame()
{
	//Show Hud with which player won the game
	OnGameEnd(CheckIfAPlayerWon());
}

AGP4_CameraActor* AGP4_Team1GameModeBase::FindCamera()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_CameraActor::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		return Cast<AGP4_CameraActor>(FoundActors[0]);
	}
	else 
	{
		if (CameraClass != nullptr)
			return GetWorld()->SpawnActor<AGP4_CameraActor>(CameraClass);
		else
		{
			LOG(5.f, FColor::Red, TEXT("WARNING: No camera class specified, spawning base GP4_CameraClass"));
			return GetWorld()->SpawnActor<AGP4_CameraActor>();
		}
	}
}
// Gets called when player dies, checks if other player is currently respawning
void AGP4_Team1GameModeBase::StartRespawnPlayer(AGP4_PlayerCharacter* Player)
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakyShaky, 1);
	// Find powerups from the player and remove them
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_Powerup::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		AGP4_Powerup* FirstPickup = Cast<AGP4_Powerup>(FoundActors[0]);
		FirstPickup->RemovePowerups(Player);
	}

	bool TwoPlayersDead = false;
	if (RespawnState == ERespawnState::BOTH_PLAYERS)
	{
		return;
	}
	// Check if the other player is currently respawning
	bool OtherPlayerHasDied = false;
	if (RespawnState == ERespawnState::PLAYER0 && Player1 == Player)
	{
		OtherPlayerHasDied = true;
	}
	else if (RespawnState == ERespawnState::PLAYER1 && Player0 == Player)
	{
		OtherPlayerHasDied = true;
	}
	if (OtherPlayerHasDied)
	{
		TwoPlayersDead = true;
		BothPlayerDied();
		RespawnState = ERespawnState::BOTH_PLAYERS;
	}
	else
	{
		if (!Player0Input)
		{
			Player0Input = Player0->Controller->InputComponent;
		}
		if (!Player1Input)
		{
			Player1Input = Player1->Controller->InputComponent;
		}
		if (Player0 == Player && Index != 1)
		{
			Index = 0;
			RespawnState = ERespawnState::PLAYER0;
			RemoveController(Player0, Index);
		}
		else if (Player1 == Player && Index != 0)
		{
			Index = 1;
			RespawnState = ERespawnState::PLAYER1;

			RemoveController(Player1, Index);

		}
		// Code below only changes for the camera
		// Is index valid?
		//if (Index >= 0)
		//{
		//	// Make the camera act like the player hasnt moved
		//	Camera->SetLocationManually(Index, Player->GetActorLocation());
		//}
		//else
		//{
		//	// only ruins for the camera
		//	Index = 0;
		//	//LOG(25.0f, FColor::Red, TEXT("Error: player index not found in GameModeBase::StartRespawnPlayer()!!!1!!"));
		//}
	}
	Players.Remove(Player);
	Player->Destroy();
	
	if (!TwoPlayersDead)
	{
		// Find spawn locations and check if they are available
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_SpawnLocation::StaticClass(), SpawnLocations);
		SpawnLocations = UpdateSpawnLocations(SpawnLocations);
		RandIndex = 0;

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, 
			&AGP4_Team1GameModeBase::ReSpawnPlayer, SpawnDelay, false);
	}
}

bool AGP4_Team1GameModeBase::RemoveController(AGP4_PlayerCharacter* FromPlayer, int PlayerIndex)
{
	APlayerController* PC = Cast<APlayerController>(FoundControllers[PlayerIndex]);
	if (PC && FromPlayer && FromPlayer->Controller && FromPlayer->Controller->InputComponent)
	{
		FromPlayer->Controller->InputComponent = nullptr;
		PC->UnPossess(); 
		return true;
	}
	return false;
}

void AGP4_Team1GameModeBase::AddScore(int playerIndex, int value)
{
	PlayerStats[playerIndex]->score += value;
	PlayerStats[playerIndex]->UpdateStats();
	CheckIfAPlayerWon();
}

void AGP4_Team1GameModeBase::ReSpawnPlayer()
{
	RespawnState = ERespawnState::NO_PLAYERS;

	if (Index < 0)
	{
		return;
	}
	Camera->ResetSpawnIndex(Index);
	// Default safeplay spawn location and rotation
	FVector SpawnLoc = SpawnPoints[0]->GetActorLocation();
	FRotator SpawnRot = SpawnPoints[0]->GetActorRotation();
	if (SpawnLocations.Num() > 0)
	{
		RandIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		SpawnLoc = SpawnLocations[RandIndex]->GetActorLocation();
		SpawnRot = SpawnLocations[RandIndex]->GetActorRotation();
	}

	AGP4_PlayerCharacter* SpawnedCharacter = SpawnPlayer(Index, SpawnLoc, SpawnRot);
	Players.Add(SpawnedCharacter);

	// Spawn shield to cover when being respawned, that removes it self after a second or so
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AGP4_RespawnShield* TempShield = GetWorld()->SpawnActor<AGP4_RespawnShield>(
		RespawnShield, SpawnLoc, SpawnRot, SpawnParams);
	TempShield->AttachToPlayer(SpawnedCharacter);
	
	Camera->SetActors();
	// Set default value for index
	Index = -1;
}

TArray<AActor*> AGP4_Team1GameModeBase::UpdateSpawnLocations(TArray<AActor*> OldLocs)
{
	TArray<AActor*> NewLocs;
	TArray<AActor*> ClosePlayers;
	ClosePlayers.Add(Player0);
	ClosePlayers.Add(Player1);
	for (int32 i = 0; i < OldLocs.Num(); i++)
	{
		AGP4_SpawnLocation* Loc = Cast<AGP4_SpawnLocation>(OldLocs[i]);
		if (Loc->bIsActive && Loc->CheckForCloseObstacles(ClosePlayers))
		{
			NewLocs.Add(OldLocs[i]);
		}
	}
	return NewLocs;
}
// safeplay makes all players spawn after a timer
void AGP4_Team1GameModeBase::BothPlayerDied()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this,
		&AGP4_Team1GameModeBase::RespawnBothPlayers, 1, false);

	Index = -1;
}

void AGP4_Team1GameModeBase::RespawnBothPlayers()
{
	Players.Empty(0);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_SpawnLocation::StaticClass(), SpawnLocations);
	SpawnLocations = UpdateSpawnLocations(SpawnLocations);
	for (int i = 0; i < NumberOfPlayers; i++)
	{
		// from respawnplayer method
		FVector SpawnLoc = SpawnPoints[i]->GetActorLocation();
		FRotator SpawnRot = SpawnPoints[i]->GetActorRotation();
		if (SpawnLocations.Num() > 0)
		{
			RandIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);

			SpawnLoc = SpawnLocations[RandIndex]->GetActorLocation();
			SpawnRot = SpawnLocations[RandIndex]->GetActorRotation();
		}
		AGP4_PlayerCharacter* Character = SpawnPlayer(i, SpawnPoints[i]->GetActorLocation(),
			SpawnPoints[i]->GetActorRotation());
		// Spawn shield to cover when being respawned, that removes it self after a second or so
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AGP4_RespawnShield* TempShield = GetWorld()->SpawnActor<AGP4_RespawnShield>(
			RespawnShield, SpawnLoc, SpawnRot, SpawnParams);
		TempShield->AttachToPlayer(Character);
		Players.Add(Character);
	}
}
