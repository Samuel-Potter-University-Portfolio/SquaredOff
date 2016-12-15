// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQPlayerState.h"
#include "SQGameMode_TeamArenaBrawl.h"

ASQGameMode_TeamArenaBrawl::ASQGameMode_TeamArenaBrawl()
{
	static ConstructorHelpers::FClassFinder<ASQCharacter> bp_player_finder(TEXT("/Game/Player/BP_SQCharacter.BP_SQCharacter_C"));
	static ConstructorHelpers::FClassFinder<ASQPlayerController> bp_controller_finder(TEXT("/Game/Player/BP_SQPlayerController.BP_SQPlayerController_C"));
	static ConstructorHelpers::FClassFinder<ASQPlayerState> bp_player_state_finder(TEXT("/Game/Player/BP_SQPlayerState.BP_SQPlayerState_C"));

	DefaultPawnClass = bp_player_finder.Succeeded() ? bp_player_finder.Class : ASQCharacter::StaticClass();
	PlayerControllerClass = bp_controller_finder.Succeeded() ? bp_controller_finder.Class : ASQPlayerController::StaticClass();
	PlayerStateClass = bp_player_state_finder.Succeeded() ? bp_player_state_finder.Class : ASQPlayerState::StaticClass();
	DefaultPlayerName = FText::FromString(TEXT("Player"));

	static ConstructorHelpers::FClassFinder<AHUD> bp_hud_finder(TEXT("/Game/UI/DefaultHUD.DefaultHUD_C"));
	HUDClass = bp_hud_finder.Succeeded() ? bp_hud_finder.Class : AHUD::StaticClass();

	GameStateClass = ASQGameState_ArenaBrawl::StaticClass();

	bDelayedStart = true;
	bUseSeamlessTravel = true;
	bPauseable = false;
	bStartPlayersAsSpectators = true;
}

void ASQGameMode_TeamArenaBrawl::HandleStartingNewPlayer_Implementation(APlayerController* new_player) 
{
	//Super::HandleStartingNewPlayer_Implementation(new_player);
}

void ASQGameMode_TeamArenaBrawl::StartPlay() 
{
	Super::StartPlay();
	sq_game_state = (ASQGameState_ArenaBrawl*) GameState;
}

void ASQGameMode_TeamArenaBrawl::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	if (HasMatchStarted() && !HasMatchEnded())
	{
		match_time_left -= delta_seconds;

		ASQGameState_ArenaBrawl* game_state = GetWorld()->GetGameState<ASQGameState_ArenaBrawl>();
		if (game_state)
			game_state->SetMatchTime(match_time_left);


		if (match_time_left <= 0)
		{
			EndMatch();

			if (game_state)
				game_state->is_match_over = true;

			for (FConstPlayerControllerIterator player = GetWorld()->GetPlayerControllerIterator(); player; ++player)
			{
				ASQPlayerController* controller = (ASQPlayerController*) player->Get();
				if (controller)
					Respawn(controller);
			}
		}
	}
}

void ASQGameMode_TeamArenaBrawl::StartMatch() 
{
	Super::StartMatch(); 
	match_time_left = match_length * 60.0f;
}

bool ASQGameMode_TeamArenaBrawl::ReadyToStartMatch_Implementation() 
{
	if (!sq_game_state || !sq_game_state->PlayerArray.Num())
		return false;

	for (APlayerState* player : sq_game_state->PlayerArray)
	{
		ASQPlayerState* player_state = (ASQPlayerState*)player;
		
		if (!player_state || (!player_state->bOnlySpectator && !player_state->is_ready))
			return false;
	}

	return true;
}

void ASQGameMode_TeamArenaBrawl::OnKill_Implementation(ASQCharacter* character) 
{
	AController* controller = character->GetController();

	if (controller)
	{
		Respawn(controller);

		AActor* last_damage = character->last_damage_source;

		//Suicide
		if (!last_damage) 
		{
			APlayerState* player_state = character->PlayerState;
			if (player_state)
				player_state->Score--;
		}

		//Reward killer
		else 
		{
			APlayerState* other_player = (APlayerState*)last_damage;
			if (other_player)
				other_player->Score++;
		}

	}
}

void ASQGameMode_TeamArenaBrawl::Respawn(AController* controller) 
{
	if (!controller || !GetWorld())
		return;
	
	FVector location(0.0f, 0.0f, 1000.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawn_info;
	spawn_info.Owner = controller;
	
	APawn* new_pawn = GetWorld()->SpawnActor<APawn>(!HasMatchEnded() ? DefaultPawnClass : SpectatorClass, location, rotation, spawn_info);
	controller->Possess(new_pawn);
}