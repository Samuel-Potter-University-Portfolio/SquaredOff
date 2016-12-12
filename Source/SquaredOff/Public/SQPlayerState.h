// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "SQPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(replicated, BlueprintReadOnly, Category = "Match")
	bool is_ready = false;

	UPROPERTY(replicated, BlueprintReadOnly, Category = "Match")
	uint8 team_index = 0;

	UFUNCTION(Server, Unreliable, WithValidation)
	void SetReadyState(const bool ready_state);

	UFUNCTION(BlueprintCallable, Category = "Match")
	void SetReady(const bool ready_state);
};
