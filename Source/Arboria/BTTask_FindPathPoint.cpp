// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "PatrolPath.h"


UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("FindPathPoint");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//attempt to get the AI controller
	if (auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		//attend to get the blackboard component
		if (auto* const bc = OwnerComp.GetBlackboardComponent())
		{
			//get the current patrol path index
			auto const Index = bc->GetValueAsInt((GetSelectedBlackboardKey()));

			//get th AI

			if (auto* npc = Cast<ANPC>(cont->GetPawn()))
			{
				//get the patrol path
				auto const Point = npc->GetPatrolPath()->GetPatrolPoint(Index);

				//convert the local vector to global point 
				auto const GlobaPoint = npc->GetPatrolPath()->GetTransform().TransformPosition(Point);
				bc->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobaPoint);


				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}

