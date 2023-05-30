// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/RangedComponent.h"

#include "Weapons/Core/Projectile.h"

// Called when the game starts
void URangedComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Remove Component if not enabled
	if(!bIsEnabled)
	{
		UnregisterComponent();
		DestroyComponent();
		return;
	}

	// Set up Ammo
	CurrentAmmunition = MaxAmmunition;
}

void URangedComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Clear the Timer
	if(ShootCooldownTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(ShootCooldownTimerHandle);
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
	if(!bCanShoot)
		return;
	if(MaxAmmunition > 0 && CurrentAmmunition <= 0)
		return;
	SpawnProjectile( LightProjectileClass );
	OnLightAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::HeavyAttack()
{
	if(!bCanShoot)
		return;
	if(MaxAmmunition > 0 && CurrentAmmunition <= 0)
		return;
	SpawnProjectile( HeavyProjectileClass );
	OnHeavyAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::WeaponAbility()
{
	if(!bCanShoot)
		return;
	if(MaxAmmunition > 0 && CurrentAmmunition <= 0)
		return;
	SpawnProjectile( AbilityProjectileClass );
	OnWeaponAbility.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::StartCooldownTimer()
{
	if(ShootCooldown <= 0.f)
		return;
	bCanShoot = false;
	GetWorld()->GetTimerManager().SetTimer(ShootCooldownTimerHandle, [this]() { bCanShoot = true; }, ShootCooldown, false);
}

void URangedComponent::Reload(int32 AmmoToReload)
{
	if( AmmoToReload <= 0 )
		return;

	CurrentAmmunition = FMath::Clamp(CurrentAmmunition + AmmoToReload, 0, MaxAmmunition);
}

void URangedComponent::SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass )
{
	if (!ProjectileClass)
		return;

	CurrentAmmunition--;
	StartCooldownTimer();
	
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, FirePoint->GetComponentLocation(), FirePoint->GetComponentRotation());
	
	if (Projectile)
	{
		Projectile->Init(Target, CanDamageTypes, GetOwner());
	}
}



