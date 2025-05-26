// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArboriaGameStateBase.h"
#include "GateKey.generated.h"

UCLASS()
class ARBORIA_API AGateKey : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGateKey();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	AArboriaGameStateBase* MyGameState;

};
