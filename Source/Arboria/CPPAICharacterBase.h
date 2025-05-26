// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPPAICharacterBase.generated.h"

UCLASS()
class ARBORIA_API ACPPAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPAICharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);

	void AttackStart() const;
	void AttackEnd() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* RightFistCollisionBox;

private:
	UPROPERTY()
	class UWidgetComponent* WidgetComponent;
	float const MaxHealth{ 100.0f };
	float Health;

	UFUNCTION()
	void OnAttackOverlapBegin(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* const OtherCommponent,
		int const OtherBodyIndex,
		bool const FromSweep,
		FHitResult const& SweepResult
	); 


	UFUNCTION()
	void OnAttackOverlapEnd(
		UPrimitiveComponent* const OverlappedComponent,
		AActor* const OtherActor,
		UPrimitiveComponent* OtherCommponent,
		int const OtherBodyIndex);

};
