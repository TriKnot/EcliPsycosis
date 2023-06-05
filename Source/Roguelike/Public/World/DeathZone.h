// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathZone.generated.h"

UCLASS()
class ROGUELIKE_API ADeathZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathZone();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;
};
