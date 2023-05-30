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
		EclipseSubsystem->OnNatureStateChanged.AddDynamic(this, &ARogueLikeAIController::UpdateEcpliseState);
	UBehaviorTree* BehaviorTree = EclipseSubsystem->GetCurrentState() == ENatureState::Sun ? SunPhaseBehaviorTree : EclipsePhaseBehaviorTree;
	SetBehaviorTree(BehaviorTree);

	// Cache Controlled Pawn
	ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (!ControlledCharacter)
		return;

	// Subscribe to OnDetectTriggerOverlap
	ControlledCharacter->OnDetectTriggerOverlap.AddDynamic(this, &ARogueLikeAIController::PlayerDetected);

}

void ARogueLikeAIController::PlayerDetected()
{
	// Exit if no Blackboard
	if (!Blackboard)
		return;

	Blackboard->SetValueAsBool("PlayerInRange", true);

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

void ARogueLikeAIController::SetBehaviorTree(UBehaviorTree* _InBehaviorTree)
{
	RunBehaviorTree(_InBehaviorTree);
	
	SetDefaultBlackboardValues();
}

void ARogueLikeAIController::SetDefaultBlackboardValues() const
{
	// Exit if no Blackboard
	if (!Blackboard)
		return;

	//Ensure Keys are present
	if (!Blackboard->IsValidKey(Blackboard->GetKeyID("PlayerInRange")))
		UE_LOG( LogTemp, Error, TEXT("ARogueLikeAIController:: PlayerInRange Key is not Valid. Please create in Blackboard %s as a %s"), *Blackboard->GetBlackboardAsset()->GetName(), *UBlackboardKeyType_Bool::StaticClass()->GetName() );
	if (!Blackboard->IsValidKey(Blackboard->GetKeyID("PlayerReference")))
		UE_LOG( LogTemp, Error, TEXT("ARogueLikeAIController:: PlayerReference Key is not Valid. Please create in Blackboard %s as a %s"), *Blackboard->GetBlackboardAsset()->GetName(), *UBlackboardKeyType_Object::StaticClass()->GetName() );

	// Set Default Values
	Blackboard->SetValueAsBool("PlayerInRange", false);
	Blackboard->SetValueAsObject("PlayerReference", PlayerCharacter);
}

void ARogueLikeAIController::UpdateEcpliseState(ENatureState _NewState) 
{
	if(_NewState == ENatureState::Eclipse)
	{
		if(SunPhaseBehaviorTree)
			SetBehaviorTree(SunPhaseBehaviorTree);
	}
	else
	{
		if(EclipsePhaseBehaviorTree)
			SetBehaviorTree(EclipsePhaseBehaviorTree);
	}
}



