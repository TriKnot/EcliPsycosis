// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/MeleeComponent.h"

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Components/ShapeComponent.h"

void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> _temp = GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), TEXT("MeleeHitBox"));
	//TODO: Need to Find a better way to Do this 
	for (const auto obj : _temp)
		HitBoxes.Add(Cast<UShapeComponent>(obj));
	checkf(!(HitBoxes.Num() <= 0), TEXT("Add a HitBox To the MeleeComponent HitBox Array"));
	for (const auto HitBox : HitBoxes)
	{
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &UMeleeComponent::OverlapBegin);
		HitBox->OnComponentEndOverlap.AddDynamic(this, &UMeleeComponent::OverlapEnd);
	}
	ToggleHitBox(false);
}


void UMeleeComponent::LightAttack()
{
	CurrentDamageValue = LightAttackDamage;
	OnLightAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void UMeleeComponent::StartLightAttack()
{
	DamagedActors.Empty();
	ToggleHitBox(true);
}

void UMeleeComponent::EndLightAttack()
{
	//DamageInRangeActors();
	ToggleHitBox(false);
	DamagedActors.Empty();
	OnAttackStateChanged.Broadcast(false);
}

void UMeleeComponent::HeavyAttack()
{
	CurrentDamageValue = HeavyAttackDamage;
	OnHeavyAttack.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void UMeleeComponent::StartHeavyAttack()
{
	DamagedActors.Empty();
}

void UMeleeComponent::EndHeavyAttack()
{
	DamagedActors.Empty();
	OnAttackStateChanged.Broadcast(false);
}

void UMeleeComponent::WeaponAbility()
{
	CurrentDamageValue = WeaponAbilityDamage;
	OnWeaponAbility.Broadcast();
	OnAttackStateChanged.Broadcast(true);
}

void UMeleeComponent::StartWeaponAbility()
{
	DamagedActors.Empty();
}

void UMeleeComponent::EndWeaponAbility()
{
	DamagedActors.Empty();
	OnAttackStateChanged.Broadcast(false);
}

void UMeleeComponent::ToggleHitBox(bool bIsEnabled)
{
	if(bIsEnabled)
	{
		for (const auto HitBox : HitBoxes)
		{
			// Enable collision for the HitBox
			HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			// Fins all the Overlapping Components
			TArray<UPrimitiveComponent*> OverlappingComponents;
			HitBox->GetOverlappingComponents(OverlappingComponents);
			// Loop through all and Damage all the Overlapping Actors
			for (UPrimitiveComponent* Component : OverlappingComponents)
			{
				IDamageSystem* DamageSystem;
				if(TryGetDamageSystem(Component, DamageSystem))
				{
					DamageSystem->TransferDamage(CurrentDamageValue + (CurrentDamageValue * AttackModifier)/*, CurrentEffectType*/);
					DamagedActors.AddUnique(Component->GetOwner());
				}			}
		}
	}
	else
	{
		for (const auto HitBox : HitBoxes)
			HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UMeleeComponent::DamageInRangeActors()
{
	for (auto _actor : InRangeActors)
	{
		_actor->TransferDamage(CurrentDamageValue + (CurrentDamageValue * AttackModifier)/*, CurrentEffectType*/);
		DamagedActors.Add(Cast<USceneComponent>(_actor)->GetOwner());
	}
	InRangeActors.Empty();
}

bool UMeleeComponent::TryGetDamageSystem(UPrimitiveComponent* OtherComponent, IDamageSystem*& OutDamageSystem) const
{
	AActor* OtherActor = OtherComponent->GetOwner();
	
	if ( OtherActor == GetOwner() || DamagedActors.Contains(OtherActor))
		return false;


	if (CanDamageTypes == ECanDamageTypes::CDT_Player)
	{
		if (!OtherActor->IsA(APlayerCharacter::StaticClass()))
			return false;
	}
	else if (CanDamageTypes == ECanDamageTypes::CDT_Enemy)
	{
		if (!OtherActor->IsA(AEnemyCharacter::StaticClass()))
			return false;
	}
	
	OutDamageSystem = Cast<IDamageSystem>(OtherComponent);
	if(!OutDamageSystem)
		return false;

	
	return true;
}

void UMeleeComponent::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IDamageSystem* DamageSystem;
	if(TryGetDamageSystem(OtherComp, DamageSystem))
	{
		DamageSystem->TransferDamage(CurrentDamageValue + (CurrentDamageValue * AttackModifier)/*, CurrentEffectType*/);
		DamagedActors.AddUnique(OtherActor);
	}
	
}

void UMeleeComponent::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IDamageSystem* _other = Cast<IDamageSystem>(OtherComp))
	{
		InRangeActors.Remove(_other);
	}
}
