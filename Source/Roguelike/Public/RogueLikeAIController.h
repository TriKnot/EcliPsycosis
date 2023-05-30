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
	/** Setup */
	void SetBehaviorTree(UBehaviorTree* _InBehaviorTree);
	void SetDefaultBlackboardValues() const;

	void UpdateEcpliseState(ENatureState _NewState);

	/** Sets player detected key to true */
	UFUNCTION()
	void PlayerDetected();

	/** Find position away from player */
	UFUNCTION(BlueprintCallable, Category = AI)
	FVector FindPositionAwayFromPlayer();
	
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
