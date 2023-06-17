#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damage/DamageSystem.h"
#include "CustomStructs/StructSet.h"
#include "CustomStructs/EnumSet.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectTriggerEnter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS()
class ROGUELIKE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

private:
	
	//** On Overlap Begin Event */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Implement Modifier Function */
	UFUNCTION()
	void ImplementModifier(FModifierSet _InSet);
	void ClearModifier();
	void ApplyModifier() const;

	/** Melee Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* MeleeComponent;

	/** Ranged Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URangedComponent* RangedComponent;

	/** Modifier set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modifiers", meta = (AllowPrivateAccess = "true"))
	FModifierSet ModifierSet;

	
protected:
	// Called when an instance of this is placed or spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when this actor is no longer being played
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Light Attack */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void LightAttack();
	
	/** Heavy Attack */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void HeavyAttack();

	/** Weapon Ability */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void WeaponAbility();

	/** Ranged Light Attack */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void RangedLightAttack();

	/** Range Heavy Attack */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void RangedHeavyAttack();

	/** Range Weapon Ability */
	UFUNCTION(BlueprintCallable, Category = Attacks)
	void RangedWeaponAbility();

	/** Function to Receive Damage */
	UFUNCTION()
	void ReceiveDamage(float _InDamage, FAttackEffect _EffectType);

	/** Central Function to handle Player Death**/
	UFUNCTION(BlueprintCallable)
	void EnemyDeath();

	/** adds both Negative and Positive Health While Clamping it to the Min and Max**/
	void AddHealth(float _InHealth);

	//** Adds Negative And Positive Health in Percentage **/
	UFUNCTION()
	void AddHealthPercentage(float _InHealthPercent);

	/** Rotate Character to Target */
	UFUNCTION(BlueprintCallable, Category = AI)
	void RotateToTarget(AActor* TargetActor);

	/** */
	UFUNCTION(BlueprintCallable, Category = AI)
	APlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	/** Getter For Stun Flag **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetStunnedStatus() const { return bIsStunned; }

	/** Setter for the Stun Flag **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStunnedStatus(bool _NewState) { bIsStunned = _NewState; }

	/** Launch Character */
	UFUNCTION(BlueprintCallable, Category = AI)
	void CustomJump(const FVector& Destination, float Height, float Speed);

private:

	//** Player Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* PlayerCharacter;

	//** Hurt Box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UHurtBox* HurtBox;

	//** Damage Effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UDamageEffect* DamageEffect;

	
	//////////////////* Stats *////////////////////

	/** Max Health */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	
	//** Current Health */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float Health;

	//** Defense */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float Defense;

	//** Guard/Block Chance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float GuardChance;

	//** Attack Damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	int32 AttackDamage;

	//** Attack Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	//** Attack Range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	//** Attack Miss Chance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackMissChance;

	//** Movement Speed */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	//** Chase Range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float DetectionRangeRadius = 100.0f;

	//////////////////* Flags *////////////////////

	/** Stun Flag **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsStunned;

	/** Can Pick Up Item Flag -> Default = false **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanPickUpItem = false;

	/** On Item Pickup CoolDown **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bOnItemPickupCooldown = false;

	//////////////////* Timer Handles *////////////////////
	UPROPERTY()
	FTimerHandle StunTimerHandle;

	UPROPERTY()
	FTimerHandle ModifierTimer;
	
public:
	
	/** Chase Trigger Volume */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionTriggerSphere;

	/** OnChaseTriggerEnter */
	UPROPERTY(BlueprintAssignable, Category = AI)
	FOnDetectTriggerEnter OnDetectTriggerOverlap;

	/** OnDeath */
	UPROPERTY(BlueprintAssignable, Category = AI)
	FOnEnemyDeath OnEnemyDeath;

	
};
