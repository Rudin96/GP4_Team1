// Fill out your copyright notice in the Description page of Project Settings.

#include "GP4_PlayerCharacter.h"
#include "GP4_Team1/Weapon/GP4_WeaponBase.h"
#include "GP4_Team1/Utils/GP4_Macros.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GP4_PlayerController.h"
#include "Curves/RichCurve.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GP4_Team1/Weapon/Projectile/GP4_GrappleProjectile.h>

// Sets default values
AGP4_PlayerCharacter::AGP4_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	/*GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;*/

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowColor = FColor::Red;
	Arrow->bHiddenInGame = false;
	Arrow->ArrowSize = 5.f;

	DebuffText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Debuff Text Component"));
	DebuffText->SetupAttachment(RootComponent);

	/*GetArrowComponent()->bHiddenInGame = false;
	GetArrowComponent()->ArrowSize = 5.f;
	GetArrowComponent()->ArrowColor = FColor::Red;*/
}

// Called when the game starts or when spawned
void AGP4_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	for(size_t i = 0; i < CharacterStats->WeaponClasses.Num(); i++)
		EquippedWeapons.Add(NewObject<UGP4_WeaponBase>(this, CharacterStats->WeaponClasses[i]));
	Health = CharacterStats->StartingHealth;
	if (AssignedTeam == ETeam::TEAM_BLUE)
		Arrow->ArrowColor = FColor::Blue;
	DashOverrideCurve = CharacterStats->DefaultDashCurve;
	GetCharacterMovement()->MaxWalkSpeed = CharacterStats->MovementSpeed;
}

// Called every frame
void AGP4_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (UGP4_WeaponBase* Weapon : EquippedWeapons)
		Weapon->TickWeapon(DeltaTime);

	if (bWantsToDash)
		PerformDash(DashOverrideCurve);

	//RemainingStunDuration = GetWorld()->GetTimerManager().GetTimerRemaining(StunTimerHandle, &AGP4_PlayerCharacter::Stun);
}

float AGP4_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;
	LOG(3.f, FColor::Red, FString::SanitizeFloat(Health));
	if (Health <= 0)
	{
		Kill(EventInstigator, DamageCauser);
	}
	return Health - DamageAmount;
}

void AGP4_PlayerCharacter::Kill(AController* KillerController, AActor* KillerActor)
{
	OnDeath();
	AGP4_Team1GameModeBase* GameMode = Cast<AGP4_Team1GameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->StartRespawnPlayer(this);

	Health = CharacterStats->StartingHealth;

	if (KillerController == nullptr || KillerActor == nullptr)
	{
		for (AGP4_PlayerCharacter* Character : GameMode->Players)
		{
			if (Character != nullptr && Character != this)
			{
				GameMode->AddScore(Cast<AGP4_PlayerController>(Character->GetController())->NetPlayerIndex, 1);
				Cast<AGP4_PlayerController>(Character->GetController())->ClientPlayForceFeedback(KillForceFeedBack);
			}
		}
	}

	for (UGP4_WeaponBase* Weapon : EquippedWeapons)
	{
		Weapon->ResetWeapon();
	}
		

	if (AGP4_PlayerController* InstigatorController = Cast<AGP4_PlayerController>(KillerController))
	{
		GameMode->AddScore(InstigatorController->NetPlayerIndex, 1);
		InstigatorController->ClientPlayForceFeedback(KillForceFeedBack);
	}
}

void AGP4_PlayerCharacter::Dash(FVector EndPos, UCurveFloat* Curve, AActor* DestActor, float NewDashDistanceTolerance, bool bStunOverride, bool bGrappleOverride)
{
	if ((bGrappleOverride) || !bIsDashing && !bIsStunned && bCanDash || (bStunOverride))
	{
		if (NewDashDistanceTolerance > 0.f)
		{
			DashDistToleranceOverride = NewDashDistanceTolerance;
		}
		if (Curve != nullptr)
			SetDashCurve(Curve);
		DashEnd = EndPos;
		SetActorLocation(GetActorLocation() + FVector::UpVector * CharacterStats->DashZOffset);
		DashEnd.Z = GetActorLocation().Z + CharacterStats->DashZOffset;
		DashToActor = DestActor;
		bWantsToDash = true;
	}
}

void AGP4_PlayerCharacter::Grapple(FVector EndPos, UCurveFloat* Curve, AActor* DestActor, float DashDistanceTolerance)
{
	//Stun(CharacterStats->DefaultStunDuration);
	Dash(EndPos, Curve, DestActor, DashDistanceTolerance, true, true);
}

