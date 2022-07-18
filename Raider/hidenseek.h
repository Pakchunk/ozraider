#pragma once

#include "gui.h"
#include "ue4.h"

class HideAndSeek
{
public:
    void InitializeHideAndSeek()
    {
        ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZoneActive = false;
        ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = true;
    }

    void AircraftJump(AFortPlayerControllerAthena* PC, AFortPlayerStateAthena* State, AFortPlayerStateAthena* Seeker)
    {
        if (State == Seeker)
        {
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
        else
        {
            UFortWeaponRangedItemDefinition* PID = nullptr;

            for (int i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
            {
                auto instance = PC->WorldInventory->Inventory.ItemInstances[i];
                if (instance->ItemEntry.ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
                {
                    PID = (UFortWeaponRangedItemDefinition*)instance->ItemEntry.ItemDefinition;
                    break;
                }
                else
                    continue;
            }

            if (PC && PC->QuickBars && PC->WorldInventory)
            {
                PC->QuickBars->K2_DestroyActor();
                PC->WorldInventory->Inventory.ItemInstances.FreeArray();
                PC->WorldInventory->Inventory.ReplicatedEntries.FreeArray();
                PC->QuickBars = SpawnActor<AFortQuickBars>({ -280, 400, 3000 }, PC);
                PC->OnRep_QuickBar();
                EquipLoadout(PC, {PID});
            }

            auto Pawn = (AFortPlayerPawnAthena*)PC->Pawn;
            Pawn->SetMaxHealth(50);
            Pawn->SetMaxShield(0);

            EquipInventoryItem(PC, PC->QuickBars->PrimaryQuickBar.Slots[0].Items[0]);
        }
    }
};