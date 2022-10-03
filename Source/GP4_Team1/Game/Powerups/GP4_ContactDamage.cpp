// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_ContactDamage.h"
#include "GP4_Powerup.h"
#include "Components/SphereComponent.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"
#include "GP4_Team1/Utils/GP4_Macros.h"
#include "GameFramework/Controller.h"
#include "GP4_Team1/Game/GP4_RespawnShield.h"

// Sets default values
AGP4_ContactDamage::AGP4_ContactDamage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerZone->SetSphereRadius(Radius);
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
	TriggerZone->SetupAttachment(RootComponent);
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AGP4_ContactDamage::OnOverlapBegin);

}


// Called when the game starts or when spawned
void AGP4_ContactDamage::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this,
		&AGP4_ContactDamage::WaitForPlayerInstance, 0.2f, false);

}

void AGP4_ContactDamage::WaitForPlayerInstance()
{
	if (Player && !bActive)
	{
		bActive = true;
		SetupRelativeScaleValues();
	}
}

void AGP4_ContactDamage::SetupRelativeScaleValues()
{
	PlayerScale = Player->GetActorScale3D();
	PlayerMesh = Cast<UStaticMeshComponent>(
		Player->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	MeshScale = PlayerMesh->GetRelativeScale3D();
	// If player has shrink powerup activated
	if (PlayerScale.Z < 1.2)
	{
		SetActorRelativeScale3D(FVector(1.7f, 1.7f, 1.0f));
		return;
	}
	// Check if player has growth powerup activated
	if (MeshScale.Z > 2)
	{
		SetActorRelativeScale3D(FVector(1.6f, 1.6f, 1.6f));
	}
}


// Called every frame
void AGP4_ContactDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bActive)
	{
		// Check if other powerups gets added to the as a safeplay remove this one because of scale problems
		if (PlayerScale.Z < Player->GetActorScale3D().Z || MeshScale.Z > PlayerMesh->GetRelativeScale3D().Z)
		{
			Execute_DeActivate(this->_getUObject());
		}
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_RespawnShield::StaticClass(), Actors);
		if (Actors.Num() > 0)
		{
			bHasOtherPlayerShield = true;
		}
		else
		{
			bHasOtherPlayerShield = false;
		}
		
	}
}

void AGP4_ContactDamage::Activate_Implementation()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, 
		&AGP4_ContactDamage::DeActivate_Implementation, Timer, false);

	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, false);
	AttachToActor(Player, rules);
	SetActorRelativeLocation(FVector(0, 0, -56));
}

void AGP4_ContactDamage::DeActivate_Implementation()
{
	FDetachmentTransformRules rules(EDetachmentRule::KeepRelative, 
		EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, false);
	DetachFromActor(rules);
	Destroy();
}

void AGP4_ContactDamage::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActive && !bHasOtherPlayerShield && OtherActor->IsA(AGP4_PlayerCharacter::StaticClass()) && OtherActor != Player)
	{
		// Do damage
		AGP4_PlayerCharacter* PC = Cast<AGP4_PlayerCharacter>(OtherActor);
		FRadialDamageEvent DamageEvent;
		DamageEvent.Origin = GetActorLocation();
		DamageEvent.Params = FRadialDamageParams(Damage, Radius);
		AController* ThisController = Player->GetInstigatorController();
		if (ThisController != nullptr)
		{
			PC->TakeDamage(Damage, DamageEvent, ThisController, Player);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald,
				OtherActor->GetFName().ToString());
		}
		// push the other player
		if (bPushing)
		{
			OtherActor->SetActorLocation(
				OtherActor->GetActorLocation() += Player->GetActorForwardVector() * 300);
		}

	}
}


