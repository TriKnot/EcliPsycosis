#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damage/DamageSystem.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class AAIController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDetectTriggerEnter);

UCLASS()
class ROGUELIKE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

private:

	//** On Detect Trigger Overlap Function */
	UFUNCTION()
	void OnDetectTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Melee Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* MeleeComponent;

	/** Ranged Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URangedComponent* RangedComponent;
	
protected:
	// Called when an instance of this is placed or spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


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
	UFUNCTION()
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

	//** Chase Range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float DetectionRangeRadius = 100.0f;

	//////////////////* Flags *////////////////////

	/** Stun Flag **/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsStunned;
public:
	
	/** Chase Trigger Volume */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionTriggerSphere;

	/** OnChaseTriggerEnter */
	UPROPERTY(BlueprintAssignable, Category = AI)
	FOnDetectTriggerEnter OnDetectTriggerOverlap;

	
};
