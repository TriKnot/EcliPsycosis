// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/MeleeComponent.h"
#include "Components/ShapeComponent.h"
void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> _temp = GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), TEXT("WeaponHitBox"));
	//TODO: Need to Find a better way to Do this 
	for (const auto obj : _temp)
		HitBoxes.Add(Cast<UShapeComponent>(obj));
	UE_LOG(LogTemp, Warning, TEXT("Number of HitBoxes: %d"), HitBoxes.Num());
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
}

void UMeleeComponent::StartLightAttack()
{
	DamageableObjects.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndLightAttack()
{
	DamageableObjects.Empty();
	bIsAttacking = false;
}

void UMeleeComponent::HeavyAttack()
{
	CurrentDamageValue = HeavyAttackDamage;
}

void UMeleeComponent::StartHeavyAttack()
{
	DamageableObjects.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndHeavyAttack()
{
	DamageableObjects.Empty();
	bIsAttacking = false;
}

void UMeleeComponent::WeaponAbility()
{
	CurrentDamageValue = WeaponAbilityDamage;
}

void UMeleeComponent::StartWeaponAbility()
{
	DamageableObjects.Empty();
	bIsAttacking = true;
}

void UMeleeComponent::EndWeaponAbility()
{
	DamageableObjects.Empty();
	bIsAttacking = false;
}


void UMeleeComponent::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking)
		return;
	if (IDamageSystem* _other = Cast<IDamageSystem>(OtherComp))
	{
		if (!DamageableObjects.Contains(_other))
		{
			_other->TransferDamage(CurrentDamageValue, CurrentEffectType); 
			DamageableObjects.Add(_other);
		}
	}
}

void UMeleeComponent::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IDamageSystem* _other = Cast<IDamageSystem>(OtherComp))
	{
		
	}
}
