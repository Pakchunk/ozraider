#pragma once

#include "gui.h"
#include "ue4.h"
#include "game.h"

class Playground
{
public:
    void InitializePlayground(UFortPlaylistAthena* CurrentPlaylist, AAthena_GameState_C* GameState)
    {
        auto GameMode = reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode);
        GameMode->bSafeZoneActive = false;
        GameMode->bSafeZonePaused = true;

        auto Playlist = CurrentPlaylist;

        if (Playlist)
        {
            Playlist->bNoDBNO = true;
            Playlist->FriendlyFireType = EFriendlyFireType::Off;
            Playlist->RespawnType = EAthenaRespawnType::InfiniteRespawn;
            Playlist->RespawnLocation = EAthenaRespawnLocation::Air;

            GameState->CurrentPlaylistData = Playlist;
            GameState->OnRep_CurrentPlaylistData();
        }
    }

    void OnDeath(AFortPlayerControllerAthena* PC, FVector NewLocation)
    {
        NewLocation.Z += 20000;

        if (PC->Pawn)
            PC->Pawn->K2_DestroyActor();

        InitPawn(PC, NewLocation, FQuat(), false);

        auto CM = (UFortCheatManager*)PC->CheatManager;
        CM->AllowRespawn();
        PC->RespawnPlayerAfterDeath();
        CM->RespawnPlayer();
        CM->RespawnPlayerServer();
    }
};