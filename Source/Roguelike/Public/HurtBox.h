// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Damage/DamageSystem.h"
#include "HurtBox.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UHurtBox : public UBoxComponent, public IDamageSystem
{
	GENERATED_BODY()
	
};
