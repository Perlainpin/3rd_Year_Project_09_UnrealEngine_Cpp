// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterController.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputMappingContext.h"
#include "InputAction.h"
#include <Kismet/KismetMathLibrary.h>
#include "Arrow.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MagicalArrow.h"
#include "Perception/AISense_Sight.h"
#include "ArboriaTopDownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterController::ACharacterController()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    _maximumScale = 3.0f;
    _rainCooldown = 3.0f;
	_rainCooldownTime = 3.0f;
    _triCooldown = 7.5f;
	_triCooldownTime = 7.5f;
    _dashCooldown = 2.0f;
    _damageBoost = 1.0f;
    _boostCooldown = 5.0f;
    _isRainOnCooldown = false;
    _isTriOnCooldown = false;
    _isHolding = false;
    _isDashOnCooldown = false;
    _isBoosted = false;

    GetCapsuleComponent()->InitCapsuleSize(34.f, 88.0f);

    // Set up Mesh (Character Mesh)
    GetMesh()->SetupAttachment(GetCapsuleComponent()); // Attach to root
    GetMesh()->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));

    // Load Skeletal Mesh Asset
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset2(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
    if (MeshAsset2.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MeshAsset2.Object); 
    }

    // Load Animation Blueprint
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Sparrow_AnimBlueprint"));
    if (AnimBP.Succeeded()) 
    {
        GetMesh()->SetAnimInstanceClass(AnimBP.Class); 
    }

    // Set up BowAnticipation (Make sure it's created)
    _bowAnticipation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowAnticipation"));
    if (_bowAnticipation) // Ensure it is valid before using
    {
        _bowAnticipation->SetupAttachment(GetCapsuleComponent()); // Attach to root
        _bowAnticipation->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
        _bowAnticipation->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
        _bowAnticipation->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.5f));
        _bowAnticipation->SetMobility(EComponentMobility::Movable);
        _bowAnticipation->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/SM_BowAnticipation.SM_BowAnticipation"));
        if (MeshAsset.Succeeded())
        {
            _bowAnticipation->SetStaticMesh(MeshAsset.Object);
        }

        static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Materials/M_BowAnticipation.M_BowAnticipation"));
        if (MaterialAsset.Succeeded())
        {
            _bowAnticipation->SetMaterial(0, MaterialAsset.Object);
        }
        _bowAnticipation->SetVisibility(false);
    }

    // Set up Spring Arm
    _springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    if (_springArm)
    {
        _springArm->SetupAttachment(GetCapsuleComponent());  
        _springArm->TargetArmLength = 500.0f;
        _springArm->SocketOffset = FVector(0.0f, 0.0f, 500.0f);
        _springArm->bUsePawnControlRotation = false; 
        _springArm->bInheritPitch = false;
        _springArm->bInheritYaw = false;
        _springArm->bInheritRoll = false;
    }

    // Set up Camera
    _camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    if (_camera)
    {
        _camera->SetupAttachment(_springArm); 
        _camera->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
        _camera->bUsePawnControlRotation = false; 
    }

    // Load Input Mapping Context
    _inputMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Character/IMC_Movements.IMC_Movements"));
    _moveAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_Move.IA_Move"));
    _attackAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_Attack.IA_Attack"));
    _loadUpAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_LoadUp.IA_LoadUp"));
    _ultimateAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_Ultimate.IA_Ultimate"));
    _spellAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_Spell.IA_Spell"));
    _dashAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Character/IA_Dash.IA_Dash"));

    _primaryFireAnimMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage"));
    _rainArrowAnimMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Q_Ability_Montage.Q_Ability_Montage"));
    _triArrowAnimMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Tri_Spell_Montage.Tri_Spell_Montage"));
    _playerDeathMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Death_Montage.Death_Montage"));
}

void ACharacterController::SetExp(float const NewExp)
{
}

// Called when the game starts or when spawned
void ACharacterController::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (InputSubsystem)
        {
            InputSubsystem->AddMappingContext(_inputMappingContext, 0);
        }
    }

    FString vfxPath = "Game/VFX/NS_DashTrail.NS_DashTrail"; 
    UNiagaraSystem* niagaraSystem = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *vfxPath));

    _niagaraDashVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(  
        niagaraSystem,
        RootComponent, 
        TEXT("DashVFX"), 
        FVector(0.0f, 0.0f, 0.0f), 
        FRotator(0.0f, 0.0f, 0.0f), 
        EAttachLocation::KeepRelativeOffset, 
        false
    );

    USceneComponent* FoundComponent = Cast<UNiagaraComponent>(GetComponentByName(this, "TrailVFX"));
    //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();

    if (FoundComponent) 
    {
        //FoundComponent->Deactivate(); 
        FoundComponent->SetVisibility(false);
    }

    //UNiagaraFunctionLibrary::SpawnSystemAttached(
    //    _lightningVFX,
    //    RootComponent,
    //    TEXT("LightningVFX"),
    //    FVector(0.0f, 0.0f, 0.0f),
    //    FRotator(0.0f, 0.0f, 0.0f),
    //    EAttachLocation::KeepRelativeOffset,
    //    false
    //);

    USceneComponent* FoundComponent2 = Cast<UNiagaraComponent>(GetComponentByName(this, "LightningVFX"));
    //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();

    if (FoundComponent2)
    {
        //FoundComponent->Deactivate();
        FoundComponent2->SetVisibility(false);
    }
}

