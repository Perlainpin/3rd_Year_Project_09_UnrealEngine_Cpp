// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPack.h"
#include "CPPAICharacterBase.h"
#include "ArboriaTopDownGameMode.h"
#include "Kismet/GameplayStatics.h"

AHealthPack::AHealthPack() : APickableItem()
{
}

void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
}

void AHealthPack::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthPack::OnPicked(AActor* OtherActor)
{
	ACPPAICharacterBase* myCharacter = Cast<ACPPAICharacterBase>(OtherActor);

	if (myCharacter)
	{
		float health = myCharacter->GetHealth();

		health += 20.0f;

		health = FMath::Clamp(health, 0.0f, myCharacter->GetMaxHealth());

		myCharacter->SetHealth(health);
		Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->UseHeal(20.f);
	}
}

void AHealthPack::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		OnPicked(OtherActor);

		Destroy();
	}
}