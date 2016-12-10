// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "SQPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Player")
	void SetupHUD(const TArray<TSubclassOf<UUserWidget>>& widgets);
	
	
};
