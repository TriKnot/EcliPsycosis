#include "EnemyCharacter.h"
#include "Damage/DamageEffect.h"
#include "HurtBox.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Core/MeleeComponent.h"
#include "Weapons/Core/RangedComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Detection Trigger Sphere
	// DetectionTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectTriggerSphere"));
	// DetectionTriggerSphere->SetupAttachment(GetMesh());
	// DetectionTriggerSphere->InitSphereRadius(1500.f);
	// DetectionTriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// DetectionTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnDetectTriggerEnter);

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
	Health = MaxHealth;

}

void AEnemyCharacter::OnDetectTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Exit if not Player Character
	if (OtherActor != PlayerCharacter)
		return;
	OnDetectTriggerOverlap.Broadcast();
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
	UE_LOG(LogTemp, Warning, TEXT("Health Damage received"));
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
	if(GEngine)
	{
		const FString Text = FString::Printf(TEXT("Changing %s health by %f to %f"), *GetName(), _InHealth, Health);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, Text);
	}
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

	UE_LOG( LogTemp, Warning, TEXT("JumpVelocity: %s"), *JumpVelocity.ToString() );

	LaunchCharacter(JumpVelocity, true, true);
}
