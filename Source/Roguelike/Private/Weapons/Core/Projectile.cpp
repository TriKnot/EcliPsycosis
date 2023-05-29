// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/Projectile.h"

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Damage/DamageSystem.h"
#include "Weapons/Core/Weapon.h"

// Sets default values
AProjectile::AProjectile()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlap);
	
	HitSphere = CreateDefaultSubobject<USphereComponent>("HitBox");
	HitSphere->SetupAttachment(RootComponent);
	HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHitOverlap);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ToggleHitBox(true);
}

void AProjectile::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IDamageSystem* DamageSystem;
	if(TryGetDamageSystem(OtherComp, DamageSystem))
	{
		DamageSystem->TransferDamage(Damage * DamageMultiplier);
		DamagedActors.AddUnique(OtherActor);
	}
	}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveProjectile(DeltaTime);
}

void AProjectile::MoveProjectile(float DeltaTime)
{
	if (!Target)
		return;

	const FVector TargetLocation = Target->GetActorLocation();
	const FVector CurrentLocation = GetActorLocation();
	FVector Direction = TargetLocation - CurrentLocation;
	Direction.Normalize();
	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRotation = GetActorRotation();
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnRate);
	SetActorRotation(NewRotation);
	SetActorLocation(CurrentLocation + GetActorForwardVector() * Speed * DeltaTime, true);
	LifeTimeElapsed += DeltaTime;
	if (LifeTimeElapsed >= LifeTime)
		Destroy();
	
}

void AProjectile::ToggleHitBox(bool bIsEnabled)
{
	if(bIsEnabled)
	{
			// Enable collision for the HitBox
			HitSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			// Fins all the Overlapping Components
			TArray<UPrimitiveComponent*> OverlappingComponents;
			HitSphere->GetOverlappingComponents(OverlappingComponents);
			// Loop through all and Damage all the Overlapping Actors
			for (UPrimitiveComponent* Component : OverlappingComponents)
			{
				IDamageSystem* DamageSystem;
				if(TryGetDamageSystem(Component, DamageSystem))
				{
					DamageSystem->TransferDamage(Damage * DamageMultiplier);
					DamagedActors.AddUnique(Component->GetOwner());
				}
			}
	}
	else
	{
		HitSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

bool AProjectile::TryGetDamageSystem(UPrimitiveComponent* OtherComponent, IDamageSystem*& OutDamageSystem) const
{
	AActor* OtherActor = OtherComponent->GetOwner();
	
	if ( OtherActor == GetOwner() || DamagedActors.Contains(OtherActor))
		return false;


	if (CanDamageTypes == ECanDamageTypes::CDT_Player)
	{
		if (!OtherActor->IsA(APlayerCharacter::StaticClass()))
			return false;
	}
	else if (CanDamageTypes == ECanDamageTypes::CDT_Enemy)
	{
		if (!OtherActor->IsA(AEnemyCharacter::StaticClass()))
			return false;
	}
	
	OutDamageSystem = Cast<IDamageSystem>(OtherComponent);
	if(!OutDamageSystem)
		return false;

	
	return true;
}
