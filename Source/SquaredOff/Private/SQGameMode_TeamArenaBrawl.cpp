// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"
#include "SQPlayerState.h"
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

	bDelayedStart = true;
	bUseSeamlessTravel = true;
	bPauseable = false;
	bStartPlayersAsSpectators = true;
}

bool team_allocation_flag = false;

void ASQGameMode_TeamArenaBrawl::HandleStartingNewPlayer_Implementation(APlayerController* new_player) 
{
	Super::HandleStartingNewPlayer_Implementation(new_player);
	new_player->PlayerState->bIsSpectator = true; //Make sure players start as spectators (For PIE)

	ASQPlayerState* player_state = (ASQPlayerState*) new_player->PlayerState;

	if (player_state)
	{
		player_state->team_index = team_allocation_flag;
		team_allocation_flag = !team_allocation_flag;
	}
}

void ASQGameMode_TeamArenaBrawl::StartPlay() 
{
	Super::StartPlay();
	sq_game_state = (ASQGameState_ArenaBrawl*) GameState;
}

void ASQGameMode_TeamArenaBrawl::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	//if (HasMatchStarted() )
}

void ASQGameMode_TeamArenaBrawl::StartMatch() 
{
	Super::StartMatch();

	if (!sq_game_state || !GetWorld())
		return;

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
		Respawn(*it);
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

	//Unpossess and destroy pawn (If Respawn is called straight, it's not classed as a death (Call OnKill))
	APawn* pawn = controller->GetPawn();
	if (pawn)
	{
		controller->UnPossess();
		pawn->Destroy();
	}

	FVector location(0.0f, 0.0f, 1000.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawn_info;

	APawn* new_pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, location, rotation, spawn_info);
	controller->Possess(new_pawn);
}