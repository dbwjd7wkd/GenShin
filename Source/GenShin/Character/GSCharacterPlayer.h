// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "InputActionValue.h" // Build.cs에 "EnhancedInput" 모듈을 추가해줘야 쓸 수 있음.
#include "GSCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GENSHIN_API AGSCharacterPlayer : public AGSCharacterBase
{
	GENERATED_BODY()
	
public:
	AGSCharacterPlayer();

protected:
	// 입력 맵핑 컨텍스트를 할당함.
	// 키보드와 키패드 중에서 어떤 것으로 입력 받을 것인지는 BeginPlay의 DefaultMappingContext를 통해서 진행해주면 됨.
	virtual void BeginPlay() override;

public:
	// 입력 액션과 우리가 선언한 Move, Look 함수를 서로 매핑 시키는 것을 여기서 미리 완료.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	// 에셋을 변경가능하도록 제작하기 위해 EditAnywhere로 지정.
	// 1가지 매핑컨텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	// 3가지 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	// 각 입력 액션에 대해서 매핑된 함수들
	// FInputActionValue를 입력으로 받기 때문에 #include "InputActionValue.h" 해줘야 함.
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
