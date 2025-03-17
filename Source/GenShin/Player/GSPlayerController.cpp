// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerController.h"
#include "UI/GSHUDWidget.h"

AGSPlayerController::AGSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UGSHUDWidget> GSHUDWidgetRef(TEXT("/Game/GenShin/UI/WBP_GSHUD.WBP_GSHUD_C"));
	if (GSHUDWidgetRef.Class)
	{
		GSHUDWidgetClass = GSHUDWidgetRef.Class;
	}
}

void AGSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺�� ȭ�� �ӿ��� ������.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// HUD���� ���� �� ����.
	GSHUDWidget = CreateWidget<UGSHUDWidget>(this, GSHUDWidgetClass);
	if (GSHUDWidget)
	{
		GSHUDWidget->AddToViewport();
	}
}
