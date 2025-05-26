// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <NiagaraComponent.h>
#include "PickableItem.generated.h"

UCLASS()
class ARBORIA_API APickableItem : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	float _relativeZPos;

	UPROPERTY()
	UBoxComponent* _collisionBox; 

	UPROPERTY()
	UNiagaraComponent* _niagaraComponent;
	
public:	
	// Sets default values for this actor's properties
	APickableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Caracteristics")
	float _floatingSpeed = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Caracteristics")
	int _floatingDistance = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* _objectMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* _vfx;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void Animation(float deltaTime);

	UPROPERTY()
	UStaticMeshComponent* _object;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnPicked(AActor* OtherActor);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
