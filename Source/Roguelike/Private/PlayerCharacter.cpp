// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"
#include "HurtBox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/Core/MeleeComponent.h"
#include "Weapons/Core/TargetAssist.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/EffectController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Core/RangedComponent.h"

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
	HurtBox = CreateDefaultSubobject<UHurtBox>(TEXT("HurtBox"));
	HurtBox->OnReceivedDamage.AddDynamic(this, &APlayerCharacter::ReceiveDamage);
	HurtBox->SetupAttachment(RootComponent);

	// Create MeleeComponent
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));

	// Create RangedComponent
	RangedComponent = CreateDefaultSubobject<URangedComponent>(TEXT("RangedComponent"));

	//Create TargetAssist
	TargetAssist = CreateDefaultSubobject<UTargetAssist>(TEXT("TargetAssist"));
	// Create Box Positions for Dynamic Box Movement
	LightAttackBoxPos = CreateDefaultSubobject<USceneComponent>(TEXT("LightAttackBoxPos"));
	LightAttackBoxPos->SetupAttachment(GetMesh());
	HeavyAttackBoxPos = CreateDefaultSubobject<USceneComponent>(TEXT("HeavyAttackBoxPos"));
	HeavyAttackBoxPos->SetupAttachment(GetMesh());
	WeaponAbilityBoxPos = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAbilityBoxPos"));
	WeaponAbilityBoxPos->SetupAttachment(GetMesh());

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

	// Bind Overlap Delegate for Pickup
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OverlapBegin);

	//Set Default Health
	Health = MaxHealth;
	
	// Enable Character Movement and Actions
	bCanMove = true;
	bCanDoAction = true;
	bCanPickup = true;
	bCanDash = true;

	//Binding the Attack
	MeleeComponent->OnAttackStateChanged.AddDynamic(this, &APlayerCharacter::SetterCanDoAction);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Clear all timers
	if (DashFrameTimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(DashFrameTimerHandle);
	if(DashEndTimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(DashEndTimerHandle);
	if(DashCooldownTimerHandle.IsValid())
		GetWorldTimerManager().ClearTimer(DashCooldownTimerHandle);
	if (ItemEffectHandle.IsValid())
		GetWorldTimerManager().ClearTimer(ItemEffectHandle);

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
	if (bCanMove)
		AddMovementInput(FVector(CameraComponent->GetForwardVector().X, CameraComponent->GetForwardVector().Y, 0.0f) * _InValue);
}

void APlayerCharacter::MoveRight(float _InValue)
{
	if (bCanMove)
		AddMovementInput(FVector(CameraComponent->GetRightVector().X, CameraComponent->GetRightVector().Y, 0.0f) * _InValue);
}

void APlayerCharacter::Dash()
{
	// Check if character can move or do actions
	if (!bCanDash || !bCanMove || !bCanDoAction)
	{
		UE_LOG( LogTemp, Warning, TEXT("Can't Dash") );
		return;
	}

	// Disable Character Movement and Actions
	bCanMove = false;
	bCanDoAction = false;

	// Calculate Target Destination
	FVector TargetDestination = GetActorLocation() + GetActorForwardVector() * DashDistance;

	// Check if anything is blocking the dash on ground level
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), TargetDestination, ECC_WorldStatic, CollisionParams);
	
	// If something is blocking the dash, set Target Destination to HitResult
	if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
	{
		//Check if the hit actor is an enemy
		if(!HitResult.GetActor()->IsA(AEnemyCharacter::StaticClass()))
		{
			// Set Target Destination to HitResult
			TargetDestination = HitResult.ImpactPoint;
			UE_LOG(LogTemp, Warning, TEXT("Hit something of class %s -> recalculating target destination."), *HitResult.GetActor()->StaticClass()->GetName());
		}
	}

	// Uncap Movement Acceleration Speed
	GetCharacterMovement()->MaxAcceleration = 1000000.0f;
	// Set Max Movement Speed to Dash Speed
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

	//Call the Dash State Changed
	OnDashStateChanged.Broadcast(true);

	// Execute dash
	ExecuteDashDelegate.BindUFunction(this, FName("ExecuteDash"), TargetDestination);
	GetWorld()->GetTimerManager().SetTimerForNextTick(ExecuteDashDelegate);

	// Set timer to end dash if it hasn't ended yet (in case of collision for example)
	float DashMaxDuration = DashDistance / DashSpeed + GetWorld()->GetDeltaSeconds();
	GetWorldTimerManager().SetTimer(DashEndTimerHandle, this, &APlayerCharacter::EndDash, DashMaxDuration, false);
}

