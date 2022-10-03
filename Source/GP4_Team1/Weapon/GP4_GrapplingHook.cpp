// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_GrapplingHook.h"
#include "CableActor.h"
#include "CableComponent.h"
#include "GP4_Team1/Utils/GP4_Macros.h"
#include "Utils/GP4_GrappleHookDataAsset.h"
#include "Components/SphereComponent.h"
#include "Projectile/GP4_FlingableProjectile.h"
#include "GP4_Team1/Player/GP4_PlayerController.h"

void UGP4_GrapplingHook::Fire_Implementation()
{
	FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * GrappleStats->ProjectileSpawnOffset;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();

	if (ShotCount < GrappleStats->MaxShotCount && bCanFire)
	{
		AActor* NewProjectile = nullptr;

		if (GrappleStats->ProjectileClassRed == nullptr || GrappleStats->ProjectileClassBlue == nullptr)
			return;

		if(GetOwner()->AssignedTeam == ETeam::TEAM_RED)
			NewProjectile = GetWorld()->SpawnActor<AActor>(GrappleStats->ProjectileClassRed, FTransform(SpawnRotation, SpawnLocation, FVector(GrappleStats->ProjectileScale)));
		if (GetOwner()->AssignedTeam == ETeam::TEAM_BLUE)
			NewProjectile = GetWorld()->SpawnActor<AActor>(GrappleStats->ProjectileClassBlue, FTransform(SpawnRotation, SpawnLocation, FVector(GrappleStats->ProjectileScale)));
		NewProjectile->OnActorHit.AddDynamic(this, &UGP4_GrapplingHook::ProjectileHit);
		NewProjectile->FindComponentByClass<USphereComponent>()->AddImpulse(GetOwner()->GetActorForwardVector() * GrappleStats->ProjectileSpeed, NAME_None, true);
		if(ShotCount == 0)
			SpawnCable(GetOwner(), NewProjectile);
		if (ShotCount == 1)
		{
			Ropes[0]->AttachToActor(NewProjectile, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), NAME_None);
		}
		Projectiles.Add(NewProjectile);
		ShotCount++;
		OnFire();
		bCanFire = false;
	}
}

void UGP4_GrapplingHook::SecondaryFire_Implementation()
{
	if (ShotCount > 0 && HitActors.Num() > 0 && bIsSecondaryFireReady)
	{
		FVector MiddlePos;
		for (AActor* Proj : Projectiles)
		{
			MiddlePos += Proj->GetActorLocation();
			DEBUG_SPHERE(Proj->GetActorLocation(), 100.f, FColor::Green, 3.4f)
		}
		MiddlePos = MiddlePos / HitActors.Num();
		DEBUG_SPHERE(MiddlePos, 100.f, FColor::Red, 3.f)

		for (size_t i = 0; i < HitActors.Num(); i++)
		{
			int otherActorIndex = Projectiles.Num() == 1 ? 0 : (i + 1) % 2;
			UMeshComponent* Comp = HitActors[i]->FindComponentByClass<UMeshComponent>();
			FVector FlingDir = HitActors.Num() > 1 ? Projectiles[otherActorIndex]->GetActorLocation() - MiddlePos : GetOwner()->GetActorLocation() - Projectiles[i]->GetActorLocation();
			FlingDir.Normalize();
			DEBUG_LINE(HitActors[i]->GetActorLocation(), HitActors[i]->GetActorLocation() + FlingDir * GrappleStats->FlingStrength, FColor::White, 4.f)

			if (Comp != nullptr && Comp->IsSimulatingPhysics())
			{
				DEBUG_SPHERE(MiddlePos, 100.f, FColor::Cyan, 3.f)
				Comp->AddImpulse(FlingDir * GrappleStats->FlingStrength, NAME_None, true);
				//HitActors[i]->OnActorHit.AddDynamic(this, &UGP4_GrapplingHook::FlingProjectileHit);
				//HitActors[i]->OnActorBeginOverlap.AddDynamic(this, &UGP4_GrapplingHook::FlingProjectileOverlap);
			}

			if (AGP4_PlayerCharacter* OtherCharacter = Cast<AGP4_PlayerCharacter>(HitActors[i]))
			{
				LOG(10.f, FColor::Yellow, FString::Printf(TEXT("Other actor index: %i"), otherActorIndex))
				OtherCharacter->Stun(GrappleStats->StunDuration);
				AActor* DestinationActor = Projectiles.Num() == 1 ? GetOwner() : nullptr;
				OtherCharacter->Grapple(Projectiles[otherActorIndex]->GetActorLocation(), GrappleStats->GrappleCurveObjectPlayer, DestinationActor, GrappleStats->GrappleStopDistanceToObject);
				DEBUG_SPHERE(Projectiles[otherActorIndex]->GetActorLocation(), 30, FColor::Black, 3.f)
			}

			if (Comp != nullptr && !Comp->IsSimulatingPhysics())
			{
				if (Projectiles.Num() == 1)
				{
					GetOwner()->Grapple(Projectiles[i]->GetActorLocation(), nullptr, nullptr);
					break;
				}
			}
		}
		OnSecondaryFire();
		StartResetWeaponTimer(GrappleStats->ResetWeaponCooldown);
		return;
	}
}


