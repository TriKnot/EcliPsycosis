// Fill out your copyright notice in the Description page of Project Settings.


#include "World/DeathZone.h"

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"

// Sets default values
ADeathZone::ADeathZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = BoxComponent;

}

void ADeathZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);		
	// If player
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerDeath();
	}

	// If enemy
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyCharacter)
	{
		EnemyCharacter->EnemyDeath();
	}
	
	
}



