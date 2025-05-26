// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicalArrow.h"
#include <Components/BoxComponent.h>
#include "CPPAICharacterBase.h"
#include "CharacterController.h"
#include <Kismet/GameplayStatics.h>

AMagicalArrow::AMagicalArrow() : _MagicalArrowCollision{ CreateDefaultSubobject<UBoxComponent>(TEXT("MagicalArrowCollision")) }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	_sceneComponent->SetRelativeLocation(FVector::ZeroVector); // Ensure it's at the actor's origin
	RootComponent = _sceneComponent;

	_object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	_object->SetupAttachment(_sceneComponent);
	_object->SetVisibility(true);
	_object->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_object->SetCollisionObjectType(ECC_WorldDynamic);
	_object->SetCollisionResponseToAllChannels(ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assets/SM_MagicalArrow.SM_MagicalArrow'")); // Ensure correct path

	_MagicalArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AMagicalArrow::OnMagicalArrowHit);

	_MagicalArrowCollision->SetBoxExtent(FVector{ 5.f,5.f ,5.f }, false);
	_MagicalArrowCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_MagicalArrowCollision->SetCollisionObjectType(ECC_WorldDynamic);
	_MagicalArrowCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	_MagicalArrowCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	_MagicalArrowCollision->SetupAttachment(RootComponent);

	if (MeshAsset.Succeeded())
	{
		_object->SetStaticMesh(MeshAsset.Object);
	}
}

void AMagicalArrow::AttackStart() const
{
	_MagicalArrowCollision->SetCollisionProfileName("MagicalArrow");
	_MagicalArrowCollision->SetNotifyRigidBodyCollision(true);
}

void AMagicalArrow::AttackEnd() const
{
	_MagicalArrowCollision->SetCollisionProfileName("MagicalArrow");
	_MagicalArrowCollision->SetNotifyRigidBodyCollision(false);
}

void AMagicalArrow::BeginPlay()
{
	Super::BeginPlay();
	if (!_MagicalArrowCollision)
	{
		UE_LOG(LogTemp, Error, TEXT("MagicalArrow collision component is NULL!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MagicalArrow collision component is valid!"));
	}
}

void AMagicalArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void AMagicalArrow::Move(float deltaTime)
{
	if (!_isHoming)
	{
		FVector currentPos = GetActorLocation();
		FVector forwardDir = GetActorForwardVector();

		SetActorLocation(currentPos + forwardDir * _speed);
	}
}

void AMagicalArrow::OnMagicalArrowHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (auto* Enemy = Cast<ACPPAICharacterBase>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));

		ACharacterController* myCharacter = Cast<ACharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		float newHealth = Enemy->GetHealth() - 40.0f * myCharacter->GetDamageBoost();
		Enemy->SetHealth(newHealth);

		//UE_LOG(LogTemp, Warning, TEXT("Arrow hit enemy! New health: %f"), NewHealth);

		Destroy();
	}
}
