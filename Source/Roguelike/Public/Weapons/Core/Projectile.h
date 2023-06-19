// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damage/DamageSystem.h"
#include "GameFramework/Actor.h"
#include "CustomStructs/EnumSet.h"
#include "Projectile.generated.h"

class UCapsuleComponent;
class USphereComponent;

UCLASS()
class ROGUELIKE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** Handles checking for hit */
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Handles resolving of hit */
	UFUNCTION()
	void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Root Capsule Component */
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* RootCapsule;
	
	/** Mesh Component */
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	/** Hit Box -> Handles hit overlaps */
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* HitCapsule;

	/** Damage Sphere -> Handles Damaging Other Actors */
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* DamageSphere;

	/** Ignored classes */
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> IgnoredClasses;	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Init Function*/
	void Init(AActor* _InTarget, ECanDamageTypes _CanDamageTypes, AActor* _InOwner);

	/** Sets target for tracking */
	FORCEINLINE void SetTarget(AActor* _Target){ Target = _Target; }

	/** Set which targets can be damages by projectile */
	FORCEINLINE void SetCanDamageTypes(ECanDamageTypes _CanDamageTypes){ CanDamageTypes = _CanDamageTypes; }

	/** Set Damage multiplier -> Final Damage = Damage * Multiplier */
	FORCEINLINE void SetDamageMultiplier(float _DamageMultiplier){ DamageMultiplier = _DamageMultiplier; }

	/** Add Lifetime to projectile increasing how long it will be before auto despawn */
	FORCEINLINE void AddLifeTime(float _LifeTime){ LifeTime += _LifeTime; }

	/** Set Damage Sphere Radius -> Only use this to change during runtime, in editor please change in BP */
	void SetDamageSphereRadius(float _Radius);

	/** Enable Physics Simulation and enable collision for root */
	UFUNCTION( BlueprintCallable )
	void EnablePhysics();
	
private:
	/** Function to move projectile along path -> Called in Tick */
	void MoveProjectile(float DeltaTime);
	
	/** Toggles HitBox on and off
	 * Checks for overlaps when enabled
	 */
	void ToggleHitBox(bool bIsEnabled);

	/** Returns true if any DamageSystems are found
	 * Passed in DamageSystem will be set to the first found
	 */
	bool TryGetDamageSystem(UPrimitiveComponent* OtherComponent, IDamageSystem*& OutDamageSystem) const;

	/** Despawn and clean up projectile */
	void Despawn();

	/** Despawn with delay */
	UFUNCTION(BlueprintCallable)
	void Despawn(float Delay);

	/** Despawn Timer Handle */
	FTimerHandle DespawnTimerHandle;
	
	/** Projectile Target */
	UPROPERTY(VisibleAnywhere)
	AActor* Target;

	/** Projectile Speed cm/s */
	UPROPERTY(EditDefaultsOnly)
	float Speed = 1000;

	/** Damage on collision */
	UPROPERTY(EditDefaultsOnly)
	float Damage = 10;

	/** Damage Multiplier */
	float DamageMultiplier = 1;
	
	/** Max Lifetime of projectile */
	UPROPERTY(EditDefaultsOnly)
	float LifeTime = 5;

	/** Time passed since spawn */
	UPROPERTY()
	float LifeTimeElapsed = 0;

	/** Turn speed of projectile to track the target
	 * Higher values = Better tracking
	 * 0 = Perfect tracking
	 */
	UPROPERTY(EditDefaultsOnly)
	float TurnRate = 10;
	
	/** Types that this component can attack and damage */
	UPROPERTY(EditAnywhere, Category = "Configurator")
	ECanDamageTypes CanDamageTypes;

	/** Damageable Objects that have already been hit during an attack*/
	UPROPERTY()
	TArray<AActor*> DamagedActors;

	/** On hit Damage time -> How long the damaging part should be enabled after collision
	 * A time of 0 or less will be active for one frame
	 */
	UPROPERTY(EditDefaultsOnly)
	float OnHitDamageTime = 0.1f;

	/** Set to false when projectile should stop moving */
	bool bShouldMove = false;
	
};
