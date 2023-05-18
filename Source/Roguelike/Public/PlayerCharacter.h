// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damage/DamageSystem.h"
#include "CustomStructs/StructSet.h"
#include "PlayerCharacter.generated.h"

/*Enum Class to Track the Current Weapon Type that is Equipped*/
UENUM(Blueprintable, BlueprintType)
enum class EWeaponMode : uint8
{
	Melee			UMETA(DisplayName = "Melee"),
	Ranged			UMETA(DisplayName = "Ranged")
};

UCLASS()
class ROGUELIKE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();
	virtual ~APlayerCharacter() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game ends or when destroyed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns Current Weapon **/
	FORCEINLINE EWeaponMode GetEquippedWeapon() const { return CurrentWeapon; }
	/** Sets Current Weapon **/
	FORCEINLINE void SetEquippedWeapon(EWeaponMode _InWeapon) { CurrentWeapon = _InWeapon; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Skeletal Mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMesh;

	/** Hit Box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitBox, meta = (AllowPrivateAccess = "true"))
		class UHurtBox* HurtBox;

	/** Dash Timer Handle */
	FTimerHandle DashTimerHandle;

	/** Move Speed -> Saved at start of play so that we can reset to default */
	UPROPERTY()
		float MoveSpeed;

	/** Move Acceleration Speed */
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float AccelerationSpeed;

	/**
	 * Distance / Speed = Time
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"));
	float DashDistance = 1000.0f;

	/**
	 * Distance / Speed = Time
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"));
	float DashSpeed = 2000.0f;

	//////////////////* Flags *////////////////////

	/* Locking Movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanMove;

	/* Locking Actions */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanDoAction;

	/* Storing Weapon Mode Flag */ //TODO: EditAnywhere needs to be Changed to VisibleAnywhere. It is Edit so that we can easily Test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		EWeaponMode CurrentWeapon;

	//////////////////* Player States *////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	float maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	FModifierSet ModifierSet;
public:
	/** Move Function */
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveForward(float _InValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveRight(float _InValue);

	/** Start Dash Ability */
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void Dash();

	/** Terminate Dash Ability */
	UFUNCTION(BlueprintCallable, Category = "Dash")
		void EndDash();

	/** Interact */
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
		void Pause();

	/** Weapon Ability **/
	UFUNCTION(BlueprintCallable, Category = "Attacks")
		void WeaponAbility();

	/** adds both Negative and Positive Health While Clamping it to the Min and Max**/
	void AddHealth(float _InHealth);


private:
	/** Dash Function -> Recursively calling itself until dash is finished */
	UFUNCTION()
		void ExecuteDash(FVector TargetDestination);

	/** Function to Receive Damage **/
	UFUNCTION()
		void ReceiveDamage(float _InDamage/*, EEffectType _EffectType*/);

	/** Central Function to handle Player Death**/
	UFUNCTION()
		void PlayerDeath();

	void ImplementModifier(FModifierSet _InSet);
};