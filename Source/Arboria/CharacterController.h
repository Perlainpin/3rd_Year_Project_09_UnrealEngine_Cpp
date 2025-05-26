// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Animation/AnimMontage.h"
#include "CPPAICharacterBase.h"
#include "CharacterController.generated.h"

UCLASS()
class ARBORIA_API ACharacterController : public ACPPAICharacterBase
{
	GENERATED_BODY()

	UPROPERTY()
	float _maximumScale;

	UPROPERTY()
	float _rainCooldown;

	UPROPERTY()
	float _triCooldown;

    UPROPERTY()
    float _rainCooldownTime;

    UPROPERTY()
    float _triCooldownTime;

    UPROPERTY()
    float _dashCooldown;

	UPROPERTY()
	bool _isRainOnCooldown;

	UPROPERTY()
	bool _isTriOnCooldown;

    UPROPERTY()
    bool _isHolding;

    UPROPERTY()
    bool _isDashOnCooldown;

    UPROPERTY()
    float _expPoint;

    UPROPERTY()
    float _damageBoost;

    UPROPERTY()
    float _boostCooldown;

    UPROPERTY()
    bool _isBoosted;

    UPROPERTY()
    bool _isAlive = true; 

	UPROPERTY()
	UCapsuleComponent* _capsuleComponent;

	UPROPERTY()
	UStaticMeshComponent* _bowAnticipation;

    UPROPERTY()
    UNiagaraComponent* _niagaraDashVFX;

	UPROPERTY()
	UCameraComponent* _camera;

    UPROPERTY()
    UAnimMontage* _primaryFireAnimMontage;

    UPROPERTY() 
    UAnimMontage* _rainArrowAnimMontage; 

    UPROPERTY() 
    UAnimMontage* _triArrowAnimMontage; 

    UPROPERTY()
    UAnimMontage* _playerDeathMontage;

public:
    ACharacterController();

    float GetExp() const;
    float GetMaxEp() const;
    void SetExp(float const NewExp);

    UFUNCTION()
    void DoDeathAnimation();

    UFUNCTION()
    void StartDamageBoost();

    UFUNCTION() 
    float GetDamageBoost();

	UFUNCTION()
    bool GetPlayerStatue();


    void SetPlayerStatue(bool _newStatue); 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* _lightningVFX;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);
    void LoadAttack(const FInputActionValue& Value);
    void ArrowsRain(const FInputActionValue& Value);
    void TriAttack(const FInputActionValue& Value);
    void Dash(const FInputActionValue& Value);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USpringArmComponent* _springArm;

private:
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* _inputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* _moveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* _attackAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* _loadUpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* _ultimateAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* _spellAction;

    UPROPERTY(EditAnywhere, Category = "Input") 
    class UInputAction* _dashAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess="true"))
    UAnimMontage* Montage; 


    class UAIPerceptionStimuliSourceComponent* _stimuliSource;

    void SetupStimuliSource();

    UFUNCTION()
    void HandleCooldowns(float deltaTime);

    UFUNCTION()
    void StopDamageBoost();

    UFUNCTION()
    UActorComponent* GetComponentByName(AActor* Actor, FName ComponentName);
};

