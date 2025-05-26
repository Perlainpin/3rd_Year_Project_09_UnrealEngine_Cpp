// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "CombatInterface.h"
#include"AIController.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include"Engine/LatentActionManager.h"
#include"Components/SkeletalMeshComponent.h"
#include"Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"


UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//not attack if we are out of range
	auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
	if (OutOfRange)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	//in range, get the AI and the NPC
	auto const* const cont = OwnerComp.GetAIOwner();
	auto* const npc = Cast<ANPC>(cont->GetPawn());

	//Execute Melee Attack
	if (auto* const icombat = Cast<ICombatInterface>(npc))
	{
		if (MontageHasFinished(npc)) {

			icombat->Execute_MeleeAttack(npc);

		}
	}

	//finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(ANPC* const npc)
{
	return npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npc->GetMontage()); 
}
