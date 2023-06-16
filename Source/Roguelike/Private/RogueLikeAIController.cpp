// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueLikeAIController.h"

#include "EnemyCharacter.h"
#include "NavigationSystem.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	
	RunBehaviorTree(BehaviorTree);
	SetDefaultBlackboardValues();

	// Cache Controlled Pawn
	ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());

}

void ARogueLikeAIController::FindPositionsAwayFromPlayerInBounds(float _MoveStepDistance, TArray<FVector>& _Locations, float DistanceFromHit, bool _bDebug)
{
	const FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	// Exit if no Player Character
	if (!PlayerCharacter)
		return;

	// Find Direction Straight away from Player
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const FVector DirectionFromPlayer = (ControlledCharacterLocation - PlayerLocation).GetSafeNormal();

	// Line trace to find the wall - Rotate the direction by 10 degrees and line trace to cover a total of 90 degress in front of character
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(ControlledCharacter);	
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	EDrawDebugTrace::Type DrawDebugType = _bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	FNavLocation ResultLocation;

	for (int i = 0; i < 10; i++)
	{
		const FVector EndLocation = ControlledCharacterLocation + (DirectionFromPlayer.RotateAngleAxis(i * 10 - 40, FVector::UpVector) * _MoveStepDistance);
		
		// Line trace to find the wall UKismetSystemLibrary::LineTraceSingle
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			ControlledCharacterLocation,
			EndLocation,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			TArray<AActor*>(),
			DrawDebugType,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f))
		{
			// Find position DistanceFromHit m away from the wall
			const FVector WallLocation = HitResult.Location;
			const FVector DirectionFromWall = HitResult.ImpactNormal;
			const FVector TestOriginPosition = WallLocation + (DirectionFromWall * DistanceFromHit);
			
			// Project the hit location onto the navmesh
			NavSystem->ProjectPointToNavigation(TestOriginPosition, ResultLocation, FVector(100, 100, 100));

			// Check if the projected location is valid
			if (ResultLocation.Location == FVector::ZeroVector)
				continue;			
		}else
		{
			NavSystem->ProjectPointToNavigation(EndLocation, ResultLocation, FVector(100, 100, 100));

			if (ResultLocation.Location == FVector::ZeroVector)
				continue;			
		}
		_Locations.Add(ResultLocation.Location);
	}
}

void ARogueLikeAIController::FindCoverLocations(float _CheckDistance, TArray<FVector>& _Locations, float DistanceFromHit, int32 _DegreesOfPrecision, bool _bDebug)
{
	const FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	// Exit if no Player Character
	if (!PlayerCharacter)
		return;


	// Setup Line Trace
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(ControlledCharacter);
	CollisionQueryParams.AddIgnoredActor(PlayerCharacter);
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	EDrawDebugTrace::Type DrawDebugType = _bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	FNavLocation ResultLocation;

	// Get Player Location and project it on to the navmesh for the end location
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FNavLocation EndLocation;
	NavSystem->ProjectPointToNavigation(PlayerLocation, EndLocation, FVector(100, 100, 100));

	for (int i = 0; i < _DegreesOfPrecision; i++)
	{
		// Calculate the Start location based on the angle
		
		const FVector StartLocation = ControlledCharacterLocation + (UKismetMathLibrary::GetForwardVector(ControlledCharacter->GetActorRotation()) * _CheckDistance).RotateAngleAxis(i * 360 / _DegreesOfPrecision, FVector::UpVector);
		FNavLocation StartNavLocation;
		NavSystem->ProjectPointToNavigation(StartLocation, StartNavLocation, FVector(_CheckDistance, _CheckDistance, _CheckDistance));
		
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			StartNavLocation.Location,
			EndLocation.Location,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			TArray<AActor*>(),
			DrawDebugType,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			5.f))
		{
			// Find position DistanceFromHit m away from the wall
			const FVector WallLocation = HitResult.Location;
			const FVector DirectionFromWall = HitResult.ImpactNormal;
			const FVector TestOriginPosition = WallLocation + (DirectionFromWall * DistanceFromHit);
			
			// Project the hit location onto the navmesh
			NavSystem->ProjectPointToNavigation(TestOriginPosition, ResultLocation, FVector(100, 100, 100));
			UE_LOG( LogTemp, Warning, TEXT("1Location: %s"), *ResultLocation.Location.ToString() );

			// Check if the projected location is valid
			if (ResultLocation.Location == FVector::ZeroVector)
				continue;			

			_Locations.Add(ResultLocation.Location);
		}
	}
	
	
}