void ACharacterController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime); 

    HandleCooldowns(DeltaTime);
}

// Called to bind functionality to input
void ACharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInput) return;

    if (_moveAction) EnhancedInput->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ACharacterController::Move);
    if (_attackAction) EnhancedInput->BindAction(_attackAction, ETriggerEvent::Triggered, this, &ACharacterController::Attack);
    if (_loadUpAction) EnhancedInput->BindAction(_loadUpAction, ETriggerEvent::Triggered, this, &ACharacterController::LoadAttack);
    if (_ultimateAction) EnhancedInput->BindAction(_ultimateAction, ETriggerEvent::Triggered, this, &ACharacterController::ArrowsRain);
    if (_spellAction) EnhancedInput->BindAction(_spellAction, ETriggerEvent::Triggered, this, &ACharacterController::TriAttack);
    if (_dashAction) EnhancedInput->BindAction(_dashAction, ETriggerEvent::Triggered, this, &ACharacterController::Dash);
}

void ACharacterController::Move(const FInputActionValue& Value)
{
    FVector2D MovementInput = Value.Get<FVector2D>();
    
    FVector newPos = FVector(MovementInput.X, MovementInput.Y, 0.0f);
    newPos.Normalize(0.0001f);

    FRotator newRotation = UKismetMathLibrary::MakeRotFromX(newPos);
    newRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), newRotation, FApp::GetDeltaTime(), 10.0f);
    SetActorRotation(newRotation);  
    AddMovementInput(newPos);
}

void ACharacterController::Attack(const FInputActionValue& Value)
{
    _bowAnticipation->SetVisibility(false); 
    _bowAnticipation->SetRelativeScale3D(FVector(0.7f, 1.0f, 1.0f));

    _isHolding = false;

    // Get the actor's forward vector, location, and rotation
    FVector actorLocation = GetActorLocation();
    FVector forwardVector = GetActorForwardVector(); 
    FRotator actorRotation = GetActorRotation(); 

    // Calculate spawn location by adding forward vector to actor location
    FVector spawnLocation = actorLocation + (forwardVector * 150.0f) + FVector(0.0f, 0.0f, 15.0f);

    // Spawn the projectile
    FActorSpawnParameters spawnParams;  
    spawnParams.Owner = this; 
    spawnParams.Instigator = GetInstigator(); 

    AArrow* spawnedArrow = GetWorld()->SpawnActor<AArrow>(
        AArrow::StaticClass(),
        spawnLocation,
        actorRotation, 
        spawnParams 
    );

    GetMesh()->GetAnimInstance()->Montage_Play(_primaryFireAnimMontage);
}

void ACharacterController::LoadAttack(const FInputActionValue& Value)
{
    _bowAnticipation->SetVisibility(true); 

    _isHolding = true; 

    FVector newScale = _bowAnticipation->GetRelativeScale3D();  

    newScale += FVector(GetWorld()->GetDeltaSeconds() * 5.0f, 0.0f, 0.0f); 

    float newXScale = newScale.X; 

    newXScale = FMath::Clamp(newXScale, 0.7f, _maximumScale); 

    newScale = FVector(newXScale, newScale.Y, newScale.Z);

    _bowAnticipation->SetRelativeScale3D(newScale);  
}

void ACharacterController::ArrowsRain(const FInputActionValue& Value)
{
    if (_isRainOnCooldown)
        return;

    GetMesh()->GetAnimInstance()->Montage_Play(_rainArrowAnimMontage);

    FTransform transform = GetActorTransform();

    FVector systemPos = transform.GetLocation() + GetActorForwardVector() * 75.0f + FVector(0.0f, 0.0f, 100.0f);

    FString niagaraPath = "/Game/VFX/NS_ArrowsRain.NS_ArrowsRain";

    UNiagaraSystem* niagaraSystem = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *niagaraPath));

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        niagaraSystem, 
        systemPos,    
        transform.GetRotation().Rotator(),
        FVector(1.0f)     
    );

    _rainCooldown = 3.0f;

    _isRainOnCooldown = true;
}

