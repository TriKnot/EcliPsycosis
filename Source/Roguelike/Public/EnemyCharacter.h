#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AAIController;

UCLASS()
class ROGUELIKE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

private:
	//** Move to Target */
	UFUNCTION(BlueprintCallable, Category = AI)
	void MoveToTargetLocation(FVector TargetLocation);

	//** Move to Other Character */
	UFUNCTION(BlueprintCallable, Category = AI)
	void MoveToCharacter(ACharacter* Character);

	//** Attack Other Character */
	UFUNCTION(BlueprintCallable, Category = AI)
	void AttackCharacter(ACharacter* Character);

	//** Initiate chase */
	UFUNCTION(BlueprintCallable, Category = AI)
	void ChaseCharacter(ACharacter* Character);

	//** OnChaseTriggerEnter */
	UFUNCTION(BlueprintCallable, Category = AI)
	void OnChaseTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when an instance of this is placed or spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//** AI Controller */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAIController> AIControllerBP;

	UPROPERTY()
	AAIController* AIController;

	//** Player Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* PlayerCharacter;
	
	//** Move Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	//** Health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float Health;

	//** Defense */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float Defense;

	//** Guard/Block Chance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float GuardChance;

	//** Attack Damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	int32 AttackDamage;

	//** Attack Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	//** Attack Range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	//** Attack Miss Chance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float AttackMissChance;

	//** Chase Range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float ChaseRangeRadius;

	// TODO: Add Phase state here when implemented

public:
	/** Waypoints */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Waypoints;

	/** Chase Trigger Volume */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ChaseTriggerSphere;
};
