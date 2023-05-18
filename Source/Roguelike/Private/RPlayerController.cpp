// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerController.h"
#include "PlayerCharacter.h"

ARPlayerController::ARPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARPlayerController::BeginPlayingState()
{
	CurrentPawn = Cast<APlayerCharacter>(GetPawn());
}

void ARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	/*Bind axis mappings*/
	InputComponent->BindAxis("MoveForward", this, &ARPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARPlayerController::MoveRight);
	/*Bind Action Mappings*/
	InputComponent->BindAction("Dash", IE_Pressed, this, &ARPlayerController::Dash);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ARPlayerController::Interact);
	InputComponent->BindAction("LightAttack", IE_Pressed, this, &ARPlayerController::LightAttack);
	InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ARPlayerController::HeavyAttack);
	InputComponent->BindAction("WeaponAbility", IE_Pressed, this, &ARPlayerController::WeaponAbility);
	InputComponent->BindAction("Pause", IE_Pressed, this, &ARPlayerController::PlayerPause);
}

void ARPlayerController::MoveForward(float _InVal)
{
	if (CurrentPawn)
		CurrentPawn->MoveForward(_InVal);
}

void ARPlayerController::MoveRight(float _InVal)
{
	if (CurrentPawn)
		CurrentPawn->MoveRight(_InVal);
}

void ARPlayerController::Dash()
{
	if (CurrentPawn)
		CurrentPawn->Dash();
}


void ARPlayerController::Interact()
{
	if (CurrentPawn)
		CurrentPawn->Interact();
}

void ARPlayerController::LightAttack()
{
	if (CurrentPawn)
		CurrentPawn->LightAttack();
}

void ARPlayerController::HeavyAttack()
{
	if (CurrentPawn)
		CurrentPawn->HeavyAttack();
}

void ARPlayerController::PlayerPause()
{
	UE_LOG(LogTemp, Warning, TEXT("Paused"));
}

void ARPlayerController::WeaponAbility()
{
	if (CurrentPawn)
		CurrentPawn->WeaponAbility();
}