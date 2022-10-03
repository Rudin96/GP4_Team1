#pragma once
#include "Blueprint/UserWidget.h"
#include "GP4_PlayerStats.generated.h"

class UTextBlock;
class UImage;
class AGP4_Team1GameModeBase;

UCLASS()
class UGP4_PlayerStats : public UUserWidget
{
	GENERATED_BODY()
public:
	int playerIndex;
	int score = 0;
	
	UPROPERTY()
	AGP4_Team1GameModeBase* GameMode;

	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* PlayerText();
	UFUNCTION(BlueprintImplementableEvent)
	UTextBlock* ScoreText();
	UFUNCTION(BlueprintImplementableEvent)
	UImage* BackgroundImage();

	void UpdateStats();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void SetTeamColors();

private:
	bool bSetColors = true;
};
