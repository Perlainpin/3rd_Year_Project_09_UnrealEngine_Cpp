// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPlayerAlive.h"
#include "CharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "CPPAICharacterBase.h"


UBTService_CheckPlayerAlive::UBTService_CheckPlayerAlive()
{
	NodeName = TEXT("Check Player Alive");
}

void UBTService_CheckPlayerAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    // Récupérer le Player
    ACharacterController* Player = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player is null!"));
        BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), false);
        return;
    }

    // Mettre à jour le Blackboard avec l'état du joueur
    bool bIsPlayerAlive = Player->GetPlayerStatue();
    BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), bIsPlayerAlive);
}
