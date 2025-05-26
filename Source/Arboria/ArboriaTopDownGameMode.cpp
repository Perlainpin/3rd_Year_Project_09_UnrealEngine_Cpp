// Fill out your copyright notice in the Description page of Project Settings.


#include "ArboriaTopDownGameMode.h"
#include "ArboriaGameStateBase.h"
#include "ArboriaUI.h"

AArboriaTopDownGameMode::AArboriaTopDownGameMode() {
    GameStateClass = AArboriaGameStateBase::StaticClass();
    MainMenuWidgetClass = nullptr;
    MainMenuWidget = nullptr;
}

void AArboriaTopDownGameMode::StartPlay() {
    GetWorld()->GetAuthGameMode()->GetGameState<AArboriaGameStateBase>();

    Super::StartPlay();
    
    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UArboriaUI>(GetWorld(), MainMenuWidgetClass);

        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
            SetLevelText(FText::FromName(GetWorld()->GetAuthGameMode()->GetGameState<AArboriaGameStateBase>()->GetActuaLevel()));
            SetStateText(FText::FromName(GetWorld()->GetAuthGameMode()->GetGameState<AArboriaGameStateBase>()->GetActualState()));
			ResetHealthBar();
			ResetXPBar();
			ArrowsRainSlideRecharge(1.f);
			TriSpellSlideRecharge(1.f);
        }
    }
}

void AArboriaTopDownGameMode::ArrowsRainSlideRecharge(float deltatime) {
    if (MainMenuWidget)
    {
		MainMenuWidget->SetRainSlider(deltatime);
    }
}

void AArboriaTopDownGameMode::TriSpellSlideRecharge(float deltatime) {
    if (MainMenuWidget)
    {
        MainMenuWidget->SetTriSlider(deltatime);
    }
}

void AArboriaTopDownGameMode::SetLevelText(FText leveltext) {
    if (MainMenuWidget)
    {
        MainMenuWidget->SetLevel(leveltext);
    }
}

void AArboriaTopDownGameMode::SetStateText(FText statetext) {
	if (MainMenuWidget)
	{
		MainMenuWidget->SetState(statetext);
	}
}

void AArboriaTopDownGameMode::ResetHealthBar(){
	if (MainMenuWidget)
	{
		MainMenuWidget->SetHealth(100, 100);
	}
}

void AArboriaTopDownGameMode::TakeDamage(float CurrentHealth) {
    if (MainMenuWidget)
    {
        MainMenuWidget->SetHealth(CurrentHealth, 100);
    }
}

void AArboriaTopDownGameMode::UseHeal(float heal) {
    if (MainMenuWidget)
    {
        MainMenuWidget->SetHealth((MainMenuWidget->GetHealth() + heal), 100);
    }
}

void AArboriaTopDownGameMode::ResetXPBar() {
	if (MainMenuWidget)
	{
		MainMenuWidget->SetXP(0, 100);
	}
}

void AArboriaTopDownGameMode::AddXP(float xp) {
	if (MainMenuWidget)
	{
		MainMenuWidget->SetXP((MainMenuWidget->GetXP() + xp), 100);
	}
}