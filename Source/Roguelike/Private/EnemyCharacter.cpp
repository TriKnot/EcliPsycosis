#include "EnemyCharacter.h"
#include "Damage/DamageEffect.h"
#include "HurtBox.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/WorldStateSubSystem.h"
#include "Weapons/Core/MeleeComponent.h"
#include "Weapons/Core/RangedComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Hurt Box
	HurtBox = CreateDefaultSubobject<UHurtBox>(TEXT("HurtBox"));
	HurtBox->OnReceivedDamage.AddDynamic(this, &AEnemyCharacter::ReceiveDamage);
	HurtBox->SetupAttachment(RootComponent);

	//DamageEffect
	DamageEffect = CreateDefaultSubobject<UDamageEffect>(TEXT("DamageEffect"));
	DamageEffect->OnStunStateChanged.AddDynamic(this, &AEnemyCharacter::SetStunnedStatus);
	DamageEffect->OnDamagePerSecond.AddDynamic(this, &AEnemyCharacter::AddHealthPercentage);

	// Melee Component
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));

	// Ranged Component
	RangedComponent = CreateDefaultSubobject<URangedComponent>(TEXT("RangedComponent"));

	// Bind Overlap Delegate for Pickup
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);

}

void AEnemyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bCanPickUpItem)
	{
		if (bOnItemPickupCooldown)
			return;
		IPickupItem* _Temp = Cast<IPickupItem>(OtherActor);
		if (!_Temp)
			return;
		ImplementModifier(_Temp->GetModifiers());
		_Temp->Pickup();
	}
}

void AEnemyCharacter::ImplementModifier(FModifierSet _InSet)
{
	bOnItemPickupCooldown = true;
	ModifierSet.SpeedModifier = _InSet.SpeedModifier;
	ModifierSet.LightAttackModifier = _InSet.LightAttackModifier;
	ModifierSet.HeavyAttackModifier = _InSet.HeavyAttackModifier;
	ModifierSet.WeaponAbilityModifier = _InSet.WeaponAbilityModifier;
	if (_InSet.LightAttackEffect != EEffectTypes::AE_None)
	{
		ModifierSet.LightAttackEffect = _InSet.LightAttackEffect;
		ModifierSet.DamageOverTime = _InSet.DamageOverTime;
		ModifierSet.AttackEffectTime = _InSet.AttackEffectTime;
	}
	if (_InSet.HeavyAttackEffect != EEffectTypes::AE_None)
		ModifierSet.HeavyAttackEffect = _InSet.HeavyAttackEffect;
	if (_InSet.WeaponAbilityEffect != EEffectTypes::AE_None)
		ModifierSet.WeaponAbilityEffect = _InSet.WeaponAbilityEffect;
	
	ApplyModifier();
	
	AddHealth(Health * _InSet.HPModifier);

	
	_InSet.EffectTime > 0.0f ? GetWorldTimerManager().SetTimer(ModifierTimer, this, &AEnemyCharacter::ClearModifier, _InSet.EffectTime, false) : ClearModifier();

}

void AEnemyCharacter::ClearModifier()
{
	bOnItemPickupCooldown = false;
	ModifierSet = FModifierSet::ZERO();
	ApplyModifier();
}

