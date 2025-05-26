// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PatrolPath.h"
#include "CombatInterface.h"
#include "Animation/AnimMontage.h"
#include "CPPAICharacterBase.h"
#include "TimerManager.h"
#include "NPC.generated.h"


UCLASS()
class ARBORIA_API ANPC : public ACPPAICharacterBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

	UAnimMontage* GetMontage() const;

	UAnimMontage* GetDeathMontage() const;


	int MeleeAttack_Implementation() override;
	int Death_Implementation() override;

	void DestroyNPC();

	void SpawnRandomItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* _enemyDeathMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawn", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> _spawnActorExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawn", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> _spawnItems;

	UPROPERTY(EditAnywhere, Category = "spawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	TArray<float> _spawnProbabilities;


private : 
	bool bIsDead = false;

};
