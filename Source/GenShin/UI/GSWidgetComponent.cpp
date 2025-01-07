// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GSWidgetComponent.h"
#include "UI/GSUserWidget.h"

void UGSWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UGSUserWidget* GSUserWidget = Cast<UGSUserWidget>(GetWidget());
	if (GSUserWidget)
	{
		GSUserWidget->SetOwningActor(GetOwner());
	}

}
