// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumSet.generated.h"


UENUM(BlueprintType)
enum class ENatureState : uint8
{
	Sun			UMETA(DisplayName = "Sun"),
	Eclipse			UMETA(DisplayName = "Eclipse")
};

UENUM(BlueprintType)
enum class ECanDamageTypes  : uint8
{
	CDT_Player			UMETA( DisplayName = "Player" ),
	CDT_Enemy 			UMETA( DisplayName = "Enemy" ),
	CDT_Everything 		UMETA( DisplayName = "Everything" )
};

/**
 * 
 */
UCLASS()
class ROGUELIKE_API UEnumSet : public UObject
{
	GENERATED_BODY()
	
};
