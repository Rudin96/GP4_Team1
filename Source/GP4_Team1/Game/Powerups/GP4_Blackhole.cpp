// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_Blackhole.h"
#include "GP4_Powerup.h"
#include "Components/SphereComponent.h"
#include "GP4_Team1/Player/GP4_PlayerCharacter.h"

// Sets default values
AGP4_Blackhole::AGP4_Blackhole(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootCmp = OI.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	SetRootComponent(RootCmp);
	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Sphere"));
	TriggerZone->SetSphereRadius(Radius);
	TriggerZone->SetCollisionProfileName(TEXT("Trigger"));
	TriggerZone->SetupAttachment(RootCmp);
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AGP4_Blackhole::OnOverlapBegin);
	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AGP4_Blackhole::OnOverlapEnd);

	BlackHoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Black Hole"));
	//FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);
	//BlackHoleMesh->AttachToComponent(RootComponent, rules);
	
	BlackHoleMesh->SetupAttachment(RootCmp);
}

// Called when the game starts or when spawned
void AGP4_Blackhole::BeginPlay()
{
	Super::BeginPlay();
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);
	BlackHoleMesh->AttachToComponent(RootComponent, rules);
	SetActorLocation(FVector(0, 0, -100000)); 
	Powerup = GetFirstPowerupInstance();
	if (Powerup)
	{
		Powerup->Blackholes.Add(this);
	}
}

// Called every frame
void AGP4_Blackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Is something in its reach
	if (bIsHit)
	{
		for (AActor* CloseActor : HitActors)
		{
			// Ignore possible powerups nearby
			if (CloseActor && !CloseActor->IsA(AGP4_Powerup::StaticClass()))
			{
				// Move actors towards hole and make them smaller
				CloseActor->SetActorLocation(MoveActorCloser(CloseActor, DeltaTime));
				CloseActor->SetActorRelativeScale3D(
					FMath::Lerp(CloseActor->GetActorRelativeScale3D(),
					ScaleTarget,
					DeltaTime * ScaleSpeed));
			}
		}
	}

	if (!bActivated)
	{
		MoveCursor();
	}
	// Make sure only one blackhole calls this to avoid interference form the tick call list
	else if (bFirstBlackhole)
	{
		LookForOtherBlackholes();
	}
	
	if (Player && !bSetInput)
	{
		AGP4_PlayerCharacter* Char = Cast<AGP4_PlayerCharacter>(Player);
		Char->InputComponent->BindAction("ReleaseBlackhole", IE_Pressed, this, &AGP4_Blackhole::DropBlackHole);
		bSetInput = true;
	}
}

FVector AGP4_Blackhole::MoveActorCloser(AActor* Actor, float DeltaTime)
{
	return FMath::Lerp(Actor->GetActorLocation(), GetActorLocation(), DeltaTime * PullSpeed);
}

void AGP4_Blackhole::MoveCursor()
{
	FHitResult Hit;
	FVector Start = Player->GetActorLocation();
	FVector End = Start + (Player->GetActorForwardVector() * DropDistance);
	FCollisionQueryParams Params;
	// todo? change to make default method with start and end vectors only as params?
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (Hit.bBlockingHit)
	{
		bCanActivate = false;
		End = Hit.ImpactPoint;
	}
	else
	{
		bCanActivate = true;
	}
	DrawDebugSphere(GetWorld(), End, 5, 0, FColor::Silver, false, 0.0f);
}

void AGP4_Blackhole::Activate_Implementation()
{
	// TDOD Indicator the player obtain the black hole powerup!
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, 
		TEXT("Black hole powerup obtained, press C or L3 to activate"));

}

void AGP4_Blackhole::DeActivate_Implementation()
{
	Powerup->UpdateBlackholeList(this); 
	
	for (AActor* CloseActor : HitActors)
	{
		if (CloseActor)
		{
			// Ignore possible powerups nearby
			if (!CloseActor->IsA(AGP4_Powerup::StaticClass()))
			{
				if (CloseActor->IsA(AGP4_PlayerCharacter::StaticClass()) || CloseActor->IsA(APawn::StaticClass()))
				{
					CloseActor->SetActorRelativeScale3D(FVector(
						PlayerOrginialScale, PlayerOrginialScale, PlayerOrginialScale));
				}
				else
				{
					CloseActor->SetActorRelativeScale3D(FVector(1, 1, 1));

				}
			}
		}
	}
	Destroy();
}