FVector ARogueLikeAIController::GetFurthestNavPointPointFrom(TArray<FVector> _Locations, FVector _Origin, bool _Debug)
{
	FVector FurthestLocation = FVector::ZeroVector;
	float FurthestDistance = 0.f;
	for (FVector Location : _Locations)
	{
		const float Distance = (Location - _Origin).Size();
		if (Distance > FurthestDistance)
		{
			FurthestDistance = Distance;
			FurthestLocation = Location;
		}
	}

	// Validate Location on NavMesh
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation ResultLocation;
	NavSystem->ProjectPointToNavigation(FurthestLocation, ResultLocation, FVector(100, 100, 100));
	
	// Draw Debug
	if(_Debug)
		DrawDebugSphere(GetWorld(), ResultLocation, 100.f, 12, FColor::Red, false, 5.f);
	
	return ResultLocation;
}

FVector ARogueLikeAIController::GetClosestNavPointPointFrom(TArray<FVector> _Locations, FVector _Origin, bool _Debug)
{
	FVector ClosestLocation = FVector::ZeroVector;
	float ClosestDistance = std::numeric_limits<float>::max();
	for (FVector Location : _Locations)
	{
		const float Distance = (Location - _Origin).Size();
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestLocation = Location;
		}
	}
	
	// Validate Location on NavMesh
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation ResultLocation;
	NavSystem->ProjectPointToNavigation(ClosestLocation, ResultLocation, FVector(100, 100, 100));
	
	// Draw Debug
	if(_Debug)
		DrawDebugSphere(GetWorld(), ResultLocation, 100.f, 12, FColor::Red, false, 5.f);
	
	return ResultLocation;
}

bool ARogueLikeAIController::IsPlayerInRange(float _Range)
{
	// Exit if no Player Character
	if (!PlayerCharacter)
		return false;
	// Find Distance to Player
	const FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const float DistanceToPlayer = (ControlledCharacterLocation - PlayerLocation).Size();
	// Return true if distance is less than range
	return DistanceToPlayer < _Range;
}


bool ARogueLikeAIController::IsInCoverFromActor(AActor* _Actor)
{
	// Exit if no Player Character
	if (!PlayerCharacter)
		return false;
	// Get start and end for line trace
	const FVector ControlledCharacterLocation = ControlledCharacter->GetActorLocation();
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation StartLocation;
	FNavLocation EndLocation;

	// Project the controlled character and player location onto the navmesh
	NavSystem->ProjectPointToNavigation(ControlledCharacterLocation, StartLocation, FVector(100, 100, 1000));
	NavSystem->ProjectPointToNavigation(PlayerLocation, EndLocation, FVector(100, 100, 1000));

	// Setup Line Trace
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(ControlledCharacter);
	IgnoredActors.Add(PlayerCharacter);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;

	// Line trace to player and return if something is hit
	bool hit =  UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		StartLocation,
		EndLocation,
		UEngineTypes::ConvertToTraceType(ECC_Camera),
		false,
		IgnoredActors,
		DrawDebugType,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f);

	AActor* HitActor = HitResult.GetActor();

	if(HitActor)
		UE_LOG( LogTemp, Warning, TEXT("Controlledcharacter: %s, HitActor: %s"), *ControlledCharacter->GetName(), *HitActor->GetName() );
	
	return hit;
}


void ARogueLikeAIController::SetDefaultBlackboardValues() const
{
	// Exit if no Blackboard
	if (!Blackboard)
		return;

	//Ensure Keys are present and Set Default Values
	if (Blackboard->IsValidKey(Blackboard->GetKeyID("PlayerReference")))
		Blackboard->SetValueAsObject("PlayerReference", PlayerCharacter);
	if (Blackboard->IsValidKey(Blackboard->GetKeyID("bIsSunPhase")))
		Blackboard->SetValueAsBool("bIsSunPhase", true);
}

void ARogueLikeAIController::UpdateEclipseState(ENatureState _NewState) 
{
	if(_NewState == ENatureState::Eclipse)
	{
		if (Blackboard->IsValidKey(Blackboard->GetKeyID("bIsEclipsePhase")))
			Blackboard->SetValueAsBool("bIsEclipsePhase", true);
	}
	else
	{
		if (Blackboard->IsValidKey(Blackboard->GetKeyID("bIsSunPhase")))
			Blackboard->SetValueAsBool("bIsSunPhase", true);
	}
}





