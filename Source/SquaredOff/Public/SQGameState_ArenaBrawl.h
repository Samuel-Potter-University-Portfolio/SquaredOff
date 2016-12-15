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
protected:
	UPROPERTY(ReplicatedUsing = OnRep_MatchTime)
	float replicated_time;

	UPROPERTY(BlueprintReadOnly, Category = GameState)
	float match_time;

public:
	ASQGameState_ArenaBrawl();

	UFUNCTION(BlueprintPure, Category = "Match")
	FText GetFormattedPhase();

	UFUNCTION(BlueprintPure, Category = "Match")
	FText GetFormattedDisplayedTime();

	void SetMatchTime(float time) 
	{
		match_time = time;
		replicated_time = time; //TODO make more efficient
	}
	UFUNCTION()
	void OnRep_MatchTime() 
	{
		match_time = replicated_time;
	}

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match")
	bool is_match_over = false;


	virtual void Tick(float delta_seconds) override;
};
