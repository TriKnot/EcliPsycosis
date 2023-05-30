// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damage/DamageSystem.h"
#include "GameFramework/Actor.h"
#include "CustomStructs/EnumSet.h"
#include "Projectile.generated.h"

enum class EProjectileType : uint8;
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

private:
	/** Handles checking for hit */
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Handles resolving of hit */
	UFUNCTION()
	void OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Mesh Component */
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	/** Hit Box -> Handles Damaging Other Actors */
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HitSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetTarget(AActor* _Target){ Target = _Target; }
	FORCEINLINE void SetCanDamageTypes(ECanDamageTypes _CanDamageTypes){ CanDamageTypes = _CanDamageTypes; }
	FORCEINLINE void SetDamageMultiplier(float _DamageMultiplier){ DamageMultiplier = _DamageMultiplier; }
	void MoveProjectile(float DeltaTime);
	void ToggleHitBox(bool bIsEnabled);
	bool TryGetDamageSystem(UPrimitiveComponent* OtherComponent, IDamageSystem*& OutDamageSystem) const;

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
	 * 0 = no tracking
	 */
	UPROPERTY(EditDefaultsOnly)
	float TurnRate = 10;

	/** Types that this component can attack and damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configurator")
	ECanDamageTypes CanDamageTypes;

	/* Damageable Objects that have already been hit during an attack*/
	UPROPERTY()
	TArray<AActor*> DamagedActors;


	
};
