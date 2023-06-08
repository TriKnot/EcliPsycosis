// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomStructs/EnumSet.h"
#include "RogueLikeAIController.generated.h"

class AEnemyCharacter;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class ROGUELIKE_API ARogueLikeAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateEclipseState(ENatureState _NewState);
	
	/** Find position away from player */
	UFUNCTION(BlueprintCallable, Category = AI)
	FVector FindPositionAwayFromPlayer();

	/** Find positions away from player and of the given distance away from wall */
	UFUNCTION(BlueprintCallable, Category = AI)
	void FindPositionsAwayFromPlayerInBounds(float _MoveStepDistance, TArray<FVector>& _Locations, float DistanceFromHit);

	/** Get furthest point in array */
	UFUNCTION(BlueprintCallable, Category = AI)
	FVector GetFurthestPointFrom(TArray<FVector> _Locations, FVector _Origin);
	
	/** Set default blackboard values */
	void SetDefaultBlackboardValues() const;

private:
	
	/** Behavior Tree Sun Phase */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* SunPhaseBehaviorTree;
	
	/** Behaviour Tree Eclipse Phase */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* EclipsePhaseBehaviorTree;

	/** Controlled Pawn Reference */
	UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	AEnemyCharacter* ControlledCharacter;

	/** Player Character Reference */
	UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	APlayerCharacter* PlayerCharacter;
	
};
