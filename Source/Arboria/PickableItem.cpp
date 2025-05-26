// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include <NiagaraFunctionLibrary.h>

// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create a Root Component
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	SetRootComponent(Root); 

	// Create the Static Mesh Component
	_object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	_object->SetupAttachment(Root);
	_object->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	_object->SetVisibility(true);
	_object->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_object->SetCollisionObjectType(ECC_WorldDynamic);  
	_object->SetCollisionResponseToAllChannels(ECR_Ignore);
	_object->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	// Create the Box Collision Component
	_collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	_collisionBox->SetupAttachment(_object);
	_collisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f)); 
	_collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_collisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	_collisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	_collisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	// Bind Overlap Event
	_collisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();

	_relativeZPos = _object->GetRelativeLocation().Z; 

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		_vfx,
		RootComponent,
		TEXT("VFX"),
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f),
		EAttachLocation::KeepRelativeOffset,
		false
	);
}

void APickableItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (_objectMesh)
	{
		_object->SetStaticMesh(_objectMesh);
	}
}

void APickableItem::Animation(float deltaTime)
{
	float newPosZ = _relativeZPos + sin(GetGameTimeSinceCreation() * _floatingSpeed) * _floatingDistance;

	_object->SetRelativeLocation(FVector(0, 0, newPosZ));

	float newRotationZ = 1.5f; 
	FQuat DeltaRotation = FQuat(FRotator(0.0f, newRotationZ, 0.0f));

	_object->AddRelativeRotation(DeltaRotation);
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Animation(DeltaTime);
}

void APickableItem::OnPicked(AActor* OtherActor)
{
}

void APickableItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		OnPicked(OtherActor);

		Destroy();
	}
}