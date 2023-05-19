// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructSet.generated.h"

USTRUCT(BlueprintType)
struct FModifierSet
{
	GENERATED_BODY()
public:
	FModifierSet() : HPModifier(0.0f), SpeedModifier(0.0f), AttackModifier(0.0f), RangeModifier(0.0f), CooldownModifier(0.0f), EffectTime(0.0f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HPModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedModifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackModifier;
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
