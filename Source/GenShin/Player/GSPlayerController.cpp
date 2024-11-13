// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerController.h"

void AGSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 마우스가 화면 속에서 시작함.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
