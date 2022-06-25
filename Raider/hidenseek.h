#pragma once

#include "gui.h"
#include "ue4.h"

class HideAndSeek
{
public:
    void InitializeHideAndSeek()
    {
        auto GameMode = reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode);
        GameMode->bSafeZoneActive = false;
        GameMode->bSafeZonePaused = true;
    }

    void AircraftJump(AFortPlayerControllerAthena* PC, AFortPlayerStateAthena* State, AFortPlayerStateAthena* Seeker)
    {
        if (State == Seeker)
        {
            PC->ActivateSlot(EFortQuickBars::Primary, 0, 0, true); // Select the pickaxe

            bool bFound = false;
            auto PickaxeEntry = FindItemInInventory<UFortWeaponMeleeItemDefinition>(PC, bFound);

            if (bFound)
                EquipInventoryItem(PC, PickaxeEntry.ItemGuid);
        }
        else
        {
            //PC->ActivateSlot(EFortQuickBars::Primary, 0, 0, true); // Select the pickaxe

            //bool bFound = false;
            //auto PickaxeEntry = FindItemInInventory<UFortWeaponMeleeItemDefinition>(PC, bFound);
            //auto Entries = PC->WorldInventory->Inventory.ItemInstances;
            //if (bFound)
                //EquipInventoryItem(PC, PickaxeEntry.ItemGuid);
            static std::vector<UFortWeaponRangedItemDefinition*> HiderLoadout = 
            {
                (UFortWeaponRangedItemDefinition*)PC->WorldInventory->Inventory.ItemInstances[13]->ItemEntry.ItemDefinition,
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                nullptr
            };

            PC->QuickBars->Owner = nullptr;
            PC->QuickBars->SetOwner(nullptr);
            PC->QuickBars->OnRep_Owner();
            PC->QuickBars->K2_DestroyActor();
            PC->QuickBars = nullptr;
            PC->OnRep_QuickBar();
            InitInventory(PC, false);
            //EquipLoadout(PC, HiderLoadout);
            //AddItem(PC, nullptr, 0);
            auto Pawn = (AFortPlayerPawnAthena*)PC->Pawn;
            Pawn->SetMaxHealth(1);
            Pawn->SetMaxShield(0);
            EquipLoadout(PC, HiderLoadout);

            PC->ActivateSlot(EFortQuickBars::Primary, 0, 0, true);

            bool bFoundAgain = false;
            auto newPickaxeEntry = FindItemInInventory<UFortWeaponMeleeItemDefinition>(PC, bFoundAgain);

            if (bFoundAgain)
                EquipInventoryItem(PC, PC->WorldInventory->Inventory.ItemInstances[13]->ItemEntry.ItemGuid);
        }
    }
};