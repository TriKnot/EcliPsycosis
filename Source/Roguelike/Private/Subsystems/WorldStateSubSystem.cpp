// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/WorldStateSubSystem.h"

#include "EnemyCharacter.h"

TArray<AEnemyCharacter*> UWorldStateSubSystem::GetActiveEnemiesWithingDistanceFromPoint(FVector Point, float Distance) const
{
	TArray<AEnemyCharacter*> EnemiesWithinDistance;

	for (AEnemyCharacter* ActiveEnemy : ActiveEnemies)
	{
		if(ActiveEnemy)
		{
			const float DistanceToEnemy = FVector::Dist(ActiveEnemy->GetActorLocation(), Point);
			if(DistanceToEnemy <= Distance)
			{
				EnemiesWithinDistance.Add(ActiveEnemy);
			}
		}
	}
	return EnemiesWithinDistance;
}
