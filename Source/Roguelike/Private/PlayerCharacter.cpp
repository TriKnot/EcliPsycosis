// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach camera to end of boom
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create Hit Box
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetMesh());

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
}

APlayerCharacter::~APlayerCharacter()
{
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Save max movement speed set in editor
	MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	// Save Acceleration Speed set in editor
	AccelerationSpeed = GetCharacterMovement()->MaxAcceleration;

	// Enable Character Movement and Actions
	bCanMove = true;
	bCanDoAction = true;
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Clear all timers
	if(DashTimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(DashTimerHandle);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::MoveForward(float _InValue)
{
	if(bCanMove)
		AddMovementInput(FVector(CameraComponent->GetForwardVector().X, CameraComponent->GetForwardVector().Y, 0.0f)*_InValue);
}

void APlayerCharacter::MoveRight(float _InValue)
{
	if(bCanMove)
		AddMovementInput(FVector(CameraComponent->GetRightVector().X, CameraComponent->GetRightVector().Y, 0.0f)*_InValue);
}

void APlayerCharacter::Dash()
{
	// Check if character can move or do actions
	if( !bCanMove || !bCanDoAction )
		return;
	
	// Disable Character Movement and Actions
	bCanMove = false;
	bCanDoAction = false;

	// Calculate offset in z axis to make it go from center of hitbox
	const FVector ZOffset = FVector(0.0f, 0.0f, HitBox->GetScaledBoxExtent().Z);
	
	// Calculate Target Destination
	FVector TargetDestination = GetActorLocation() + GetActorForwardVector() * DashDistance + ZOffset;

	// Check if anything is blocking the dash
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation() + ZOffset, TargetDestination, ECC_Visibility, CollisionParams);

	// If something is blocking the dash, set Target Destination to HitResult
	if( HitResult.bBlockingHit )
	{
		// Set Target Destination to HitResult
		TargetDestination = HitResult.ImpactPoint;
		UE_LOG( LogTemp, Warning, TEXT("Hit something recalculating target destination.") );
	}

	// Uncap Movement Acceleration Speed
	GetCharacterMovement()->MaxAcceleration = 1000000.0f;
	// Set Max Movement Speed to Dash Speed
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	
	// Execute dash 
	ExecuteDash(TargetDestination);
}

// Maybe this should be done recursively every frame instead on a timer.
void APlayerCharacter::ExecuteDash(FVector TargetDestination)
{
	// Calculate Dash Direction
	FVector DashDirection = TargetDestination - GetActorLocation();
	DashDirection.Normalize();
	// Get time since last frame
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	// Calculate Dash Velocity
	FVector DashTickDeltaMove = DashDirection * DashSpeed * DeltaTime;
	
	// Check if something static is blocking the dash
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	// Calculate Dash Start Location (Starts at edge of hitbox in front of character)
	FVector DashStartLocation = GetActorLocation() + GetActorForwardVector() * HitBox->GetScaledBoxExtent().X;
	// Calculate Dash Tick End Location
	FVector DashTickEndLocation = DashStartLocation + DashTickDeltaMove;

	// End early if something is blocking the dash
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, DashStartLocation, DashTickEndLocation, ECC_WorldStatic, CollisionParams);
	if( bHit )
	{
		// Add movement up to HitResult
		AddMovementInput(HitResult.ImpactPoint - GetActorLocation());
		// End Dash
		EndDash();
		return;
	}

	// Check if Dash is complete and end recursion if true
	FVector _Target = TargetDestination;
	_Target.Z = 0;
	FVector _Actor = GetActorLocation();
	_Actor.Z = 0;
	if (FVector::Dist(_Actor, _Target) < DashTickDeltaMove.Size())
	{
		// Add movement up to Target Destination
		DashTickDeltaMove = TargetDestination - GetActorLocation();
		AddMovementInput(DashTickDeltaMove);
		// End Dash
		EndDash();
		return;
	}

	// Move Character
	AddMovementInput(DashTickDeltaMove);

	// Schedule again for next frame if not complete
	FTimerDelegate DashTimerDelegate;
	DashTimerDelegate.BindUFunction(this, FName("ExecuteDash"), TargetDestination);
	GetWorld()->GetTimerManager().SetTimerForNextTick( DashTimerDelegate );
		
}

void APlayerCharacter::EndDash()
{
	// Unlock Character Movement and Actions
	bCanMove = true;
	bCanDoAction = true;
	
	// Stop Timer
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	// Reset Acceleration Speed
	GetCharacterMovement()->MaxAcceleration = AccelerationSpeed;
	// Reset Max Movement Speed
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	// Slow Character to new move speed cap immediately
	GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity.GetSafeNormal() * MoveSpeed;
}

void APlayerCharacter::Interact()
{
}

void APlayerCharacter::LightAttack()
{

}

void APlayerCharacter::HeavyAttack()
{

}

void APlayerCharacter::Pause()
{

}

void APlayerCharacter::WeaponAbility()
{

}

