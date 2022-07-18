#pragma once

#include "gui.h"
#include "ue4.h"
#include "game.h"

// GAMEMODE WIP

class GunGame
{
    void InitializeGunGame(AAthena_GameState_C* GameState)
    {
        ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZoneActive = false;
        ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = true;

        if (GameState->CurrentPlaylistData)
        {
            GameState->CurrentPlaylistData->bNoDBNO = true;
            GameState->CurrentPlaylistData->FriendlyFireType = EFriendlyFireType::On;
            GameState->CurrentPlaylistData->RespawnType = EAthenaRespawnType::InfiniteRespawn;
            GameState->CurrentPlaylistData->RespawnLocation = EAthenaRespawnLocation::Air;
            GameState->OnRep_CurrentPlaylistData();
        }
    }

    void DroneAnimEnded(AFortPlayerControllerAthena* PC, FVector NewLocation)
    {
        NewLocation.Z += 4000;

        if (PC->Pawn)
            PC->Pawn->K2_DestroyActor();

        InitPawn(PC, NewLocation, FQuat(), false);

        auto CM = (UFortCheatManager*)PC->CheatManager;
        CM->AllowRespawn();
        PC->RespawnPlayerAfterDeath();
        CM->RespawnPlayer();
        CM->RespawnPlayerServer();

        for (int i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
        {
            auto instance = PC->WorldInventory->Inventory.ItemInstances[i];
            if (instance->ItemEntry.ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
            {
                EquipInventoryItem(PC, instance->ItemEntry.ItemGuid);
                break;
            }
            else
                continue;
        }
    }
};