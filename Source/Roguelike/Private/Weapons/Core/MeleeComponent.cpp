// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/MeleeComponent.h"
#include "Components/ShapeComponent.h"
#include "Damage/DamageSystem.h"

void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> _temp = GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), TEXT("WeaponHitBox"));
	//TODO: Need to Find a better way to Do this 
	for (const auto obj : _temp)
		HitBoxes.Add(Cast<UShapeComponent>(obj));
	checkf(!(HitBoxes.Num() <= 0), TEXT("Add a HitBox To the MeleeComponent HitBox Array"));
	for (const auto HitBox : HitBoxes)
	{
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &UMeleeComponent::OverlapBegin);
		HitBox->OnComponentEndOverlap.AddDynamic(this, &UMeleeComponent::OverlapEnd);
	}
}


void UMeleeComponent::LightAttack()
{
	CurrentDamageValue = LightAttackDamage;
	UE_LOG(LogTemp, Error, TEXT("Damage Amount %f"), CurrentDamageValue);
	OnLightAttack.Broadcast();
}

void UMeleeComponent::StartLightAttack()
{
	DamagedActors.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndLightAttack()
{
	DamagedActors.Empty();
	bIsAttacking = false;
}

void UMeleeComponent::HeavyAttack()
{
	CurrentDamageValue = HeavyAttackDamage;
	OnHeavyAttack.Broadcast();
}

void UMeleeComponent::StartHeavyAttack()
{
	DamagedActors.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndHeavyAttack()
{
	DamagedActors.Empty();
	bIsAttacking = false;
}

void UMeleeComponent::WeaponAbility()
{
	CurrentDamageValue = WeaponAbilityDamage;
	OnWeaponAbility.Broadcast();
}

void UMeleeComponent::StartWeaponAbility()
{
	DamagedActors.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndWeaponAbility()
{
	DamagedActors.Empty();
	bIsAttacking = false;
}


void UMeleeComponent::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking || OtherActor == GetOwner() || DamagedActors.Contains(OtherActor))
		return;
	if (IDamageSystem* _other = Cast<IDamageSystem>(OtherComp))
	{
		UE_LOG(LogTemp, Error, TEXT("UMeleeComponent::OverlapBegin"));
		_other->TransferDamage(CurrentDamageValue + (CurrentDamageValue * AttackModifier)/*, CurrentEffectType*/); 
		DamagedActors.Add(OtherActor);
	}
}

void UMeleeComponent::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IDamageSystem* _other = Cast<IDamageSystem>(OtherComp))
	{
		
	}
}
