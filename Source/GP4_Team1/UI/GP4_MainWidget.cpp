#include "GP4_MainWidget.h"
#include "GP4_Team1/Game/GP4_Team1GameModeBase.h"
#include "Components/NamedSlot.h"
#include "Score/GP4_PlayerStats.h"

void UGP4_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	GameMode = GetWorld()->GetAuthGameMode<AGP4_Team1GameModeBase>();

	for (size_t i = 0; i < PlayerCount; i++)
	{
		AddPlayerWidget(i);
	}
}

void UGP4_MainWidget::AddPlayerWidget(int index)
{
	UGP4_PlayerStats* playerStatsWidget = NewObject<UGP4_PlayerStats>(this, PlayerWidgetClass);
	playerStatsWidget->playerIndex = index;
	GameMode->PlayerStats.Add(playerStatsWidget);
	GetPlayerSlots()[index]->AddChild(playerStatsWidget);
}
