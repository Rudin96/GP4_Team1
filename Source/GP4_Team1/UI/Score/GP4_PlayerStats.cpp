#include "GP4_PlayerStats.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GP4_Team1/Game/GP4_Team1GameModeBase.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"
#include "GP4_Team1/UI/GP4_MainWidget.h"

void UGP4_PlayerStats::NativeConstruct()
{
	Super::NativeConstruct();
	GameMode = GetWorld()->GetAuthGameMode<AGP4_Team1GameModeBase>();
	
	UpdateStats();
}

void UGP4_PlayerStats::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(bSetColors)
		SetTeamColors();
}

void UGP4_PlayerStats::SetTeamColors()
{
	if(GameMode->Players.Num() < 1)
	{
		return;
	}
		
	if(GameMode->Players[playerIndex]->AssignedTeam == ETeam::TEAM_RED)
	{
		BackgroundImage()->SetBrushTintColor(FColor::Red);
	}
	else if(GameMode->Players[playerIndex]->AssignedTeam == ETeam::TEAM_BLUE)
	{
		BackgroundImage()->SetBrushTintColor(FColor::FromHex(FString("#5392FFFF")));
	}
	bSetColors = false;
}

void UGP4_PlayerStats::UpdateStats()
{
	PlayerText()->SetText(FText::FromString(FString::Printf(TEXT("Player %d"), playerIndex + 1)));
	ScoreText()->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
}
