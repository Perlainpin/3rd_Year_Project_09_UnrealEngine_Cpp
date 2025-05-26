// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NPC.h"	

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& Owneromp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(Owneromp, NodeMemory);
	if (auto const Cont = Owneromp.GetAIOwner())
	{
		if (auto* const NPC = Cast<ANPC>(Cont->GetPawn()))
		{
			NPC->GetCharacterMovement()->MaxWalkSpeed = speed;
		}
	}
}
