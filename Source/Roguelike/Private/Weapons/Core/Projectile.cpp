// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Core/Projectile.h"

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Damage/DamageSystem.h"

// Sets default values
AProjectile::AProjectile()
{

	Root = CreateDefaultSubobject<UBoxComponent>("Root");
	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Root;
	
	HitCapsule = CreateDefaultSubobject<UCapsuleComponent>("HitBox");
	HitCapsule->SetupAttachment(RootComponent);
	HitCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitCapsule->SetGenerateOverlapEvents(true);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Make sure that we can rotate Mesh Component
	
	DamageSphere = CreateDefaultSubobject<USphereComponent>("DamageBox");
	DamageSphere->SetupAttachment(RootComponent);
	DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageSphere->SetGenerateOverlapEvents(true);

	ToggleHitBox(false);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if( RandomMeshes.Num() > 0 )
	{
		MeshComponent->SetStaticMesh(RandomMeshes[FMath::RandRange(0, RandomMeshes.Num() - 1)]);
	}
	
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Clear Timer
	if( DespawnTimerHandle.IsValid() )
		GetWorld()->GetTimerManager().ClearTimer(DespawnTimerHandle);
}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if( !OtherComp->IsA(UHurtBox::StaticClass())
		|| OtherActor == Owner
		|| OtherActor == this
		)
		return;

	// Ignore if in ignores actors list
	for(const TSubclassOf<AActor> IgnoredClass : IgnoredClasses)
	{
		if(OtherActor->IsA(IgnoredClass))
			return;
	}
	
	ToggleHitBox(true);

	OnCollisionDelegate.Broadcast();
	
	bShouldMove = false;
	// Schedule Despawn
	if(OnHitDamageTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(DespawnTimerHandle, this, &AProjectile::Despawn, OnHitDamageTime, false);
	}else
	{
		// Set active for one frame to allow for damage to be applied then despawn
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AProjectile::Despawn);		
	}
	
}

void AProjectile::OnHitOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IDamageSystem* DamageSystem;
	if(TryGetDamageSystem(OtherComp, DamageSystem))
	{
		UE_LOG( LogTemp, Warning, TEXT("Transfer Damage to %s"), *OtherComp->GetName() );
		DamageSystem->TransferDamage(Damage * DamageMultiplier, EEffectTypes::AE_None);//TODO: Add Effect
		DamagedActors.AddUnique(OtherActor);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bShouldMove)
		MoveProjectile(DeltaTime);
}

void AProjectile::Init(AActor* _InTarget, ECanDamageTypes _CanDamageTypes, AActor* _InOwner)
{
	Target = _InTarget;
	// Rotate to face the Target
	if(Target)
	{
		const FVector TargetLocation = Target->GetActorLocation();
		const FVector CurrentLocation = GetActorLocation();
		const FVector Direction = TargetLocation - CurrentLocation;
		const FRotator TargetRotation = Direction.Rotation();
		SetActorRotation(TargetRotation);
	}
	bShouldMove = true;
	CanDamageTypes = _CanDamageTypes;
	SetOwner(_InOwner);
	HitCapsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnProjectileOverlap);
}

void AProjectile::SetDamageSphereRadius(float _Radius)
{
	if(DamageSphere)
		DamageSphere->SetSphereRadius(_Radius);
}

void AProjectile::EnablePhysics(FVector StartImpulse)
{
	// Enable collision for root component
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Root->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
	// Enable physics for root component
	Root->SetSimulatePhysics(true);
	Root->SetEnableGravity(true);

	// Apply impulse
	Root->AddImpulse(StartImpulse, NAME_None, true);
}

void AProjectile::DisablePhysics()
{
	// Disable collision for root component
	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Root->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Disable physics for root component
	Root->SetSimulatePhysics(false);
	Root->SetEnableGravity(false);
	
}

void AProjectile::MoveProjectile(float DeltaTime)
{
	FVector Direction;
	const FVector CurrentLocation = GetActorLocation();
	
	if (Target)
	{
		const FVector TargetLocation = Target->GetActorLocation();
		Direction = TargetLocation - CurrentLocation;
	}else
	{
		Direction = GetActorForwardVector();
	}
	
	Direction.Normalize();
	const FRotator TargetRotation = Direction.Rotation();
	const FRotator CurrentRotation = GetActorRotation();
	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, TurnRate);
	SetActorRotation(NewRotation);
	SetActorLocation(CurrentLocation + GetActorForwardVector() * Speed * DeltaTime, true);
	LifeTimeElapsed += DeltaTime;
	if (LifeTimeElapsed >= LifeTime)
		Despawn();
}

void AProjectile::ToggleHitBox(bool bIsEnabled)
{
	if(bIsEnabled)
	{
		DamageSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnHitOverlap);

		// Enable collision for the HitBox
		DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// Find all the Overlapping Components
		TArray<UPrimitiveComponent*> OverlappingComponents;
		DamageSphere->GetOverlappingComponents(OverlappingComponents);
		// Loop through all and Damage all the Overlapping Actors
		for (UPrimitiveComponent* Component : OverlappingComponents)
		{
			IDamageSystem* DamageSystem;
			if(TryGetDamageSystem(Component, DamageSystem))
			{
				DamageSystem->TransferDamage(Damage * DamageMultiplier, EEffectTypes::AE_None);//TODO: Add Effect
				DamagedActors.AddUnique(Component->GetOwner());
			}
		}
	}
	else
	{
		DamageSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AProjectile::Despawn()
{
	if(DespawnTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DespawnTimerHandle);
	
	Destroy();
}

void AProjectile::Despawn(float Delay)
{
	// Schedule Despawn
	GetWorld()->GetTimerManager().SetTimer(DespawnTimerHandle, this, &AProjectile::Despawn, Delay, false);
}
