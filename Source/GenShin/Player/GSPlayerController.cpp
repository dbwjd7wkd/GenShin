// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerController.h"

void AGSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺�� ȭ�� �ӿ��� ������.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