void APlayerCharacter::ExecuteDash(FVector TargetDestination)
{

	// Calculate Dash Direction
	FVector DashDirection = TargetDestination - GetActorLocation();
	DashDirection.Normalize();
	// Get time since last frame
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	// Calculate Dash Velocity
	FVector DashTickDeltaMove = DashDirection * DashSpeed * DeltaTime;

	// Move Character
	FHitResult HitResult;
	AddActorWorldOffset(DashTickDeltaMove, true, &HitResult, ETeleportType::TeleportPhysics);

	// Check if enemy is hit and force through any collision and do any enemy specific logic
	if(HitResult.bBlockingHit && HitResult.GetActor() != nullptr && HitResult.GetActor()->IsA(AEnemyCharacter::StaticClass()))
	{
		//Force the character through the enemy
		SetActorLocation(GetActorLocation() + DashTickDeltaMove);
		// TODO: Maybe damage here?
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
	
	DashFrameTimerHandle = GetWorld()->GetTimerManager().SetTimerForNextTick(ExecuteDashDelegate);	
}

void APlayerCharacter::EndDash()
{
	// Unlock Character Movement and Actions
	bCanMove = true;
	bCanDoAction = true;

	// Stop Timers
	GetWorldTimerManager().ClearTimer(DashFrameTimerHandle);
	GetWorldTimerManager().ClearTimer(DashEndTimerHandle);
	// Reset Acceleration Speed
	GetCharacterMovement()->MaxAcceleration = AccelerationSpeed;
	// Reset Max Movement Speed
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * ModifierSet.SpeedModifier;
	// Slow Character to new move speed cap immediately
	GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity.GetSafeNormal() * MoveSpeed;
	//Call the Dash State Changed
	OnDashStateChanged.Broadcast(false);

	// Start Dash Cooldown with lambda in timer
	bCanDash = false;
	GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, [this]() { bCanDash = true; }, DashCooldown, false);
	
}

void APlayerCharacter::Interact()
{
}

void APlayerCharacter::LightAttack()
{
	if (bCanDoAction)
	{
		switch (CurrentWeapon)
		{
		case EWeaponMode::Melee:
			SetDamageBoxPos(LightAttackBoxPos);
			SetAssistRotation(TargetAssist->CalculateTargetActor());
			MeleeComponent->LightAttack();
			break;
		case EWeaponMode::Ranged:
			RangedComponent->LightAttack();
		}
		
	}
}

void APlayerCharacter::HeavyAttack()
{
	if (bCanDoAction)
	{
		switch (CurrentWeapon)
		{
		case EWeaponMode::Melee:
			SetDamageBoxPos(HeavyAttackBoxPos);
			SetAssistRotation(TargetAssist->CalculateTargetActor());
			MeleeComponent->HeavyAttack();
			break;
		case EWeaponMode::Ranged:
			RangedComponent->HeavyAttack();
		}

	}
}

void APlayerCharacter::WeaponAbility()
{
	if (bCanDoAction)
	{
		switch (CurrentWeapon)
		{
		case EWeaponMode::Melee:
			SetDamageBoxPos(WeaponAbilityBoxPos);
			SetAssistRotation(TargetAssist->CalculateTargetActor());
			MeleeComponent->WeaponAbility();
			break;
		case EWeaponMode::Ranged:
			RangedComponent->WeaponAbility();
		}

	}
}

void APlayerCharacter::ReceiveDamage(float _InDamage, FAttackEffect _EffectType)
{
	AddHealth( -_InDamage );
	if (Health <= 0.0f)
	{
		PlayerDeath();
	}
}

