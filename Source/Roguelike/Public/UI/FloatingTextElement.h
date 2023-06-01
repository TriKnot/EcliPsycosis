// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingTextElement.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UFloatingTextElement : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initialization */
	UFUNCTION(BlueprintCallable, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	void Init(float InDamage, FColor InColor, FVector InWorldPosition);

	/** Calculate FinalScreenPosition and set it */
	UFUNCTION(BlueprintCallable, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	void SetFinalScreenPosition(FVector2D RandomXRange, FVector2D RandomYRange);

private:
	/** Calculate InitialScreenPosition and set it */
	void SetInitialScreenPosition( FVector WorldPosition );

	/** Set Text with damage value */
	void SetText(float InDamage) const;

public:
	/** Text to display */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* DamageText;

	/** Animation to play */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageTextAnim;

	/** Starting position for text before any animation */
	UPROPERTY(BlueprintReadWrite, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	FVector2D InitialScreenPosition;

	/** Final position for text to end up in after animation */
	UPROPERTY(BlueprintReadWrite, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	FVector2D FinalScreenPosition;

	/** Damage Number to display in widget */
	UPROPERTY(BlueprintReadWrite, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	float DamageToDisplay;

	/** Color of text displayed */
	UPROPERTY(BlueprintReadWrite, Category = "Floating Text", meta = (AllowPrivateAccess = "true"))
	FColor TextColour;
	
};


