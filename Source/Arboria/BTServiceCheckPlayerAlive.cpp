// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceCheckPlayerAlive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "CPPAICharacterBase.h"
#include "CharacterController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"

UBTServiceCheckPlayerAlive::UBTServiceCheckPlayerAlive()
{
	NodeName = TEXT("TestAlive");
}

void UBTServiceCheckPlayerAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    // Vérifie si le joueur est toujours valide dans le Blackboard
    auto* Player = Cast<ACharacterController>(BlackboardComp->GetValueAsObject(PlayerAliveKey));
    if (!Player)
    {
        // Log pour vérifier que le player est bien retrouvé ou non
        UE_LOG(LogTemp, Warning, TEXT("Player is null!"));
        BlackboardComp->SetValueAsBool("IsPlayerAlive", false);
        return;
    }


	bool bIsPlayerAlive = (Player && Player->GetPlayerStatue());
	UE_LOG(LogTemp, Warning, TEXT("Is Player Alive: %s"), bIsPlayerAlive ? TEXT("True") : TEXT("False"));
	BlackboardComp->SetValueAsBool(PlayerAliveKey, bIsPlayerAlive);

	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsPlayerAlive);

}
