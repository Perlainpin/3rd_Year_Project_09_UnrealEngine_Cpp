// Fill out your copyright notice in the Description page of Project Settings.


#include "ArboriaGameStateBase.h"
#include "ArboriaTopDownGameMode.h"
#include "Kismet/GameplayStatics.h"

AArboriaGameStateBase::AArboriaGameStateBase()
{

}

void AArboriaGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	LevelIndex = 0;
	StateIndex = 0;
	ListOfLevel = {
		TEXT("EscapeTheOrkCamp"),
		TEXT("PathToVictory")
	};
	ListOfState = {
		{ListOfLevel[0],{TEXT("FindTheKeys"),TEXT("OpenTheGate")}},
		{ListOfLevel[1], {TEXT("Go")}}
	};
	ActualLevel = ListOfLevel[0];
	ActualState = ListOfState[ActualLevel][0];
	FirstStateNumberOfKey = 0;
	FirstStateNeededNumberOfKey = 3;
};

void AArboriaGameStateBase::UpdateState() {
	if (ActualState == ListOfState[TEXT("EscapeTheOrkCamp")][0]) {
		FirstStateNumberOfKeyCheck();
	}
	ChangeActualState();
	ChangeActualLevel();
}

void AArboriaGameStateBase::ChangeActualState() {
	if (StateIndex > ListOfState[ActualLevel].Num()) {
		LevelIndex++;
		StateIndex = 0;
	}
	ActualState = ListOfState[ActualLevel][StateIndex];
	Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetStateText(FText::FromName(ActualState));
};

void AArboriaGameStateBase::ChangeActualLevel() {
	ActualLevel = ListOfLevel[LevelIndex];
	Cast< AArboriaTopDownGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->SetLevelText(FText::FromName(ActualLevel));
}

void AArboriaGameStateBase::AddFirstSateKey() {
	if (ActualState == ListOfState[TEXT("EscapeTheOrkCamp")][0]) {
		FirstStateNumberOfKey++;
	}
	else {
		return;
	}
};

int32 AArboriaGameStateBase::GetFirstStateKey() {
	return FirstStateNumberOfKey;
};

int32 AArboriaGameStateBase::GetFirstStateNeededKey() {
	return FirstStateNeededNumberOfKey;
};

FName AArboriaGameStateBase::GetActuaLevel() {
	return ActualLevel;
};

FName AArboriaGameStateBase::GetActualState() {
	return ActualState;
};

void AArboriaGameStateBase::FirstStateNumberOfKeyCheck() {
	if (ActualState == ListOfState[TEXT("EscapeTheOrkCamp")][0] && FirstStateNumberOfKey == FirstStateNeededNumberOfKey) {
		StateIndex++;
	}
	else {
		return;
	}
};

void AArboriaGameStateBase::TriggerStateChange() {
	StateIndex++;
}