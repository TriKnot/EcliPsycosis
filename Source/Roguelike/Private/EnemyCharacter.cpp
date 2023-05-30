#include "EnemyCharacter.h"

#include "AIController.h"
#include "HurtBox.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapons/Core/MeleeComponent.h"
#include "Weapons/Core/RangedComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Detection Trigger Sphere
	DetectionTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectTriggerSphere"));
	DetectionTriggerSphere->SetupAttachment(GetMesh());
	DetectionTriggerSphere->InitSphereRadius(1500.f);
	DetectionTriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnDetectTriggerEnter);

	// Hurt Box
	HurtBox = CreateDefaultSubobject<UHurtBox>(TEXT("HurtBox"));
	HurtBox->OnReceivedDamage.AddDynamic(this, &AEnemyCharacter::ReceiveDamage);
	HurtBox->SetupAttachment(RootComponent);

	// Melee Component
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));

	// Ranged Component
	RangedComponent = CreateDefaultSubobject<URangedComponent>(TEXT("RangedComponent"));

}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache Player Character
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

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

void AEnemyCharacter::Attack()
{
	// Initialize attack here
	MeleeComponent->LightAttack();
}

void AEnemyCharacter::ReceiveDamage(float _InDamage)
{
	AddHealth(-_InDamage);
	if (Health <= 0.0f)
	{
		EnemyDeath();
	}
}

void AEnemyCharacter::EnemyDeath()
{
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
