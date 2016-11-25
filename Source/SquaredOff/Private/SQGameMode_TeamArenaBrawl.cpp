// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQPlayerController.h"
#include "SQPlayerState.h"
#include "SQCharacter.h"
#include "SQGameMode_TeamArenaBrawl.h"

ASQGameMode_TeamArenaBrawl::ASQGameMode_TeamArenaBrawl()
{
	DefaultPawnClass = ASQCharacter::StaticClass();
	PlayerControllerClass = ASQPlayerController::StaticClass();
	PlayerStateClass = ASQPlayerState::StaticClass();
	GameStateClass = ASQGameState_ArenaBrawl::StaticClass();
}


