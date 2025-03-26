// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GSGameMode.h"
#include "Player/GSPlayerController.h"

AGSGameMode::AGSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/GenShin/Blueprint/BP_GSCharacter.BP_GSCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/GenShin.GSPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AGSGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	AGSPlayerController* GSPlayerController = Cast<AGSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GSPlayerController)
	{
		GSPlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		if (GSPlayerController)
		{
			GSPlayerController->GameClear();
		}
	}
}

void AGSGameMode::OnPlayerDead()
{
	AGSPlayerController* GSPlayerController = Cast<AGSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GSPlayerController)
	{
		GSPlayerController->GameOver();
	}
}

bool AGSGameMode::IsGameCleared()
{
	return bIsCleared;
}
