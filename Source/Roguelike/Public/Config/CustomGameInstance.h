// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomStructs/EnumSet.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCustomGameInstance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ENatureState NatureState;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite)
	float RemainingTime;
	
};
