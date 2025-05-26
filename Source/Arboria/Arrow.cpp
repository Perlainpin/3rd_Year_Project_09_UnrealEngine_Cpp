// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include <Components/BoxComponent.h>
#include <Components/CapsuleComponent.h>
#include "CPPAICharacterBase.h"
#include "DrawDebugHelpers.h"
#include "CharacterController.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AArrow::AArrow() : _ArrowCollision{ CreateDefaultSubobject<UCapsuleComponent>(TEXT("ArrowCollision")) }
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Assets/SM_Arrow.SM_Arrow'")); // Ensure correct path

	_ArrowCollision->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnArrowHit);

	_ArrowCollision->InitCapsuleSize(45.0f, 25.0f); 
	_ArrowCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_ArrowCollision->SetCollisionObjectType(ECC_WorldDynamic);
	//_ArrowCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	_ArrowCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	_ArrowCollision->SetupAttachment(RootComponent);


	if (MeshAsset.Succeeded())
	{
		_object->SetStaticMesh(MeshAsset.Object);
	}
}

void AArrow::AttackStart() const
{
	_ArrowCollision->SetCollisionProfileName("Arrow");
	_ArrowCollision->SetNotifyRigidBodyCollision(true);
}

void AArrow::AttackEnd() const
{
	_ArrowCollision->SetCollisionProfileName("Arrow");
	_ArrowCollision->SetNotifyRigidBodyCollision(false);
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();

	if (!_ArrowCollision)
	{
		UE_LOG(LogTemp, Error, TEXT("Arrow collision component is NULL!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Arrow collision component is valid!"));
	}
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime); 
}

void AArrow::Move(float deltaTime)
{
	FVector currentPos = GetActorLocation();
	FVector forwardDir = GetActorForwardVector();

	SetActorLocation(currentPos + forwardDir * _speed);
}

void AArrow::OnArrowHit(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 10.0f, 12, FColor::Red, false, 2.0f);
	if (!OtherActor || OtherActor == this) return;

	if (auto* Enemy = Cast<ACPPAICharacterBase>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));

		ACharacterController* myCharacter = Cast<ACharacterController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		float newHealth = Enemy->GetHealth() - 10.0f * myCharacter->GetDamageBoost();
		Enemy->SetHealth(newHealth);

		//UE_LOG(LogTemp, Warning, TEXT("Arrow hit enemy! New health: %f"), NewHealth);

		Destroy();
	}

}
