// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathAnimNotifyState.h"
#include "NPC.h"

void UDeathAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{	
	if (ANPC* const npc = Cast<ANPC>(MeshComp->GetOwner()))
	{
		npc->Destroy();
	}
}
