// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathAnimNotify.h"
#include "NPC.h"

void UDeathAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ANPC* NPC = Cast<ANPC>(MeshComp->GetOwner()))
	{
		NPC->Destroy();
	}
}
