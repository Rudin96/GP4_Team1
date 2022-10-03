// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_Powerup.h"
#include "Components/SphereComponent.h"
#include <GP4_Team1/Player/GP4_PlayerCharacter.h>
#include "GP4_Blackhole.h"
#include "GP4_Team1/Utils/GP4_Macros.h"

// Sets default values
AGP4_Powerup::AGP4_Powerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	Trigger->SetSphereRadius(SphereRadius);
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGP4_Powerup::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGP4_Powerup::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

// Called every frame
void AGP4_Powerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP4_Powerup::SetLocationAndRotation(float DeltaTime)
{
	//// Get new Location
	//FVector NewLocation = GetActorLocation();
	//// Fluctuating the value between -1 and 1
	//Delta = FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime);

	//NewLocation += Movement * Delta * MaxBound;
	//RunningTime += DeltaTime;

	//// Get rotation
	//FRotator NewRot = GetActorRotation();
	//NewRot -= RotationSpeed * DeltaTime;
	//SetActorLocationAndRotation(NewLocation, NewRot);
}

void AGP4_Powerup::ResumePowerup()
{
	bIsDone = false;
	SetActorLocation(StartLocation);
}

void AGP4_Powerup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsDone && OtherActor->IsA(AGP4_PlayerCharacter::StaticClass()))
	{
		GetPowerup(OtherActor);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AGP4_Powerup::ResumePowerup, TimerInterval, false);
	}
}

void AGP4_Powerup::GetPowerup(AActor* PowerPlayer)
{
	bIsDone = true;
	int32 Index = FMath::RandRange(0, Powerups.Num() - 1);
	AActor* Actor = GetWorld()->SpawnActor<AActor>(Powerups[Index],
		GetActorLocation(), GetActorRotation());
	// Moves this actor out from the map
	SetActorLocation(FVector(0, 0, -100000));
	IPowerup* Powerup = Cast<IPowerup>(Actor);
	// Set the player for the power up before activating it
	Powerup->Player = PowerPlayer;
	Powerup->Execute_Activate(Powerup->_getUObject());
}

void AGP4_Powerup::RemovePowerups(AGP4_PlayerCharacter* FromPlayer)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), 
		UPowerup::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		IPowerup* Power = Cast<IPowerup>(FoundActors[i]);
		if (Power->Player == FromPlayer)
		{
			Power->Execute_DeActivate(Power->_getUObject());
		}
	}

}

void AGP4_Powerup::UpdateBlackholeList(AGP4_Blackhole* RemoveItem)
{
	if (Blackholes.Contains(RemoveItem))
	{
		Blackholes.Remove(RemoveItem);
	}
	if (Blackholes.Num() > 0)
	{
		bool NoFirsthole = false;
		for (int32 i = 0; i < Blackholes.Num(); i++)
		{
			if (Blackholes[i]->bFirstBlackhole)
			{
				return;
			}
		}
		Blackholes[0]->bFirstBlackhole = true;
	}
}
