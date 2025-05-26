// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArboriaUI.generated.h"

UCLASS(Abstract)
class ARBORIA_API UArboriaUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHealth(float CurrentHealth, float MaxHealth);
	float GetHealth();
	void SetXP(float CurrentXP, float MaxXP);
	float GetXP();
	void SetLevel(FText CurrentLevel);
	void SetState(FText CurrentState);
	void SetTriSlider(float CurrentPercent);
	void SetRainSlider(float CurrentPercent);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* XPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* StateText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class URadialSlider* TriSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class URadialSlider* RainSlider;
};
