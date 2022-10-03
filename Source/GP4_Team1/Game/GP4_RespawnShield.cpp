// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_RespawnShield.h"
#include "Components/SphereComponent.h"

// Sets default values
AGP4_RespawnShield::AGP4_RespawnShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	TempShield = CreateDefaultSubobject<USphereComponent>(TEXT("Shield Sphere"));
	TempShield->SetSphereRadius(Radius);
	TempShield->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGP4_RespawnShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP4_RespawnShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGP4_RespawnShield::AttachToPlayer(AActor* Player)
{
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
	AttachToActor(Player, rules);
	FTimerHandle WaitAFewFramesHandle;
	GetWorldTimerManager().SetTimer(WaitAFewFramesHandle, this,
		&AGP4_RespawnShield::RemoveFromPlayer, LifeSpan, false);
}

void AGP4_RespawnShield::RemoveFromPlayer()
{
	FDetachmentTransformRules rules(EDetachmentRule::KeepRelative,
		EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false);
	DetachFromActor(rules);
	Destroy();
}

