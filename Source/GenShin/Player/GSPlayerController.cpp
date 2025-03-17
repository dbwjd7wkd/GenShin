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

	// 게임 시작 시 마우스가 화면 속에서 시작함.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// HUD위젯 생성 및 띄우기.
	GSHUDWidget = CreateWidget<UGSHUDWidget>(this, GSHUDWidgetClass);
	if (GSHUDWidget)
	{
		GSHUDWidget->AddToViewport();
	}
}
