// Fill out your copyright notice in the Description page of Project Settings.

#include "ArboriaUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/RadialSlider.h"

void UArboriaUI::SetHealth(float CurrentHealth, float MaxHealth) {
	if (HealthBar) {
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

float UArboriaUI::GetHealth() {
	if (HealthBar) {
		return HealthBar->GetPercent();
	}
	return 0.0f;
}

void UArboriaUI::SetXP(float CurrentXP, float MaxXP) {
	if (XPBar) {
		XPBar->SetPercent(CurrentXP / MaxXP);
	}
}

float UArboriaUI::GetXP() {
	if (XPBar) {
		return XPBar->GetPercent();
	}
	return 0.0f;
}

void UArboriaUI::SetLevel(FText CurrentLevel) {
	if (LevelText) {
		LevelText->SetText(CurrentLevel);
	}
}

void UArboriaUI::SetState(FText CurrentState) {
	if (StateText) {
		StateText->SetText(CurrentState);
	}
}

void UArboriaUI::SetTriSlider(float CurrentPercent) {
	if (TriSlider) {
		TriSlider->SetValue(CurrentPercent);
	}
}

void UArboriaUI::SetRainSlider(float CurrentPercent) {
	if (RainSlider) {
		RainSlider->SetValue(CurrentPercent);
	}
}