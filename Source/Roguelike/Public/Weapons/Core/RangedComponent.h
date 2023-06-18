// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Components/ActorComponent.h"
#include <limits>
#include "RangedComponent.generated.h"

class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadStateChanged, bool, bReloadState);


/**
 * Base Class for the Ranged Weapon. 
 */
UCLASS( Blueprintable )
class ROGUELIKE_API URangedComponent : public UActorComponent, public IWeapon
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnLightAttack;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnHeavyAttack;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnWeaponAbility;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FAttackStateChanged OnAttackStateChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FReloadStateChanged OnReloadStateChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Functions to Start and Stop Attack State **/
	
	UFUNCTION()
	virtual void StartAttack() override;

	UFUNCTION()
	virtual void StopAttack() override;

	UFUNCTION()
	virtual void ClearAttack() override;

	/** Implementation of Light Attack **/
	virtual void LightAttack() override;
	
	/** Implementation of Heavy Attack **/
	virtual void HeavyAttack() override;

	/** Implementation of Weapon Ability **/
	virtual void WeaponAbility() override;

	/** Cooldown for next shot */
	void StartCooldownTimer();
	
	/** Reload function -> Default to max value/max ammo*/
	UFUNCTION(BlueprintCallable)
	void Reload(int32 AmmoToReload = 2147483647);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTarget(AActor* _Target) { Target = _Target; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetFirePoint(USceneComponent* _FirePoint) { FirePoint = _FirePoint; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void MoveFirePointTo(FTransform _FirePoint) { FirePoint->SetWorldTransform(_FirePoint); };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetPreparedProjectile(AProjectile* _Projectile) { PreparedProjectile = _Projectile; };
	
	/** Configurator Variables **/

	/** Flag to set if component is enabled */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configurator")
	bool bIsEnabled = false;

	/** Light Attack Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float LightAttackDamage;

	/** Heavy Attack Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float HeavyAttackDamage;

	/** Weapon Ability Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float WeaponAbilityDamage;

	/** Attack Modifier**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float AttackModifier;

	/** Shoot Cooldown -> In seconds
	 * Values less than 0 will be translated to 0
	 */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float ShootCooldown;

	/** Reload Cooldown -> In seconds
	 * Values less than 0 will be translated to 0
	 */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float ReloadTime;
	
	/** Max Ammunition
	 * Values equal or less than 0 will treated as infinite
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configurator")
	int32 MaxAmmunition;

	/** Current Ammunition Amount */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configurator")
	int32 CurrentAmmunition;
	
	/** Types that this component can attack and damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurator")
	ECanDamageTypes CanDamageTypes;

	/** Flag for if it can shoot */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configurator")
	bool bCanShoot = true;

	/** Shot Cooldown Timer Handle */
	FTimerHandle ShootCooldownTimerHandle;

private:

	UFUNCTION(BlueprintCallable)
	AProjectile* SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass );

	UFUNCTION(BlueprintCallable)
	void ShootPreparedProjectile();
	
	/** Projectile Class to Spawn **/
	UPROPERTY(EditAnywhere, Category = "Configurator")
	TSubclassOf<AProjectile> LightProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Configurator")
	TSubclassOf<AProjectile> HeavyProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Configurator")
	TSubclassOf<AProjectile> AbilityProjectileClass;
	
	UPROPERTY(VisibleAnywhere)
	AActor* Target;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* FirePoint;

	UPROPERTY(VisibleAnywhere)
	AProjectile* PreparedProjectile;
	
};
