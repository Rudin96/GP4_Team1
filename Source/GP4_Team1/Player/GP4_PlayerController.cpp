// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_PlayerController.h"

void AGP4_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AGP4_PlayerController::HandleMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGP4_PlayerController::HandleMoveRight);
	InputComponent->BindAxis("LookUp", this, &AGP4_PlayerController::HandleLookUp);
	InputComponent->BindAxis("LookRight", this, &AGP4_PlayerController::HandleLookRight);

	InputComponent->BindAction("Primary Action", IE_Pressed, this, &AGP4_PlayerController::HandlePrimaryAction);
	InputComponent->BindAction("Secondary Action", IE_Pressed, this, &AGP4_PlayerController::HandleSecondaryAction);
	InputComponent->BindAction("Third Action", IE_Pressed, this, &AGP4_PlayerController::HandleThirdAction);
	InputComponent->BindAction("Fourth Action", IE_Pressed, this, &AGP4_PlayerController::HandleFourthAction);

	InputComponent->BindAction("Reset Weapon1", IE_Pressed, this, &AGP4_PlayerController::HandleResetWeapon1);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AGP4_PlayerController::HandleJump);

}

void AGP4_PlayerController::HandleMoveForward(float Val)
{
	//TODO: Make Data asset to stuff all values in there
	if(GetOwnedCharacter() != nullptr && GetOwnedCharacter()->CharacterStats != nullptr && !GetOwnedCharacter()->bIsStunned)
		GetOwnedCharacter()->AddMovementInput(FVector::ForwardVector * GetOwnedCharacter()->CharacterStats->MovementSpeed * Val * GetWorld()->GetDeltaSeconds());

	InputDirection.X = Val;
}

void AGP4_PlayerController::HandleMoveRight(float Val)
{
	if (GetOwnedCharacter() != nullptr && GetOwnedCharacter()->CharacterStats != nullptr && !GetOwnedCharacter()->bIsStunned)
		GetOwnedCharacter()->AddMovementInput(FVector::RightVector * GetOwnedCharacter()->CharacterStats->MovementSpeed * Val * GetWorld()->GetDeltaSeconds());

	InputDirection.Y = Val;
}

void AGP4_PlayerController::HandleLookUp(float Val)
{
	InputRotationVector.X = Val;
	if (GetOwnedCharacter() == nullptr)
		return;

	if (FMath::Abs(Val) > 0.1f && !GetOwnedCharacter()->bIsStunned)
		GetPawn()->SetActorRotation(InputRotationVector.ToOrientationQuat());
}

void AGP4_PlayerController::HandleLookRight(float Val)
{
	InputRotationVector.Y = Val;
	if (GetOwnedCharacter() == nullptr)
		return;

	if(FMath::Abs(Val) > 0.1f && !GetOwnedCharacter()->bIsStunned)
		GetPawn()->SetActorRotation(InputRotationVector.ToOrientationQuat());
}

void AGP4_PlayerController::HandlePrimaryAction()
{
	//TODO: Make grapple base class and call fire here
	if (GetOwnedCharacter() == nullptr)
		return;

	if(!GetOwnedCharacter()->bIsStunned)
		GetOwnedCharacter()->FirePrimaryWeapon();
}

void AGP4_PlayerController::HandleSecondaryAction()
{
	// also here
	if (GetOwnedCharacter() == nullptr)
		return;

	if (!GetOwnedCharacter()->bIsStunned)
		GetOwnedCharacter()->FireSecondaryWeapon();
}

void AGP4_PlayerController::HandleThirdAction()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Implement third action here"));
	if (GetOwnedCharacter() == nullptr)
		return;
	GetOwnedCharacter()->Dash(GetOwnedCharacter()->GetActorLocation() + InputDirection * 1000.f, nullptr, nullptr);
}

void AGP4_PlayerController::HandleFourthAction()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Implement fourth action here"));
}

void AGP4_PlayerController::HandleResetWeapon1()
{
	if (GetOwnedCharacter() == nullptr)
		return;
	GetOwnedCharacter()->ResetPrimaryWeapon();
}

void AGP4_PlayerController::HandleJump()
{
	if (GetOwnedCharacter() == nullptr)
		return;
	GetOwnedCharacter()->Jump();
}