void AGP4_PlayerCharacter::PerformDash(UCurveFloat* Curve)
{
	FHitResult HitRes;
	/*if (Curve == nullptr)
		SetDashCurve(Curve);*/

	int CurrentDashDistTolerance = DashDistToleranceOverride > 0.f ? DashDistToleranceOverride : CharacterStats->DashDistanceTolerance;

	bIsDashing = true;

	if (DashToActor != nullptr)
		DashEnd = DashToActor->GetActorLocation();

	CurrentDashTime += GetWorld()->GetDeltaSeconds();
	FString status = bIsDashing ? TEXT("True") : TEXT("False");
	SetActorLocation(FMath::Lerp(GetActorLocation(), DashEnd, Curve->GetFloatValue(CurrentDashTime)), true, &HitRes, ETeleportType::TeleportPhysics);
	FRichCurveKey LastKey = Curve->FloatCurve.GetLastKey();
	int Iterations = 0;
	if (HitRes.bBlockingHit)
	{
		/*while (HitRes.bBlockingHit && ++Iterations < 10)
		{
			AddActorWorldOffset(FVector::UpVector * 2.f);
			LOG(2.f, FColor::Blue, TEXT("While hit dash offset!"));
		}*/

		if (HitRes.GetComponent() != nullptr && HitRes.GetComponent()->IsSimulatingPhysics())
		{
			HitRes.GetComponent()->AddImpulse(HitRes.ImpactNormal * CharacterStats->DashIntoPhysicsStrength, NAME_None, true);
		}

		if (CheckIfHitWall(CharacterStats->DashWallAngleTolerance, HitRes))
		{
			bWantsToDash = false;
			bIsDashing = false;
			bCanDash = false;
			CurrentDashTime = 0.f;
			if (bStunnedWhenHittingWall)
			{
				Stun(CharacterStats->DefaultStunDuration);
				TakeDamage(1, FDamageEvent(), GetInstigatorController(), this);
				bCanDash = false;
				bStunnedWhenHittingWall = false;
			}

			GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &AGP4_PlayerCharacter::RechargeDash, CharacterStats->DashCooldown, false, -1.f);
			LOG(2.f, FColor::Green, TEXT("Hit a wall, no more dash"))
			DashDistToleranceOverride = 0.f;
			SetDashCurve(CharacterStats->DefaultDashCurve);
			return;
		}
		/*if (HitRes.Normal.Z < .3f)
		{
			bWantsToDash = false;
			bIsDashing = false;
			CurrentDashTime = 0.f;
			LOG(2.f, FColor::Green, TEXT("Blocking Hit, no more dash"))
			DashDistToleranceOverride = 0.f;
			return;
		}*/
	}

	//LOG(1.f, FColor::Green, TEXT("Dashing/Grappled"))
	float CurrentDistToEnd = (DashEnd - GetActorLocation()).Length();

	if (CurrentDashTime >= LastKey.Time || CurrentDistToEnd < CurrentDashDistTolerance)
	{
		bWantsToDash = false;
		bIsDashing = false;
		bCanDash = false;
		UnStun();
		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &AGP4_PlayerCharacter::RechargeDash, CharacterStats->DashCooldown, false, -1.f);
		CurrentDashTime = 0.f;
		LOG(3.f, FColor::Red, TEXT("No more dashtime, stop dashing"))
		DashDistToleranceOverride = 0.f;
		SetDashCurve(CharacterStats->DefaultDashCurve);
		return;
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Magenta, status);
}

void AGP4_PlayerCharacter::SetDashCurve(UCurveFloat* NewCurve)
{
	CharacterStats->DefaultDashCurve = NewCurve;
}

void AGP4_PlayerCharacter::SetStunnedIntoWall(bool bNewStun)
{
	bStunnedWhenHittingWall = bNewStun;
}

void AGP4_PlayerCharacter::Stun(float Duration)
{
	bIsStunned = true;
	DebuffText->SetText(FText::FromString(TEXT("STUNNED")));
	GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &AGP4_PlayerCharacter::UnStun, Duration, false, Duration);
	OnStunned();
}

bool AGP4_PlayerCharacter::CheckIfHitWall(float WallAngle, const FHitResult& HitRes)
{
	float ImpactDot = FVector::DotProduct(FVector::UpVector, HitRes.ImpactNormal);
	float AngleRadians = FMath::Acos(ImpactDot);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	if (AngleDegrees > WallAngle)
	{
		LOG(0.f, FColor::Green, HitRes.ImpactNormal.ToString())
		LOG(0.f, FColor::Cyan, FString::SanitizeFloat(AngleDegrees))
		return true;
	}
	else
		return false;
}

float AGP4_PlayerCharacter::GetStunTimerRemaining()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(StunTimerHandle);
}

void AGP4_PlayerCharacter::LaunchCharacter(FVector LaunchVelocity, bool bOverrideXY, bool bOverrideZ) 
{
	if (bIsDashing)
	{
		bWantsToDash = false;
		bIsDashing = false;
	}

	Super::LaunchCharacter(LaunchVelocity, bOverrideXY, bOverrideZ);
}

void AGP4_PlayerCharacter::UnStun()
{
	bIsStunned = false;
	GetWorld()->GetTimerManager().ClearTimer(StunTimerHandle);
	DebuffText->SetText(FText::FromString(TEXT("")));
	OnUnStunned();
}

void AGP4_PlayerCharacter::FirePrimaryWeapon()
{
	EquippedWeapons[0]->Fire();
}

void AGP4_PlayerCharacter::FireSecondaryWeapon()
{
	EquippedWeapons[0]->SecondaryFire();
}

void AGP4_PlayerCharacter::RechargeDash()
{
	bCanDash = true;
}

void AGP4_PlayerCharacter::ResetPrimaryWeapon()
{
	EquippedWeapons[0]->ResetWeapon();
}
