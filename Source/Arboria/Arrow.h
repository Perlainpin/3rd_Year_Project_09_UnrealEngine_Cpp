// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class ARBORIA_API AArrow : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* _sceneComponent;

	UPROPERTY()
	UStaticMeshComponent* _object;
public:	
	// Sets default values for this actor's properties
	AArrow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _speed = 50.0f;

	void AttackStart() const;
	void AttackEnd() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Move(float deltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess="true"))
	class UCapsuleComponent* _ArrowCollision;


private : 

	UFUNCTION()
	void OnArrowHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

};
