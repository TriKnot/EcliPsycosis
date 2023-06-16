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
	
	/** Find positions away from player and of the given distance away from wall */
	UFUNCTION(BlueprintCallable, Category = AI)
	void FindPositionsAwayFromPlayerInBounds(float _MoveStepDistance, TArray<FVector>& _Locations, float DistanceFromHit, bool _bDebug = false);

	/** Find possible cover locations */
	UFUNCTION(BlueprintCallable, Category = AI)
	void FindCoverLocations(float _CheckDistance, TArray<FVector>& _Locations, float DistanceFromHit, int32 _DegreesOfPrecision, bool _bDebug);
	
	/** Get furthest point in array */
	UFUNCTION(BlueprintCallable, Category = AI)
	FVector GetFurthestNavPointPointFrom(TArray<FVector> _Locations, FVector _Origin, bool _Debug);

	/** Get furthest point in array */
	UFUNCTION(BlueprintCallable, Category = AI)
	FVector GetClosestNavPointPointFrom(TArray<FVector> _Locations, FVector _Origin, bool _Debug);

	
	/** Is Player In Range */
	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsPlayerInRange(float _Range);


	/** Return true if in cover */
	UFUNCTION(BlueprintCallable, Category = AI)
	bool IsInCoverFromActor(AActor* _Actor);
	
	/** Set default blackboard values */
	void SetDefaultBlackboardValues() const;

private:
	
	/** Behavior Tree Sun Phase */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	
	/** Controlled Pawn Reference */
	UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	AEnemyCharacter* ControlledCharacter;

	/** Player Character Reference */
	UPROPERTY(BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	APlayerCharacter* PlayerCharacter;
	
};
