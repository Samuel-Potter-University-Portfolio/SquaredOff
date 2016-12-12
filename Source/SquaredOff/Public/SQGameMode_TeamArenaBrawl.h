// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SQGameMode_Team.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQCharacter.h"
#include "SQPlayerController.h"
#include "SQGameMode_TeamArenaBrawl.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQGameMode_TeamArenaBrawl : public AGameMode
{
	GENERATED_BODY()
private:
	ASQGameState_ArenaBrawl* sq_game_state;

public:
	ASQGameMode_TeamArenaBrawl();

	virtual void StartPlay() override;

	virtual void Tick(float delta_seconds) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* new_player) override;

	virtual void StartMatch() override;
	virtual bool ReadyToStartMatch_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnKill(ASQCharacter* character);

	UFUNCTION(BlueprintCallable, Category="GameMode")
	void Respawn(AController* controller);
};
