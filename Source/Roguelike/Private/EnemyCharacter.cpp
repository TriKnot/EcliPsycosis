#include "EnemyCharacter.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Chase Trigger Sphere
	ChaseTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ChaseTriggerSphere"));
	ChaseTriggerSphere->SetupAttachment(RootComponent);
	ChaseTriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	ChaseTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnChaseTriggerEnter);


}

void AEnemyCharacter::MoveToTargetLocation(FVector TargetLocation)
{
	AIController->MoveToLocation(TargetLocation, 5.0f, true, true, true);
}

void AEnemyCharacter::MoveToCharacter(ACharacter* Character)
{
	AIController->MoveToActor(Character, 5.0f, true, true, true);
}

void AEnemyCharacter::AttackCharacter(ACharacter* Character)
{
	UE_LOG( LogTemp, Warning, TEXT("AttackCharacter"));
}

void AEnemyCharacter::ChaseCharacter(ACharacter* Character)
{
	// Tell AI Controller to chase Character
	AIController->MoveToActor(Character, 5.0f, true, true, true);
}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache Player Character
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Get AI Controller
	AIController = Cast<AAIController>(GetController());
	

}

void AEnemyCharacter::OnChaseTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Exit if not Player Character
	if (OtherActor != PlayerCharacter)
		return;

	ChaseCharacter( PlayerCharacter );

}

void AEnemyCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Set Chase Trigger Sphere Radius
	ChaseTriggerSphere->SetSphereRadius(ChaseRangeRadius);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

