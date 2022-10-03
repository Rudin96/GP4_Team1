// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_SpeedBoost.h"
#include "GP4_Powerup.h"

// Sets default values
AGP4_SpeedBoost::AGP4_SpeedBoost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP4_SpeedBoost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP4_SpeedBoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player && !BPAccessPlayer)
	{
		BPAccessPlayer = Player;
	}
}

void AGP4_SpeedBoost::Activate_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Act imp speed"));
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AGP4_SpeedBoost::DeActivate_Implementation, Timer, false);
}

void AGP4_SpeedBoost::DeActivate_Implementation()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Deact imp speed"));

}
