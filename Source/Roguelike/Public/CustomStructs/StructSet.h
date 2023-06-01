// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomStructs/EnumSet.h"
#include "StructSet.generated.h"

USTRUCT(BlueprintType)
struct FModifierSet
{
	GENERATED_BODY()
public:
	FModifierSet(): HPModifier(1.0f), 
					SpeedModifier(1.0f), 
					LightAttackModifier(1.0f), 
					LightAttackEffect(EEffectTypes::AE_None),
					HeavyAttackModifier(1.0f), 
					HeavyAttackEffect(EEffectTypes::AE_None),
					WeaponAbilityModifier(1.0f), 
					WeaponAbilityEffect(EEffectTypes::AE_None),
					DamageOverTime(0.0f),
					AttackEffectTime(0.0f),
					RangeModifier(1.0f), 
					CooldownModifier(1.0f), 
					EffectTime(1.0f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightAttackModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectTypes LightAttackEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeavyAttackModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectTypes HeavyAttackEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponAbilityModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectTypes WeaponAbilityEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageOverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackEffectTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RangeModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectTime;

	static FModifierSet ZERO()
	{
		return FModifierSet();
	}
};


USTRUCT(BlueprintType, Blueprintable)
struct FAttackEffect
{
	GENERATED_BODY()
public:
	FAttackEffect() : _EffectType(EEffectTypes::AE_None), DamageOverTime(0.0f), AttackEffectTime(0.0f) {}
	FAttackEffect(EEffectTypes _effectType) : _EffectType(_effectType), DamageOverTime(0.0f), AttackEffectTime(0.0f) {}
	FAttackEffect(EEffectTypes _effectType, float _DamageOverTime, float _AttackEffectTime) : _EffectType(_effectType), DamageOverTime(_DamageOverTime), AttackEffectTime(_AttackEffectTime) {}
	EEffectTypes _EffectType;
	float DamageOverTime;
	float AttackEffectTime;
};

UCLASS()
class ROGUELIKE_API AStructSet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructSet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
