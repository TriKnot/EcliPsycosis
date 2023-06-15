// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldStateSubSystem.generated.h"

class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API UWorldStateSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "WorldStateSubSystem")
	FORCEINLINE TArray<AEnemyCharacter*> GetActiveEnemies() const
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveEnemies: %d"), ActiveEnemies.Num());
		return ActiveEnemies;
	}
	
	UFUNCTION(BlueprintCallable, Category = "WorldStateSubSystem")
	FORCEINLINE void AddActiveEnemy(AEnemyCharacter* Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActiveEnemies: %d"), ActiveEnemies.Num());
		ActiveEnemies.Add(Enemy);
	}
	
	UFUNCTION(BlueprintCallable, Category = "WorldStateSubSystem")
	FORCEINLINE void RemoveActiveEnemy(AEnemyCharacter* Enemy)
	{
		UE_LOG(	LogTemp, Warning, TEXT("ActiveEnemies: %d"), ActiveEnemies.Num());
		ActiveEnemies.Remove(Enemy);
	}

	UFUNCTION(BlueprintCallable, Category = "WorldStateSubSystem")
	FORCEINLINE void ClearActiveEnemies() { ActiveEnemies.Empty(); }

	UFUNCTION(BlueprintCallable , Category = "WorldStateSubSystem")
	TArray<AEnemyCharacter*> GetActiveEnemiesWithingDistanceFromPoint(FVector Point, float Distance) const;
	
private:
	/** Stores all currently active enemies in the world, they should add themselves on beginplay */
	TArray<AEnemyCharacter*> ActiveEnemies;
};
