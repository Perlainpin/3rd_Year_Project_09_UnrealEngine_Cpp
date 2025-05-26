// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"

#include "NPC_AIController.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	//get AI controller
	if (auto* const Cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		//try get AI
		if (auto* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			//get the blackboard 
			if (auto* const BC = OwnerComp.GetBlackboardComponent())
			{
				//Get Numver of point 
				auto const NoOfPoints = NPC->GetPatrolPath()->Num();
				auto const MinIndex = 0;
				auto const MaxIndex = NoOfPoints - 1;
				auto  Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

				//change direction 
				if (bBiDirectional)
				{
					if (Index>= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}

				}

				// write the new index to the blackboard
				BC->SetValueAsInt(GetSelectedBlackboardKey(), 
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NoOfPoints);

				//finish with success
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}



	return EBTNodeResult::Failed;
}
