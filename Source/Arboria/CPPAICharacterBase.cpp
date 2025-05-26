// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPAICharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "HealthBarWidget.h"	
#include "NPC.h"
#include "Arrow.h"
#include "ArboriaTopDownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterController.h"

// Sets default values
ACPPAICharacterBase::ACPPAICharacterBase() :
	WidgetComponent{ CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")) },
	Health{ MaxHealth },
	RightFistCollisionBox{ CreateDefaultSubobject<UBoxComponent>(TEXT("RightFirstCollisionBox")) }
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (WidgetComponent)
	{
		WidgetComponent->SetupAttachment(RootComponent);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetRelativeLocation(FVector{ 0.0f, 0.0f, 95.0f });
		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/UI/BP_HealthBar") };
		if (WidgetClass.Succeeded())
		{
			WidgetComponent->SetWidgetClass(WidgetClass.Class);
		}
	}

	if (RightFistCollisionBox)
	{
		RightFistCollisionBox->SetBoxExtent(FVector{ 5.f,5.f ,5.f }, false);
		FAttachmentTransformRules const Rules{
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			false
		};

		RightFistCollisionBox->AttachToComponent(GetMesh(), Rules, "weapon_r");
		RightFistCollisionBox->SetRelativeLocation({ -7.f, 0.f, 0.f });
	}
}

// Called when the game starts or when spawned
void ACPPAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPPAICharacterBase::OnAttackOverlapBegin);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPPAICharacterBase::OnAttackOverlapEnd);
	
}

void ACPPAICharacterBase::OnAttackOverlapBegin(
	UPrimitiveComponent* const OverlappedComponent,
	AActor* const OtherActor,
	UPrimitiveComponent* const OtherCommponent,
	int const OtherBodyIndex, bool const FromSweep,
	FHitResult const& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}
	if (auto const Enemy = Cast<ANPC>(OtherActor))
	{
		auto const NewHealth = Enemy->GetHealth() - Enemy->GetMaxHealth()* 0.1f;
		Enemy->SetHealth(NewHealth);
	}

	else if (auto const Player = Cast<ACharacterController>(OtherActor))
	{
		auto const NewHealth = Player->GetHealth() - Player->GetMaxHealth() * 0.07f;
		Player->SetHealth(NewHealth);
		Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->TakeDamage(NewHealth);
	}

}

void ACPPAICharacterBase::OnAttackOverlapEnd(
	UPrimitiveComponent* const OverlappedComponent,
	AActor* const OtherActor,
	UPrimitiveComponent* OtherCommponent,
	int const OtherBodyIndex)
{

}

// Called every frame
void ACPPAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (auto const widget = Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		widget->SetHealthBarPercentage(Health / MaxHealth);
	}
}

// Called to bind functionality to input
void ACPPAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACPPAICharacterBase::GetHealth() const
{
	return Health;
}

float ACPPAICharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

void ACPPAICharacterBase::SetHealth(float const NewHealth)
{
	Health = NewHealth;
	if (auto Player = Cast<ACharacterController>(this))
	{
		if (Health <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
			Player->DoDeathAnimation();
			Player->SetPlayerStatue(false); 
		}
	}
	else if (auto Enemy = Cast<ANPC>(this))
	{
		if (Health <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy is dead"));
			Enemy->Death_Implementation();
		
		}
	}
}

void ACPPAICharacterBase::AttackStart() const
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ACPPAICharacterBase::AttackEnd() const
{
	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

