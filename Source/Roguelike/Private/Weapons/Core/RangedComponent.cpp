// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/RangedComponent.h"

#include "Weapons/Core/Projectile.h"

// Called when the game starts
void URangedComponent::BeginPlay()
{
	Super::BeginPlay();
	if(!bIsEnabled)
	{
		// Remove Component if not enabled
		//UnregisterComponent();
		DestroyComponent();
		return;
	}
}

void URangedComponent::StartAttack()
{
	// Setup for the attack
}

void URangedComponent::StopAttack()
{
	// Stop the attack
}

void URangedComponent::ClearAttack()
{
	OnAttackStateChanged.Broadcast(false);
}

void URangedComponent::LightAttack()
{
	SpawnProjectile( LightProjectileClass );
	OnLightAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::HeavyAttack()
{
	SpawnProjectile( HeavyProjectileClass );
	OnHeavyAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::WeaponAbility()
{
	SpawnProjectile( AbilityProjectileClass );
	OnWeaponAbility.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass ) const
{
	if (!ProjectileClass)
		return;
	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation());
	
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
		Projectile->SetTarget(Target);
	}
}



