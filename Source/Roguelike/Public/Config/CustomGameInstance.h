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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RemainingTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int TargetLevels;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCompletedLevels() const { return CompletedLevels; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCompletedLevels(int _InNum) { CompletedLevels = _InNum; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCompletedPercentage() const { return (float)GetCompletedLevels() / (float)TargetLevels; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsTargetComplete() const { return CompletedLevels == TargetLevels; }

private:
	int CompletedLevels;
	
};
