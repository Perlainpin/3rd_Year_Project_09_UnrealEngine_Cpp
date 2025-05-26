// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetHealthBarPercentage(float const value);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
    UProgressBar* HealthValue = nullptr;
};
