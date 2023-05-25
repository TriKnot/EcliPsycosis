#include "EnemyCharacter.h"

#include "AIController.h"
#include "HurtBox.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapons/Core/MeleeComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Detection Trigger Sphere
	DetectionTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectTriggerSphere"));
	DetectionTriggerSphere->SetupAttachment(RootComponent);
	DetectionTriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnDetectTriggerEnter);

	// Hurt Box
	HurtBox = CreateDefaultSubobject<UHurtBox>(TEXT("HurtBox"));
	HurtBox->SetupAttachment(RootComponent);

	// Melee Component
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));

}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache Player Character
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

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
		UE_LOG( LogTemp, Warning, TEXT("Setting Detection Trigger Sphere Radius") );
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
	UE_LOG(LogTemp, Error, TEXT("Attacking"));
}

void AEnemyCharacter::ReceiveDamage(float _InDamage)
{
	Health -= _InDamage;
	UE_LOG(LogTemp, Error, TEXT("Enemy Health: %f"), Health);
	if (Health <= 0.0f)
	{
		EnemyDeath();
	}
}

void AEnemyCharacter::EnemyDeath()
{
	Destroy();
}


