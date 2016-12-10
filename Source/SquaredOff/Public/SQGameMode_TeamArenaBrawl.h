// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "SQGameMode_Team.h"
#include "SQGameMode_TeamArenaBrawl.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQGameMode_TeamArenaBrawl : public AGameMode
{
	GENERATED_BODY()
public:
	ASQGameMode_TeamArenaBrawl();
	

	virtual void Tick(float delta_seconds) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* new_player) override;
};
