// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"
#include "NPC_AIController.h"
#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get controller and npc
	auto const* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner());
	auto const* const npc = Cast<ANPC>(cont->GetPawn());

	//get player character
	auto const* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//while true or false to the backboard key depending on the distance between the player and the npc
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), npc->GetDistanceTo(player) <= MeleeRange);


}
