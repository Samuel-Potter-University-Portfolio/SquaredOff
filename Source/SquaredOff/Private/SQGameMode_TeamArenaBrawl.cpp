// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQPlayerController.h"
#include "SQPlayerState.h"
#include "SQCharacter.h"
#include "SQGameMode_TeamArenaBrawl.h"

ASQGameMode_TeamArenaBrawl::ASQGameMode_TeamArenaBrawl()
{
	static ConstructorHelpers::FClassFinder<ASQCharacter> bp_player_finder(TEXT("/Game/Player/BP_SQCharacter.BP_SQCharacter_C"));
	static ConstructorHelpers::FClassFinder<ASQPlayerController> bp_controller_finder(TEXT("/Game/Player/BP_SQPlayerController.BP_SQPlayerController_C"));

	DefaultPawnClass = bp_player_finder.Succeeded() ? bp_player_finder.Class : ASQCharacter::StaticClass();
	DefaultPlayerName = FText::FromString(TEXT("Player"));
	PlayerControllerClass = bp_controller_finder.Succeeded() ? bp_controller_finder.Class : ASQPlayerController::StaticClass();
	PlayerStateClass = ASQPlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<AHUD> bp_hud_finder(TEXT("/Game/UI/DefaultHUD.DefaultHUD_C"));
	HUDClass = bp_hud_finder.Succeeded() ? bp_hud_finder.Class : AHUD::StaticClass();

	GameStateClass = ASQGameState_ArenaBrawl::StaticClass();
	
	bUseSeamlessTravel = true;
	bPauseable = false;
	//bRemovePawnsAtStart = true;
	bStartPlayersAsSpectators = true;
}

void ASQGameMode_TeamArenaBrawl::HandleStartingNewPlayer_Implementation(APlayerController* new_player) 
{
	Super::HandleStartingNewPlayer_Implementation(new_player);
	new_player->PlayerState->bIsSpectator = true; //Make sure players start as spectators (For PIE)
}


void ASQGameMode_TeamArenaBrawl::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	if (!HasMatchStarted() && ReadyToStartMatch())
	{
	}
}