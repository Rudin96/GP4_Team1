#pragma once
#include "Blueprint/UserWidget.h"
#include "GP4_MainWidget.generated.h"

class UGP4_PlayerStats;
class AGP4_Team1GameModeBase;

UCLASS()
class UGP4_MainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int PlayerCount = 2;
	
	UFUNCTION(BlueprintImplementableEvent)
	TArray<UNamedSlot*> GetPlayerSlots();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGP4_PlayerStats> PlayerWidgetClass;

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	AGP4_Team1GameModeBase* GameMode;
	
	void AddPlayerWidget(int index);
};
