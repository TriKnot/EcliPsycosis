// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPlayerController.generated.h"

UENUM(BlueprintType, Blueprintable)
enum class ECustomInputMode : uint8
{
	KeyboardMouse		UMETA(DisplayName = "Keyboard and Mouse"),
	Gamepad				UMETA(DisplayName = "Gamepad")
};

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPlayerController();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/*Overriding Function That Gets Called When the Controller possesses the Player*/
	virtual void BeginPlayingState() override;

	/*Overriding Input Bindings*/\
	virtual void SetupInputComponent() override;

	/*Function to Bind on the MoveForward Input*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float _InVal);

	/*Function to Bind on the MoveRight Input*/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRight(float _InVal);

	/*Function to Bind on the Dash Input*/
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void Dash();

	/*Function to Bind on the Interact Input*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/** Light Attack **/
	UFUNCTION(BlueprintCallable, Category = "Attacks")
		void LightAttack();

	/** Heavy Attack **/
	UFUNCTION(BlueprintCallable, Category = "Attacks")
		void HeavyAttack();

	/** Pause **/
	UFUNCTION(BlueprintCallable, Category = "Game System")
		void PlayerPause();

	/** Weapon Ability **/
	UFUNCTION(BlueprintCallable, Category = "Attacks")
		void WeaponAbility();

private:
	APlayerCharacter* CurrentPawn;
};
