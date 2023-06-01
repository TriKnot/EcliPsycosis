// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FloatingTextElement.h"

#include "Components/TextBlock.h"

void UFloatingTextElement::Init(float InDamage, FColor InColor, FVector InWorldPosition)
{
	SetInitialScreenPosition(InWorldPosition);
	SetPositionInViewport( InitialScreenPosition );
	SetText(InDamage);
	TextColour = InColor;
}

void UFloatingTextElement::SetInitialScreenPosition( FVector WorldPosition )
{
	// Get Player Controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// Project to Screen Space
	PlayerController->ProjectWorldLocationToScreen(WorldPosition, InitialScreenPosition);
}

void UFloatingTextElement::SetFinalScreenPosition( FVector2D RandomXRange, FVector2D RandomYRange )
{
	const float RandomX = FMath::FRandRange(RandomXRange.X, RandomXRange.Y);
	const float RandomY = FMath::FRandRange(RandomYRange.X, RandomYRange.Y);
	FinalScreenPosition = FVector2D(InitialScreenPosition.X + RandomX, InitialScreenPosition.Y + RandomY);
}

void UFloatingTextElement::SetText(float InDamage) const
{
	// Round to nearest int
	const int32 Damage = FMath::RoundToInt(InDamage);
	// Convert to string
	const FString DamageString = FString::FromInt(Damage);
	// Set Text
	DamageText->SetText(FText::FromString(DamageString));
}


