// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damage/DamageSystem.h"
#include "CustomStructs/StructSet.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashStateChanged, bool, bDashState);
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
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDashStateChanged OnDashStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName MainMenuLevel;

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

	/** Collision testing For Picking up Items **/
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Setter Function for Can Do Action */
	UFUNCTION()
	FORCEINLINE void SetterCanDoAction(bool _InBool) { bCanDoAction = !_InBool; }//TODO: We are inverting the setter, need to change that

	/* Setter Function for Can Move */
	UFUNCTION()
	FORCEINLINE void SetterCanMove(bool _InBool) { bCanMove = _InBool; }


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

	/** Melee Component **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* MeleeComponent;

	/** Range Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URangedComponent* RangedComponent;

	/** Dash Timer Handles */
	FTimerHandle DashFrameTimerHandle;
	FTimerHandle DashEndTimerHandle;
	FTimerHandle DashCooldownTimerHandle;
	
	/** Execute Dash Delegate */
	FTimerDelegate ExecuteDashDelegate;


	/** Move Speed -> Saved at start of play so that we can reset to default */
	UPROPERTY()
	float MoveSpeed;

	/** Move Acceleration Speed */
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float AccelerationSpeed;
	
	/** Dash Distance
	 * Distance / Speed = Time
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"));
	float DashDistance = 1000.0f;

	/** Dash Speed
	 * Distance / Speed = Time
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"));
	float DashSpeed = 2000.0f;

	/** Dash Cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"));
	float DashCooldown = 1.0f;

	
	//////////////////* Flags *////////////////////

	/* Locking Movement */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanMove;

	/* Locking Actions */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanDoAction;

	/* Lock Dash */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanDash;

	/* Locking Pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		bool bCanPickup;

	/* Storing Weapon Mode Flag */ //TODO: EditAnywhere needs to be Changed to VisibleAnywhere. It is Edit so that we can easily Test
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flags", meta = (AllowPrivateAccess = "true"))
		EWeaponMode CurrentWeapon;

	//////////////////* Player States *////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	FModifierSet ModifierSet;

	FTimerHandle ItemEffectHandle;
	FTimerDelegate ItemEffectDelegate;

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

	UFUNCTION()
	void ImplementModifier(FModifierSet _InSet);

	UFUNCTION()
	void ClearModifier();

};