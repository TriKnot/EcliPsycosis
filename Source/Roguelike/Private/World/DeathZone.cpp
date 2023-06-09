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
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADeathZone::OverlapBegin);

}


void ADeathZone::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If not HurtBox
	if(!OtherComp->IsA(UHurtBox::StaticClass()))
		return;
	
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