void AGP4_Blackhole::DropBlackHole()
{
	if (!bActivated && bCanActivate)
	{
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);
		BlackHoleMesh->AttachToComponent(RootComponent, rules);
		FVector HolePosition = FVector(Player->GetActorLocation() + (Player->GetActorForwardVector() * DropDistance));
		SetActorLocation(HolePosition);

		DrawDebugSphere(GetWorld(), HolePosition, Radius, 0, FColor::Black, false, 1.0f);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AGP4_Blackhole::DeActivate_Implementation, Timer, false);

		bActivated = true; 
	}
}

void AGP4_Blackhole::LookForOtherBlackholes()
{
	//TArray<AActor*> Holes;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_Blackhole::StaticClass(), Holes);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald,
		FString::Printf(TEXT("nr of holes: %d"), Holes.Num()));*/
	FoundCloseHole = false;
	if (Powerup->Blackholes.Num() > 0)
	{
		AGP4_Blackhole* Black = Powerup->Blackholes[0];
		Black->bFirstBlackhole = true;
		for (int32 i = 1; i < Powerup->Blackholes.Num(); i++)
		{
			if (FVector::Distance(Powerup->Blackholes[i]->GetActorLocation(), GetActorLocation()) < Radius)
			{
				FoundCloseHole = true;
				IncreaseSize(Powerup->Blackholes[i]);
				return;
			}
		}
	}
}

void AGP4_Blackhole::DeActivateOtherBlackhole()
{
	if (Other)
	{
		Powerup->UpdateBlackholeList(Other);
		Other->Execute_DeActivate(Other->_getUObject());
	}
}

void AGP4_Blackhole::IncreaseSize(AGP4_Blackhole* OtherBlackhole)
{
	//Other = Cast<AGP4_Blackhole>(OtherBlackhole);
	OtherBlackhole->bFirstBlackhole = false;
	if (OtherBlackhole)
	{
		// Has the hole been activated?
		if (!OtherBlackhole->bActivated)
		{
			return;
		}
	}
	/*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black,
		FString::Printf(TEXT("Black hole hungry!")));*/
	// Call this once (for every black hole in radius)
	if (MeshScale < 4 && FoundCloseHole)
	{
		Powerup->Blackholes.Remove(Other);
		MeshScale += 0.2f;
		BlackHoleMesh->SetRelativeScale3D(FVector(MeshScale, MeshScale, MeshScale));
		PullSpeed += 0.2f;
		ScaleTarget *= 0.99f;
		Radius += 25;
		FoundCloseHole = false;
	}
	if (OtherBlackhole)
	{
		HitActors.Add(OtherBlackhole);
		Other = OtherBlackhole;
		// A timer to wait for the other black hole to move to the 
		// center before it gets deactivated
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, 
			&AGP4_Blackhole::DeActivateOtherBlackhole, 0.4f, false);

	}
}

void AGP4_Blackhole::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HitActors.Contains(OtherActor) &&
		!OtherActor->IsA(AGP4_Powerup::StaticClass()) && 
		!OtherActor->IsA(AGP4_Blackhole::StaticClass()))
	{
		bIsHit = true;
		HitActors.Add(OtherActor);

	}
}

void AGP4_Blackhole::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HitActors.Contains(OtherActor))
	{
		HitActors.Remove(OtherActor);
	}
}

AGP4_Powerup* AGP4_Blackhole::GetFirstPowerupInstance()
{
	TArray<AActor*> PUActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGP4_Powerup::StaticClass(), PUActors);
	if (PUActors.Num() > 0)
	{
		AGP4_Powerup* Temp = Cast<AGP4_Powerup>(PUActors[0]);
		if (Temp)
		{
			return Temp;
		}
	}
	return nullptr;
}
