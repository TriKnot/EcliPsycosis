// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.h"
#include "ItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class ROGUELIKE_API AItemBase : public AActor, public IPickupItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Pickup() override;

	virtual FModifierSet GetModifiers() const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FModifierSet ModifierSet;

private:
	

	/**  Root Collider Component **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* ColliderComponent;


	/** Component To Hold the Static Mesh **/ // TODO: Might need to add a Particle Effect if required
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;
};
