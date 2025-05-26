// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return Tree; 
}

APatrolPath* ANPC::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* ANPC::GetMontage() const
{
	return Montage;
}

UAnimMontage* ANPC::GetDeathMontage() const
{
	return _enemyDeathMontage;
}

int ANPC::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}

	return 0;
}

int ANPC::Death_Implementation()
{

	if (bIsDead)
	{
		return 0;
	}

	bIsDead = true;

	
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (_enemyDeathMontage)
	{
		PlayAnimMontage(_enemyDeathMontage);

	}

	if (_spawnActorExp)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();


		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(_spawnActorExp, SpawnLocation, SpawnRotation, SpawnParams);
	}

	SpawnRandomItem();

	return 0;
}


void ANPC::DestroyNPC()
{
	AActor::Destroy();
}

void ANPC::SpawnRandomItem()
{
	TArray<int> ValidActors;
	for (int i = 0; i < _spawnItems.Num(); i++)
	{
		if (_spawnItems[i] && _spawnProbabilities.IsValidIndex(i))
		{
			float RandomValue = FMath::RandRange(0.0f, 1.0f); 
			if (RandomValue <= _spawnProbabilities[i])
			{
				ValidActors.Add(i);
			}
		}
	}

	if (ValidActors.Num() > 0)
	{
		int RandomIndex = FMath::RandRange(0, ValidActors.Num() - 1);
		int SelectedActorIndex = ValidActors[RandomIndex];

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(_spawnItems[SelectedActorIndex], SpawnLocation, SpawnRotation, SpawnParams);
	}
}