void UGP4_GrapplingHook::TickWeapon_Implementation(float DeltaSeconds)
{
	if (Projectiles.IsEmpty())
		return;

	for (AActor* Proj : Projectiles)
	{
		FVector PlayerProjectileDistance = Proj->GetActorLocation() - GetOwner()->GetActorLocation();
		float ProjectileDistance = PlayerProjectileDistance.Length();
		if (ProjectileDistance > GrappleStats->ActivateDistance && Proj->GetAttachParentActor() != nullptr)
		{
			ResetWeapon();
			break;
		}
	}
}

void UGP4_GrapplingHook::ResetWeapon()
{
	Super::ResetWeapon();

	for (AActor* Proj : Projectiles)
	{
		Proj->Destroy();
	}
	for (ACableActor* Rope : Ropes)
	{
		Rope->Destroy();
	}
	for (AActor* Actor : HitActors)
	{
		Actor->OnActorHit.RemoveAll(this);
	}
	Projectiles.Empty();
	HitActors.Empty();
	Ropes.Empty();
	ShotCount = 0;
	bCanFire = true;
	bIsSecondaryFireReady = true;
	OnResetWeapon();
}

void UGP4_GrapplingHook::SpawnCable(AActor* AttachActor1, AActor* AttachActor2)
{
	FAttachmentTransformRules CableRules(EAttachmentRule::KeepWorld, true);
	/*if (HitActors.Num() == ShotCount)
		return;*/
	
	ACableActor* Cable = nullptr;

	if (GrappleStats->SlimeStringClassRed == nullptr || GrappleStats->SlimeStringClassBlue == nullptr)
		return;

	if (GetOwner()->AssignedTeam == ETeam::TEAM_RED)
		Cable = GetWorld()->SpawnActor<ACableActor>(GrappleStats->SlimeStringClassRed, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	if (GetOwner()->AssignedTeam == ETeam::TEAM_BLUE)
		Cable = GetWorld()->SpawnActor<ACableActor>(GrappleStats->SlimeStringClassBlue, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	Cable->CableComponent->CableWidth = GrappleStats->SlimeStringThiccness;
	Cable->AttachToActor(AttachActor1, CableRules);
	Cable->CableComponent->bEnableCollision = false;
	Cable->CableComponent->SetAttachEndTo(AttachActor2, AttachActor2->GetComponentByClass(UMeshComponent::StaticClass())->GetFName(), NAME_None);
	Ropes.Add(Cable);
}

void UGP4_GrapplingHook::StartResetWeaponTimer(float Duration)
{
	bCanFire = false;
	bIsSecondaryFireReady = false;
	for (AActor* Proj : Projectiles)
	{
		Proj->OnActorHit.RemoveAll(this);
	}
	GetWorld()->GetTimerManager().SetTimer(ResetWeaponHandle, this, &UGP4_GrapplingHook::ResetWeapon, Duration, false, Duration);
}

void UGP4_GrapplingHook::ProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	FAttachmentTransformRules ProjectileRules(EAttachmentRule::KeepWorld, true);
	if (OtherActor == GetOwner() || OtherActor == nullptr)
		return;

	if (AGP4_FlingableProjectile* FlingProj = Cast<AGP4_FlingableProjectile>(OtherActor))
	{
		FlingProj->SetCurrentOwner(GetOwner());
		FlingProj->SetCurrentOwnerController(Cast<AGP4_PlayerController>(GetOwner()->GetController()));
		HitActors.Add(FlingProj);
	}
	else {
		HitActors.Add(OtherActor);
	}
	UMeshComponent* PhysComp = OtherActor->FindComponentByClass<UMeshComponent>();
	Projectiles[ShotCount - 1]->AttachToActor(OtherActor, ProjectileRules);
	if (PhysComp != nullptr && !PhysComp->IsAnyRigidBodyAwake())
		PhysComp->PutAllRigidBodiesToSleep();

	SelfActor->SetActorEnableCollision(false);

	SelfActor->OnActorHit.RemoveAll(this);

	OnProjectileImpact();
	bCanFire = true;
	if (ShotCount == GrappleStats->MaxShotCount && GrappleStats->bShouldFireOnMaxShots)
	{
		SecondaryFire();
	}
}

void UGP4_GrapplingHook::FlingProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AGP4_PlayerCharacter>(OtherActor))
	{
		OtherActor->TakeDamage(1.f, FDamageEvent(), GetOwner()->GetController(), GetOwner());

		for (AActor* Actor : HitActors)
		{
			Actor->OnActorHit.RemoveAll(this);
		}
	}

}

void UGP4_GrapplingHook::FlingProjectileOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (AGP4_PlayerCharacter* Player = Cast<AGP4_PlayerCharacter>(OtherActor))
	{
		if (AGP4_FlingableProjectile* FlingProj = Cast<AGP4_FlingableProjectile>(OverlappedActor))
		{
			Player->TakeDamage(1.f, FDamageEvent(), FlingProj->GetCurrentOwnerController(), FlingProj->GetCurrentOwner());
		}
		FVector ImpulseDir = (OverlappedActor->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal2D() * 1000.f;
		Player->LaunchCharacter(ImpulseDir + FVector::UpVector * 500.f, true, false);
		for (AActor* Actor : HitActors)
		{
			Actor->OnActorBeginOverlap.RemoveAll(this);
		}
	}
}
