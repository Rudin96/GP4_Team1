// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_HookForce.h"
#include "GP4_Powerup.h"

// Sets default values
AGP4_HookForce::AGP4_HookForce()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGP4_HookForce::BeginPlay()
{
	Super::BeginPlay();
	if (Player && !BPAccessPlayer)
	{
		BPAccessPlayer = Player;
	}
}

// Called every frame
void AGP4_HookForce::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGP4_HookForce::Activate_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Act imp hook force"));
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AGP4_HookForce::DeActivate_Implementation, Timer, false);

}

void AGP4_HookForce::DeActivate_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Deact imp hook force"));

}
