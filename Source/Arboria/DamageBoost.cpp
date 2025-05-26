// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBoost.h"
#include "CharacterController.h"

ADamageBoost::ADamageBoost() : APickableItem()
{
}

void ADamageBoost::BeginPlay()
{
	Super::BeginPlay();
}

void ADamageBoost::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADamageBoost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamageBoost::OnPicked(AActor* OtherActor)
{
	ACharacterController* myCharacter = Cast<ACharacterController>(OtherActor);

	if (myCharacter)
	{
		myCharacter->StartDamageBoost();
	}
}

void ADamageBoost::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		OnPicked(OtherActor);

		Destroy();
	}
}
