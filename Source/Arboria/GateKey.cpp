// Fill out your copyright notice in the Description page of Project Settings.


#include "GateKey.h"


// Sets default values
AGateKey::AGateKey()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

    if (CubeVisualAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
        VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        VisualMesh->SetCollisionProfileName(TEXT("OverlapAll"));
    }
    VisualMesh->OnComponentBeginOverlap.AddDynamic(this, &AGateKey::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGateKey::BeginPlay()
{
    Super::BeginPlay();
    MyGameState = GetWorld()->GetGameState<AArboriaGameStateBase>();
}

void AGateKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    MyGameState->UpdateState();
    MyGameState->AddFirstSateKey();
}

// Called every frame
void AGateKey::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

