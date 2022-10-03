// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_Shrink.h"
#include "GP4_Powerup.h"

// Sets default values
AGP4_Shrink::AGP4_Shrink()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP4_Shrink::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP4_Shrink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player && !BPAccessPlayer)
	{
		BPAccessPlayer = Player;
	}
}

void AGP4_Shrink::Activate_Implementation()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AGP4_Shrink::DeActivate_Implementation, Timer + 0.5f, false);
	// Timer + 0.5f the offset is to wait for blueprints to finish
}

void AGP4_Shrink::DeActivate_Implementation()
{
	Destroy();

}