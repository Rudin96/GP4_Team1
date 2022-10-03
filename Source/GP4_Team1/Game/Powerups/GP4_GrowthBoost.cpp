// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_GrowthBoost.h"
#include "GP4_Powerup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP4_Team1/Utils/GP4_Macros.h"
#include "GP4_Team1/Weapon/Utils/GP4_GrappleHookDataAsset.h"
#include "GP4_Team1/Weapon/GP4_GrapplingHook.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"

// Sets default values, use OI thingy for scale and component location issues
AGP4_GrowthBoost::AGP4_GrowthBoost(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootCmp = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	SetRootComponent(RootCmp);
	ShieldZone = CreateDefaultSubobject<USphereComponent>(TEXT("Shield Sphere"));
	ShieldZone->SetSphereRadius(Radius);
	ShieldZone->SetupAttachment(RootCmp);
	
	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerZone->SetSphereRadius(Radius + TriggerOffset);
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
	TriggerZone->SetupAttachment(RootCmp);
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AGP4_GrowthBoost::OnOverlapBegin);
	
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));
	ShieldMesh->SetupAttachment(RootCmp);
}

// Called when the game starts or when spawned
void AGP4_GrowthBoost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGP4_GrowthBoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGP4_GrowthBoost::WaitForPlayerInstance()
{
	if (Player)
	{
		PlayerChar = Cast<AGP4_PlayerCharacter>(Player);

		if (!PlayerChar)
		{
			LOG(50.0f, FColor::Red, TEXT("Error: Growth powerup cant find its player, have you seen it? "));
			return;
		}
		// Set timer for this powerups duration
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this,
			&AGP4_GrowthBoost::DeActivate_Implementation, Timer, false);
		// Attachments
		PlayerMesh = Cast<UStaticMeshComponent>(
			PlayerChar->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		PlayerMesh->SetRelativeScale3D(FVector(GrowthScale, GrowthScale, GrowthScale));
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
		AttachToActor(PlayerChar, rules);
		SetActorRelativeLocation(FVector(0, 0, 0));
		// find and set values for the grapple hook
		for (size_t i = 0; i < PlayerChar->EquippedWeapons.Num(); i++)
		{
			if (Cast<UGP4_GrapplingHook>(PlayerChar->EquippedWeapons[i]))
			{
				GrapplingHook = Cast<UGP4_GrapplingHook>(PlayerChar->EquippedWeapons[i]);
				break;
			}

		}
		if (GrapplingHook)
		{
			GrapplingHook->GrappleStats->ProjectileSpeed = 6000;
			GrapplingHook->GrappleStats->FlingStrength = 3000;
			GrapplingHook->GrappleStats->ActivateDistance = 6000;

			GrapplingHook->GrappleStats->GrappleStopDistanceToObject = 2500;
			GrapplingHook->GrappleStats->GrappleStopDistanceToPlayer = 2500;
		}

	}
	else
	{
		// Calls Activate_Implementation()
		Execute_Activate(this);
	}

}

void AGP4_GrowthBoost::Activate_Implementation()
{
	FTimerHandle WaitAFewFramesHandle;
	GetWorldTimerManager().SetTimer(WaitAFewFramesHandle, this,
		&AGP4_GrowthBoost::WaitForPlayerInstance, 0.2f, false);

}

void AGP4_GrowthBoost::DeActivate_Implementation()
{
	// reset values before destroying this object
	if (PlayerMesh)
	{
		PlayerMesh->SetRelativeScale3D(FVector(1.6f, 1.6f, 1.6f));
	}
	if (Player)
	{
		FDetachmentTransformRules rules(EDetachmentRule::KeepRelative,
			EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false);
		DetachFromActor(rules);
	}
	if (GrapplingHook)
	{
		GrapplingHook->GrappleStats->ProjectileSpeed = OriginalSpeed;
		GrapplingHook->GrappleStats->FlingStrength = OriginalStrength;
		GrapplingHook->GrappleStats->ActivateDistance = OriginalActivateDistance;

		GrapplingHook->GrappleStats->GrappleStopDistanceToObject = OriginalDistanceObject;
		GrapplingHook->GrappleStats->GrappleStopDistanceToPlayer = OriginalDistancePlayer;
	}
	Destroy();
}

void AGP4_GrowthBoost::DamageShield()
{
	if (HitsShieldCanTake > 0)
	{
		MeshScale *= 0.9f;
		ShieldMesh->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));
		HitsShieldCanTake--;
	}
	if (HitsShieldCanTake <= 0)
	{
		Execute_DeActivate(this);
	}
}

void AGP4_GrowthBoost::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Quick ugly solution that works in this game, just deal with it, ok?
	// Saves the player from some hits from object with static meshes
	if (OtherComp->GetFName().ToString().Equals("StaticMesh"))
	{
		DamageShield();
		ShieldMesh->AddRadialImpulse(GetActorLocation(), PushRadius, PushForce, ERadialImpulseFalloff::RIF_Constant, true);
		
	}
}

