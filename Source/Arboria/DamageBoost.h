// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItem.h"
#include "DamageBoost.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API ADamageBoost : public APickableItem
{
	GENERATED_BODY()
	
public:
	ADamageBoost();

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:

	virtual void Tick(float DeltaTime) override;

	void OnPicked(AActor* OtherActor) override;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
