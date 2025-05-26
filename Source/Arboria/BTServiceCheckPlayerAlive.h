// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceCheckPlayerAlive.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API UBTServiceCheckPlayerAlive : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTServiceCheckPlayerAlive();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard" , meta = (AllowPrivateAccess = "true"))
	FName PlayerAliveKey = "IsPlayerAlive";

};
