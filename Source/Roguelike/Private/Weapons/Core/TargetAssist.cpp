// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/TargetAssist.h"
#include "Components/ShapeComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyCharacter.h"

// Sets default values for this component's properties
UTargetAssist::UTargetAssist()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetAssist::BeginPlay()
{
	Super::BeginPlay();

	// Adding and Binding a Detection Box 
	DetectionBox = Cast<UShapeComponent>(GetOwner()->GetComponentsByTag(UShapeComponent::StaticClass(), TEXT("TargetAssist"))[0]);
	if (DetectionBox)
	{
		DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &UTargetAssist::OverlapBegin);
		DetectionBox->OnComponentEndOverlap.AddDynamic(this, &UTargetAssist::OverlapEnd);
	}
	AssistCosine = UKismetMathLibrary::Cos(FMath::DegreesToRadians(AssistConeAngle));
}


// Called every frame
void UTargetAssist::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetAssist::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto _other = Cast<AEnemyCharacter>(OtherActor))
	{
		AssistActors.AddUnique(_other);
	}
}

void UTargetAssist::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AssistActors.Contains(OtherActor))
	{
		AssistActors.Remove(OtherActor);
	}
}

FRotator UTargetAssist::CalculateTargetActor()
{
	AActor* ClosestActor = nullptr;
	
	float ClosestDot = 0.0f;
	FVector2D FWD(GetOwner()->GetActorForwardVector());
	for (auto _Target : AssistActors)
	{
		FVector2D TD((_Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal2D());
		float currentDot = FVector2D::DotProduct(FWD, TD);
		if (ClosestActor == nullptr)
		{
			ClosestActor = _Target;
			ClosestDot = currentDot;
		}
		else
		{
			if (currentDot > ClosestDot)
			{
				ClosestActor = _Target;
				ClosestDot = currentDot;
			}
		}
	}

	if(ClosestActor)
		return UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), ClosestActor->GetActorLocation());

	return FRotator::ZeroRotator;
}

