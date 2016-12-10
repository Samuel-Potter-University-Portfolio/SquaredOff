// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "Blueprint/UserWidget.h"
#include "SQPlayerController.h"

void ASQPlayerController::SetupHUD_Implementation(const TArray<TSubclassOf<UUserWidget>>& widgets)
{
	for (const TSubclassOf<UUserWidget>& widget_class : widgets)
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(this, widget_class);
		if (!widget)
			continue;
		widget->AddToViewport(); 
		widget->SetVisibility(ESlateVisibility::Visible); 
	}
}