void APlayerCharacter::PlayerDeath() const
{
	if (!MainMenuLevel.ToString().IsEmpty())
	{
		OnPlayerDeath.Broadcast();
		UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevel);
	}
}

void APlayerCharacter::ImplementModifier(FModifierSet _InSet, const TScriptInterface<IPickupItem>& Item)
{
	//Add the New Modifier Set To Our Collection
	UEffectController* _temp = NewObject<UEffectController>(this);
	_temp->Init(_InSet, this);
	_temp->OnEffectComplete.AddDynamic(this, &APlayerCharacter::ClearModifier);
	ItemPicked(_temp, Item);
	ModSets.Add(_temp);

	//Consolidate the Modifier Effect
	ConsolidateModifier();

	//Send the Data to Create Widget
	AddHealth(Health * _InSet.HPModifier);
}

void APlayerCharacter::ConsolidateModifier()
{
	ModifierSet = FModifierSet::ZERO();
	for (const auto Mod : ModSets)
	{
		ModifierSet.SpeedModifier += Mod->SpeedModifier;
		ModifierSet.LightAttackModifier += Mod->LightAttackModifier;
		ModifierSet.HeavyAttackModifier += Mod->HeavyAttackModifier;
		ModifierSet.WeaponAbilityModifier += Mod->WeaponAbilityModifier;
		if (Mod->LightAttackEffect != EEffectTypes::AE_None)
		{
			ModifierSet.LightAttackEffect = Mod->LightAttackEffect;
			ModifierSet.DamageOverTime = Mod->DamageOverTime;
			ModifierSet.AttackEffectTime = Mod->AttackEffectTime;
		}
		if (Mod->HeavyAttackEffect != EEffectTypes::AE_None)
			ModifierSet.HeavyAttackEffect = Mod->HeavyAttackEffect;
		if (Mod->WeaponAbilityEffect != EEffectTypes::AE_None)
			ModifierSet.WeaponAbilityEffect = Mod->WeaponAbilityEffect;
		
	}
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * ModifierSet.SpeedModifier;
	MeleeComponent->LightAttackModifier = ModifierSet.LightAttackModifier;
	MeleeComponent->LightAttackEffect = ModifierSet.LightAttackEffect;
	MeleeComponent->HeavyAttackModifier = ModifierSet.HeavyAttackModifier;
	MeleeComponent->HeavyAttackEffect = ModifierSet.HeavyAttackEffect;
	MeleeComponent->WeaponAbilityModifier = ModifierSet.WeaponAbilityModifier;
	MeleeComponent->WeaponAbilityEffect = ModifierSet.WeaponAbilityEffect;
	MeleeComponent->DamageOverTime = ModifierSet.DamageOverTime;
	MeleeComponent->AttackEffectTime = ModifierSet.AttackEffectTime;
}

void APlayerCharacter::ClearModifier(UEffectController* _OutgoingController)
{
	//Remove the Specific Modifier
	ModSets.Remove(_OutgoingController);
	ConsolidateModifier();
}

void APlayerCharacter::AddHealth(float _InHealth)
{
	Health = FMath::Clamp(Health += _InHealth, 0.0f, MaxHealth);
	
	if(GEngine)
	{
		const FString Text = FString::Printf(TEXT("Changing Player health by %f to %f"), _InHealth, Health);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, Text);
	}
}

void APlayerCharacter::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanPickup)
		return;
	IPickupItem* _Temp = Cast<IPickupItem>(OtherActor);
	if (!_Temp)
		return;
	ImplementModifier(_Temp->GetModifiers(), TScriptInterface<IPickupItem>(OtherActor));
	_Temp->Pickup();
}

void APlayerCharacter::SetAssistRotation(FRotator _InRotation)
{
	if(_InRotation != FRotator::ZeroRotator)
	{
		FRotator _Current = GetActorRotation();
		_Current.Yaw = _InRotation.Yaw;
		SetActorRotation(_Current, ETeleportType::TeleportPhysics);
	}
}

float APlayerCharacter::GetRemainingItemTime() const
{
	if (ItemEffectHandle.IsValid())
		return GetWorldTimerManager().GetTimerRemaining(ItemEffectHandle);
	else
		return 0.0f;
}