void AEnemyCharacter::ApplyModifier() const
{
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed * ModifierSet.SpeedModifier;
	MeleeComponent->LightAttackModifier = ModifierSet.LightAttackModifier;
	MeleeComponent->LightAttackEffect = ModifierSet.LightAttackEffect;
	MeleeComponent->HeavyAttackModifier = ModifierSet.HeavyAttackModifier;
	MeleeComponent->HeavyAttackEffect = ModifierSet.HeavyAttackEffect;
	MeleeComponent->WeaponAbilityModifier = ModifierSet.WeaponAbilityModifier;
	MeleeComponent->WeaponAbilityEffect = ModifierSet.WeaponAbilityEffect;
	MeleeComponent->DamageOverTime = ModifierSet.DamageOverTime;
	MeleeComponent->AttackEffectTime = ModifierSet.AttackEffectTime;
}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HurtBox->OnReceivedDamage.AddDynamic(DamageEffect, &UDamageEffect::DamageReceived);

	// Cache Player Character
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if(RangedComponent)
		RangedComponent->SetTarget(PlayerCharacter);

	//Set Default Health
	//Health = MaxHealth;

	// Save max movement speed set in editor
	MovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	// Add self to the list of enemies
	 GetWorld()->GetSubsystem<UWorldStateSubSystem>()->AddActiveEnemy(this);
	
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Remove self from the list of enemies
	GetWorld()->GetSubsystem<UWorldStateSubSystem>()->RemoveActiveEnemy(this);

	// Make sure to clear all timers
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void AEnemyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Set Detection Trigger Sphere Radius
	if(DetectionTriggerSphere)
	{
		DetectionTriggerSphere->SetSphereRadius(DetectionRangeRadius);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::LightAttack()
{
	// Initialize attack here
	if(MeleeComponent)
		MeleeComponent->LightAttack();
	else
		UE_LOG(LogTemp, Warning, TEXT("MeleeComponent not found in %s"), *GetName());
}

void AEnemyCharacter::HeavyAttack()
{
	// Initialize attack here
	if(MeleeComponent)
		MeleeComponent->HeavyAttack();
	else
		UE_LOG(LogTemp, Warning, TEXT("MeleeComponent not found in %s"), *GetName());
}

void AEnemyCharacter::WeaponAbility()
{
	// Initialize attack here
	if(MeleeComponent)
		MeleeComponent->WeaponAbility();
	else
		UE_LOG(LogTemp, Warning, TEXT("MeleeComponent not found in %s"), *GetName());
}

void AEnemyCharacter::RangedLightAttack()
{
	// Initialize attack here
	if(RangedComponent)
		RangedComponent->LightAttack();
	else
		UE_LOG(LogTemp, Warning, TEXT("RangedComponent not found in %s"), *GetName());

}

void AEnemyCharacter::RangedHeavyAttack()
{
	// Initialize attack here
	if(RangedComponent)
		RangedComponent->HeavyAttack();
	else
		UE_LOG(LogTemp, Warning, TEXT("RangedComponent not found in %s"), *GetName());
}

void AEnemyCharacter::RangedWeaponAbility()
{
	// Initialize attack here
	if(RangedComponent)
		RangedComponent->WeaponAbility();
	else
		UE_LOG(LogTemp, Warning, TEXT("RangedComponent not found in %s"), *GetName());
}

void AEnemyCharacter::ReceiveDamage(float _InDamage, FAttackEffect _EffectType)
{
	AddHealth(-_InDamage);
}

void AEnemyCharacter::EnemyDeath()
{
	// BreadCast Death
	OnEnemyDeath.Broadcast();
	// Destroy this actor
	GetWorld()->DestroyActor(this);
}

void AEnemyCharacter::AddHealth(float _InHealth)
{
	Health = FMath::Clamp(Health += _InHealth, 0.0f, MaxHealth);
	if (Health <= 0.0f)
	{
		EnemyDeath();
	}
}

void AEnemyCharacter::AddHealthPercentage(float _InHealthPercent)
{
	AddHealth(Health * _InHealthPercent);
}

void AEnemyCharacter::RotateToTarget(AActor* TargetActor)
{
	// Exit if no target
	if (!TargetActor)
		return;

	// Get Location of Target
	const FVector TargetLocation = TargetActor->GetActorLocation();

	// Find Look at Rotation
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);

	// Set Actor Rotation
	SetActorRotation(LookAtRotation);

}

void AEnemyCharacter::CustomJump(const FVector& Destination, float Height, float Speed)
{
	const FVector StartLocation = GetActorLocation();
	FVector JumpApex = (StartLocation + Destination) / 2.0f;
	JumpApex.Z += Height;

	const float JumpTime = FVector::Dist(StartLocation, JumpApex) / Speed;

	const float Gravity = GetCharacterMovement()->GetGravityZ();

	const float InitialVelocity = (JumpApex.Z - StartLocation.Z + 0.5f * Gravity * JumpTime * JumpTime) / JumpTime;

	FVector JumpVelocity = (Destination - StartLocation) / JumpTime;
	JumpVelocity.Z = InitialVelocity;

	LaunchCharacter(JumpVelocity, true, true);
}

void AEnemyCharacter::AttachEnemyToSocketOnCharacter(ACharacter* OtherActor, FName SocketName)
{
	// Check if Other Actor is valid
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Other Actor is not valid in %s"), *GetName());
		return;
	}
	// Get Socket Transform from Other Actor
	const FTransform SocketTransform = OtherActor->GetMesh()->GetSocketTransform(SocketName);
	// Check if socket is valid
	if (SocketTransform.IsValid())
	{
		// Set Attached Parent
		AttachedParent = OtherActor;
		// Set Attached Socket Name
		AttachedSocketName = SocketName;
		// Set Attach Flag
		bIsAttached = true;
		// Disable Collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Disable Physics
		GetCapsuleComponent()->SetSimulatePhysics(false);
		// Set Actor Location to Socket Location
		SetActorLocation(SocketTransform.GetLocation());
		// Set Actor Rotation to Socket Rotation
		SetActorRotation(SocketTransform.GetRotation());
		// Schedule next move 
		GetWorldTimerManager().SetTimerForNextTick(	this, &AEnemyCharacter::MoveWithSocket);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket %s not found in %s"), *SocketName.ToString(), *GetName());
	}
}

void AEnemyCharacter::MoveWithSocket()
{
	// Find Socket Transform
	const FTransform SocketTransform = AttachedParent->GetMesh()->GetSocketTransform(AttachedSocketName);
	// Check if socket is valid
	if (SocketTransform.IsValid())
	{
		// Set Actor Location to Socket Location
		SetActorLocation(SocketTransform.GetLocation());
		// Set Actor Rotation to Socket Rotation
		SetActorRotation(SocketTransform.GetRotation());
		// If attach flag is true
		if (bIsAttached)
			// Schedule next move
			GetWorldTimerManager().SetTimerForNextTick(	this, &AEnemyCharacter::MoveWithSocket);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Socket %s not found in %s"), *GetAttachParentSocketName().ToString(), *GetName());
	}

}

void AEnemyCharacter::DetachEnemyFromSocket()
{
	// Set Attach Flag
	bIsAttached = false;
	// Enable Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

