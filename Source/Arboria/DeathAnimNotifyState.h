// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DeathAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API UDeathAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public :
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
