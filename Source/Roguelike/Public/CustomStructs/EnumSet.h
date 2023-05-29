// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumSet.generated.h"


UENUM(BlueprintType)
enum class ENatureState : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
	Eclipse			UMETA(DisplayName = "Eclipse")
};
/**
 * 
 */
UCLASS()
class ROGUELIKE_API UEnumSet : public UObject
{
	GENERATED_BODY()
	
};
