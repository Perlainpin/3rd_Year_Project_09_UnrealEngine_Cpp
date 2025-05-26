// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ArboriaTopDownGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARBORIA_API AArboriaTopDownGameMode : public ATopDownGameMode
{
	GENERATED_BODY()

public:
	AArboriaTopDownGameMode();
	virtual ~AArboriaTopDownGameMode() = default;

	virtual void StartPlay();

	void ArrowsRainSlideRecharge(float deltatime);
	void TriSpellSlideRecharge(float deltatime);
	void SetLevelText(FText leveltext);
	void SetStateText(FText statetext);
	void ResetHealthBar();
	void TakeDamage(float damage);
	void UseHeal(float heal);
	void ResetXPBar();
	void AddXP(float xp);

protected:
	//Classe du Widget (modifiable dans l’éditeur)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UArboriaUI> MainMenuWidgetClass;

private:
	UPROPERTY()
	UArboriaUI* MainMenuWidget;
};
