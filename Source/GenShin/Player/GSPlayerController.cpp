// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GSPlayerController.h"
#include "UI/GSHUDWidget.h"
#include "Player/GSSaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogGSPlayerController);

AGSPlayerController::AGSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UGSHUDWidget> GSHUDWidgetRef(TEXT("/Game/GenShin/UI/WBP_GSHUD.WBP_GSHUD_C"));
	if (GSHUDWidgetRef.Class)
	{
		GSHUDWidgetClass = GSHUDWidgetRef.Class;
	}
}

void AGSPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AGSPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AGSPlayerController::GameOver()
{
	K2_OnGameOver();

	// 게임데이터 저장하기
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogGSPlayerController, Error, TEXT("Save Game Error!"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AGSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 마우스가 화면 속에서 시작함.
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 저장된 게임 로딩.
	SaveGameInstance = Cast<UGSSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UGSSaveGame>();
		SaveGameInstance->RetryCount = 0;
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
