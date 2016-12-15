// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SQGameMode_Team.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQCharacter.h"
#include "SQPlayerController.h"
#include "SQGameMode_TeamArenaBrawl.generated.h"

/*
From 'GameMode.h'

namespace MatchState
{
	extern ENGINE_API const FName EnteringMap;			// We are entering this map, actors are not yet ticking
	extern ENGINE_API const FName WaitingToStart;		// Actors are ticking, but the match has not yet started
	extern ENGINE_API const FName InProgress;			// Normal gameplay is occurring. Specific games will have their own state machine inside this state
	extern ENGINE_API const FName WaitingPostMatch;		// Match has ended so we aren't accepting new players, but actors are still ticking
	extern ENGINE_API const FName LeavingMap;			// We are transitioning out of the map to another location
	extern ENGINE_API const FName Aborted;				// Match has failed due to network issues or other problems, cannot continue
}
*/


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


	UFUNCTION(BlueprintNativeEvent)
	void OnKill(ASQCharacter* character);

	UFUNCTION(BlueprintCallable, Category="GameMode")
	void Respawn(AController* controller);

	//virtual bool IsMatchInProgress() const { return false; };
	//virtual bool HasMatchEnded() const { return false; };
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void StartMatch() override;
	//virtual void EndMatch() override {};
	//virtual void RestartGame() override {};
	//virtual void AbortMatch() override {};
};
