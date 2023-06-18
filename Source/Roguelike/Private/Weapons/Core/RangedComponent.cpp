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
	// Check if we have a prepared projectile and if the projectile is of the correct type
	if(!PreparedProjectile)
	{
		PreparedProjectile = SpawnProjectile( LightProjectileClass );
	}
	if(PreparedProjectile->GetClass() != LightProjectileClass)
	{
		PreparedProjectile->Destroy();
		PreparedProjectile = nullptr;
		PreparedProjectile = SpawnProjectile( LightProjectileClass );
	}

	// Shoot the projectile
	ShootPreparedProjectile();
	
	OnLightAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::HeavyAttack()
{
	if(!bCanShoot)
		return;
	if(MaxAmmunition > 0 && CurrentAmmunition <= 0)
		return;
	// Check if we have a prepared projectile and if the projectile is of the correct type
	if(!PreparedProjectile)
	{
		PreparedProjectile = SpawnProjectile( HeavyProjectileClass );
	}
	if(PreparedProjectile->GetClass() != HeavyProjectileClass)
	{
		PreparedProjectile->Destroy();
		PreparedProjectile = nullptr;
		PreparedProjectile = SpawnProjectile( HeavyProjectileClass );
	}

	// Shoot the projectile
	ShootPreparedProjectile();
	
	OnHeavyAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void URangedComponent::WeaponAbility()
{
	if(!bCanShoot)
		return;
	if(MaxAmmunition > 0 && CurrentAmmunition <= 0)
		return;
	// Check if we have a prepared projectile and if the projectile is of the correct type
	if(!PreparedProjectile)
	{
		PreparedProjectile = SpawnProjectile( AbilityProjectileClass );
	}
	if(PreparedProjectile->GetClass() != AbilityProjectileClass)
	{
		PreparedProjectile->Destroy();
		PreparedProjectile = nullptr;
		PreparedProjectile = SpawnProjectile( AbilityProjectileClass );
	}

	// Shoot the projectile
	ShootPreparedProjectile();
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
	OnReloadStateChanged.Broadcast(true);
}

AProjectile* URangedComponent::SpawnProjectile(TSubclassOf<AProjectile> ProjectileClass )
{
	return GetWorld()->SpawnActor<AProjectile>(ProjectileClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
}

void URangedComponent::ShootPreparedProjectile()
{
	CurrentAmmunition--;
	StartCooldownTimer();

	if (PreparedProjectile)
	{
		// Set location and rotation of the projectile
		PreparedProjectile->SetActorRotation(GetOwner()->GetActorRotation());
		// Make sure it's detached from the owner
		PreparedProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// Init the projectile
		PreparedProjectile->Init(Target, CanDamageTypes, GetOwner());
	}
	PreparedProjectile = nullptr;
}