void ACharacterController::TriAttack(const FInputActionValue& Value)
{
    if (_isTriOnCooldown)
        return;

    GetMesh()->GetAnimInstance()->Montage_Play(_triArrowAnimMontage);

    // Get the actor's forward vector, right vector, location, and rotation
    FVector actorLocation = GetActorLocation();
    FVector forwardVector = GetActorForwardVector();
    FVector rightVector = GetActorRightVector(); 
    FRotator actorRotation = GetActorRotation();

    // Calculate spawn location for the center arrow
    FVector spawnLocation = actorLocation + (forwardVector * 200.0f) + FVector(0.0f, 0.0f, 15.0f);

    // Spawn the center projectile
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    spawnParams.Instigator = GetInstigator();

    GetWorld()->SpawnActor<AMagicalArrow>(
        AMagicalArrow::StaticClass(),
        spawnLocation,
        actorRotation,
        spawnParams
    );

    // Spawn the left and right arrows
    float sideOffset = 50.0f; 
    FVector leftSpawnLocation = spawnLocation - (rightVector * sideOffset);
    FVector rightSpawnLocation = spawnLocation + (rightVector * sideOffset);

    GetWorld()->SpawnActor<AMagicalArrow>(
        AMagicalArrow::StaticClass(),
        leftSpawnLocation,
        actorRotation - FRotator(0.0f, 20.0f, 0.0f),
        spawnParams
    );

    GetWorld()->SpawnActor<AMagicalArrow>(
        AMagicalArrow::StaticClass(),
        rightSpawnLocation,
        actorRotation + FRotator(0.0f, 20.0f, 0.0f),
        spawnParams
    );

    _triCooldown = 7.5f;
    _isTriOnCooldown = true;
}

void ACharacterController::Dash(const FInputActionValue& Value)
{
    if (_isDashOnCooldown)
        return;

    USceneComponent* FoundComponent = Cast<UNiagaraComponent>(GetComponentByName(this, "TrailVFX"));
    //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();

    if (FoundComponent)
    {
        //FoundComponent->Activate();
        FoundComponent->SetVisibility(true);
    }
    
    _isDashOnCooldown = true;
    _dashCooldown = 0.7f;

    FVector newVelocity = GetVelocity() * 10.0f;

    this->LaunchCharacter(newVelocity, false, false);
}

void ACharacterController::DoDeathAnimation()
{

    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    UE_LOG(LogTemp, Warning, TEXT("Player is dead 2"));
    GetMesh()->GetAnimInstance()->Montage_Play(_playerDeathMontage);
}

void ACharacterController::StartDamageBoost()
{
    _isBoosted = true;

    _damageBoost = 2.0f;

    _boostCooldown = 5.0f;

    USceneComponent* FoundComponent = Cast<UNiagaraComponent>(GetComponentByName(this, "LightningVFX"));
    //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();

    if (FoundComponent)
    {
        //FoundComponent->Activate();
        FoundComponent->SetVisibility(true);
    }
}

float ACharacterController::GetDamageBoost()
{
    return _damageBoost;
}

bool ACharacterController::GetPlayerStatue()
{
    return _isAlive; 
}

void ACharacterController::SetPlayerStatue(bool _newStatue)
{
    _isAlive = _newStatue; 
}

void ACharacterController::StopDamageBoost()
{
    _isBoosted = false;
    
    _damageBoost = 1.0f;

    USceneComponent* FoundComponent = Cast<UNiagaraComponent>(GetComponentByName(this, "LightningVFX"));
    //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();
    
    if (FoundComponent) 
    {
        //FoundComponent->Deactivate(); 
        FoundComponent->SetVisibility(false);
    }
}

UActorComponent* ACharacterController::GetComponentByName(AActor* Actor, FName ComponentName)
{
    if (!Actor) return nullptr;

    for (UActorComponent* Component : Actor->GetComponents())
    {
        if (Component && Component->GetFName() == ComponentName)
        {
            return Component;
        }
    }
    return nullptr;
}

void ACharacterController::SetupStimuliSource()
{
    _stimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
    if (_stimuliSource)
    {
        _stimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
        _stimuliSource->RegisterWithPerceptionSystem();
    }
}

void ACharacterController::HandleCooldowns(float deltaTime)
{
    if (_isBoosted)
    {
        _boostCooldown -= deltaTime;

        if (_boostCooldown <= 0.0f)
            StopDamageBoost();
    }

    if (_isRainOnCooldown)
    {
        _rainCooldown -= deltaTime;
		Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->ArrowsRainSlideRecharge((_rainCooldownTime - _rainCooldown) / _rainCooldownTime);

        if (_rainCooldown <= 0.0f)
            _isRainOnCooldown = false;
    }

    if (_isTriOnCooldown)
    {
        _triCooldown -= deltaTime;
        Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->TriSpellSlideRecharge((_triCooldownTime - _triCooldown) / _triCooldownTime);

        if (_triCooldown <= 0.0f)
            _isTriOnCooldown = false;
    }

    if (_isDashOnCooldown)
    {
        _dashCooldown -= deltaTime;

        if (_dashCooldown <= 0.0f)
        {
            _isDashOnCooldown = false;

            USceneComponent* FoundComponent = Cast<UNiagaraComponent>(GetComponentByName(this, "TrailVFX"));
            //USceneComponent* FoundComponent = FindComponentByClass<UNiagaraComponent>();

            if (FoundComponent)
            {
                //FoundComponent->Deactivate();
                FoundComponent->SetVisibility(false);
            }
        }
    }
}
