// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueLikeAIController.h"

#include "EnemyCharacter.h"
#include "NavigationSystem.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Subsystems/EclipseSubsystem.h"


void ARogueLikeAIController::BeginPlay()
{
	Super::BeginPlay();

	// Find Player Character
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Hook up to EclipseSubsystem to be notified of Eclipse Phase
	UEclipseSubsystem* EclipseSubsystem = GetWorld()->GetSubsystem<UEclipseSubsystem>();
	if (EclipseSubsystem)
		EclipseSubsystem->OnNatureStateChanged.AddDynamic(this, &ARogueLikeAIController::UpdateEclipseState);
	UBehaviorTree* BehaviorTree = EclipseSubsystem->GetCurrentState() == ENatureState::Sun ? SunPhaseBehaviorTree : EclipsePhaseBehaviorTree;
	RunBehaviorTree(BehaviorTree);
	SetDefaultBlackboardValues();

	// Cache Controlled Pawn
	ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (!ControlledCharacter)
		return;
}

FVector ARogueLikeAIController::FindPositionAwayFromPlayer()
{
	const FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	// Exit if no Player Character
	if (!PlayerCharacter)
		return ControlledCharacterLocation;
	
	// Find Direction Straight away from Player
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector DirectionFromPlayer = (ControlledCharacterLocation - PlayerLocation).GetSafeNormal();
	
	// Find the size of the navigable bounds
	const FBox NavBounds = UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetNavigableWorldBounds();
	const auto NavBoundsSize = NavBounds.GetSize();

	// Test Origin Position is the navigation bounds size in the direction away from the player
	const FVector TestOriginPosition = PlayerLocation + (DirectionFromPlayer * NavBoundsSize);
	
	// Test Radius should be the distance from the controlled character to the test origin position as to not limit the search area and make sure it is on the navmesh
	const float TestRadius = (ControlledCharacterLocation - TestOriginPosition).Size();
	
	// Find furthest point inside of navigable bounds
	FNavLocation ResultLocation;
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	NavSystem->GetRandomPointInNavigableRadius(TestOriginPosition, TestRadius, ResultLocation);

	// Return the furthest point inside of navigable bounds
	return ResultLocation.Location;
}

void ARogueLikeAIController::SetDefaultBlackboardValues() const
{
	// Exit if no Blackboard
	if (!Blackboard)
		return;

	//Ensure Keys are present and Set Default Values
	if (Blackboard->IsValidKey(Blackboard->GetKeyID("PlayerReference")))
		Blackboard->SetValueAsObject("PlayerReference", PlayerCharacter);
}

void ARogueLikeAIController::UpdateEclipseState(ENatureState _NewState) 
{
	if(_NewState == ENatureState::Eclipse)
	{
		if(EclipsePhaseBehaviorTree)
			RunBehaviorTree(EclipsePhaseBehaviorTree);
	}
	else
	{
		if(SunPhaseBehaviorTree)
			RunBehaviorTree(SunPhaseBehaviorTree);
	}
}



