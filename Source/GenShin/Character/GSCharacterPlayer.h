// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GSCharacterBase.h"
#include "InputActionValue.h" // Build.cs�� "EnhancedInput" ����� �߰������ �� �� ����.
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
	// �Է� ���� ���ؽ�Ʈ�� �Ҵ���.
	// Ű����� Ű�е� �߿��� � ������ �Է� ���� �������� BeginPlay�� DefaultMappingContext�� ���ؼ� �������ָ� ��.
	virtual void BeginPlay() override;

public:
	// �Է� �׼ǰ� �츮�� ������ Move, Look �Լ��� ���� ���� ��Ű�� ���� ���⼭ �̸� �Ϸ�.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	// ������ ���氡���ϵ��� �����ϱ� ���� EditAnywhere�� ����.
	// 1���� �������ؽ�Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	// 3���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	// �� �Է� �׼ǿ� ���ؼ� ���ε� �Լ���
	// FInputActionValue�� �Է����� �ޱ� ������ #include "InputActionValue.h" ����� ��.
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
