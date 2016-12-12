// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "SQGameState_ArenaBrawl.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQGameState_ArenaBrawl : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Match")
	FText GetFormattedPhase();

	UFUNCTION(BlueprintPure, Category = "Match")
	FText GetFormattedDisplayedTime();
	virtual float GetDisplayedTime() { return 134.0f; }
};
