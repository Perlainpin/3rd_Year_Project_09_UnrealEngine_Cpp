// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckPlayerAlive.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API UBTService_CheckPlayerAlive : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public :
	UBTService_CheckPlayerAlive();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private :
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FName PlayerAliveKey = "IsPlayerAlive";
	
};
