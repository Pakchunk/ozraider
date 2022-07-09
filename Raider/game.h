#pragma once

#include "ue4.h"
#include "gui.h"

namespace Game
{
    static std::vector<UFortWeaponRangedItemDefinition*> CommonWeapons;
    static std::vector<UFortWeaponRangedItemDefinition*> UnCommonWeapons;
    static std::vector<UFortWeaponRangedItemDefinition*> RareWeapons;
    static std::vector<UFortWeaponRangedItemDefinition*> EpicWeapons;
    static std::vector<UFortWeaponRangedItemDefinition*> LegendaryWeapons;
    static std::array<UFortResourceItemDefinition*, 3> Materials;

    static std::array<UFortWeaponRangedItemDefinition*, 6> Shotguns;
    static std::array<UFortWeaponRangedItemDefinition*, 5> SMGs;
    static std::array<UFortWeaponRangedItemDefinition*, 6> Rifles;
    static std::vector<UFortWeaponRangedItemDefinition*> AllConsumables;
    static PlayerLoadout FunConsumables;
    static PlayerLoadout Heals;

    void InitLoot()
    {
        // this might be the wrong datatable V
        static auto DataTable = UObject::FindObject<UDataTable>("DataTable AthenaRangedWeapons.AthenaRangedWeapons");
        static auto FuncLibrary = reinterpret_cast<UDataTableFunctionLibrary*>(UDataTableFunctionLibrary::StaticClass());
        TArray<FName> Names;
        FuncLibrary->STATIC_GetDataTableRowNames(DataTable, &Names);

        for (int i = 0; i < Names.Num(); i++)
        {
            auto Name = Names[i].ToString();
            if (Name == "Sniper_TripleShot_Athena_SR_Ore_T03" || Name == "Sniper_Shredder_Athena_SR_Ore_T03" || Name == "Sniper_AMR_Athena_SR_Ore_T03" || Name == "Shotgun_Minigun_Athena_SR_Ore_T03" || Name == "Pistol_Zapper_Athena_SR_Ore_T03" || Name == "Pistol_Space_Athena_SR_Ore_T03" || Name == "Pistol_Gatling_Athena_SR_Ore_T03" || Name.find("Pistol_AutoHeavyPDW_Athena") != std::string::npos || Name.find("RC_Rocket_Athena") != std::string::npos || Name.find("Shotgun_BreakBarrel_Athena") != std::string::npos)
                continue;

            auto Definition = FindWID("WID_" + Name);
            if (Definition != nullptr)
            {
                std::cout << "WID_" + Name << "\n";

                if (Name.find("_C_") != std::string::npos)
                    Game::CommonWeapons.push_back(Definition);
                else if (Name.find("_UC_") != std::string::npos)
                    Game::UnCommonWeapons.push_back(Definition);
                else if (Name.find("_R_") != std::string::npos || Name == "Shotgun_SemiAuto_Athena_VR_Ore_T03")
                    Game::RareWeapons.push_back(Definition);
                else if (Name.find("_VR") != std::string::npos)
                    Game::EpicWeapons.push_back(Definition);
                else if (Name.find("_SR") != std::string::npos)
                    Game::LegendaryWeapons.push_back(Definition);
            }
        }
    }

    auto GetWeaponLoot()
    {
        if (GetMath()->STATIC_RandomBoolWithWeight(0.59))
            return UnCommonWeapons[GetMath()->STATIC_RandomInteger(UnCommonWeapons.size())];
        else if (GetMath()->STATIC_RandomBoolWithWeight(0.35))
            return RareWeapons[GetMath()->STATIC_RandomInteger(RareWeapons.size())];
        else if (GetMath()->STATIC_RandomBoolWithWeight(0.12))
            return EpicWeapons[GetMath()->STATIC_RandomInteger(EpicWeapons.size())];
        else if (GetMath()->STATIC_RandomBoolWithWeight(0.5))
            return LegendaryWeapons[GetMath()->STATIC_RandomInteger(LegendaryWeapons.size())];
        else
            return CommonWeapons[GetMath()->STATIC_RandomInteger(CommonWeapons.size())];
    }

    auto GetRandomMaterial()
    {
        if (GetMath()->STATIC_RandomBoolWithWeight(0.44))
            return Materials[1];
        else if (GetMath()->STATIC_RandomBoolWithWeight(0.22))
            return Materials[2];
        else
            return Materials[0];
    }

    auto GetSupplyDropLoot()
    {
        if (GetMath()->STATIC_RandomBoolWithWeight(0.60))
            return EpicWeapons[GetMath()->STATIC_RandomInteger(EpicWeapons.size())];
        else
            return LegendaryWeapons[GetMath()->STATIC_RandomInteger(LegendaryWeapons.size())];
    }

    void Start()
    {
        GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"open athena_terrain", GetPlayerController());
        //GetPlayerController()->SwitchLevel(L"Athena_Terrain?game=/Game/Athena/Athena_GameMode.Athena_GameMode_C");
        bTraveled = true;
    }

    void OnReadyToStartMatch()
    {
        GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
        auto GameMode = reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode);
        auto GameInstance = reinterpret_cast<UFortGameInstance*>(GetWorld()->OwningGameInstance);
        SoloPlaylist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultSolo.Playlist_DefaultSolo");
        static auto DuoPlaylist = UObject::FindObject<UFortPlaylistAthena>("FortPlaylistAthena Playlist_DefaultDuo.Playlist_DefaultDuo");
        auto InProgress = GetKismetString()->STATIC_Conv_StringToName(L"InProgress");

        GameState->bGameModeWillSkipAircraft = true;
        GameState->AircraftStartTime = 9999.9f;
        GameState->WarmupCountdownEndTime = 99999.9f;

        GameState->GamePhase = EAthenaGamePhase::Warmup;
        GameState->OnRep_GamePhase(EAthenaGamePhase::None);

        GameMode->bDisableGCOnServerDuringMatch = true;
        GameMode->bAllowSpectateAfterDeath = true;

        GameMode->MatchState = InProgress;
        GameMode->K2_OnSetMatchState(InProgress);

        auto Playlist = SoloPlaylist;

        if (Playlist)
        {
            Playlist->bNoDBNO = true;
            Playlist->FriendlyFireType = EFriendlyFireType::On;
            Playlist->RespawnLocation = EAthenaRespawnLocation::Air;
            Playlist->RespawnType = EAthenaRespawnType::None;
            Playlist->SafeZoneStartUp = ESafeZoneStartUp::StartsWithAirCraft;

            GameState->CurrentPlaylistData = Playlist;
            GameState->OnRep_CurrentPlaylistData();
        }

        GameMode->FriendlyFireType = EFriendlyFireType::Off;

        GameMode->StartPlay();
        GameState->DefaultBattleBus = UObject::FindObject<UAthenaBattleBusItemDefinition>("AthenaBattleBusItemDefinition BBID_PurpleBus.BBID_PurpleBus");
        GameState->bReplicatedHasBegunPlay = true;
        GameState->OnRep_ReplicatedHasBegunPlay();

        GameMode->StartMatch();
        GameMode->bAlwaysDBNO = false;

        GetWorld()->NetworkManager->NetCullDistanceSquared *= 3;
        if (TimesInGame <= 1)
        {
            //TODO; make this WAY WAY better, this is dogshit.
            if (bLooting)
                InitLoot();
            static auto RarePumpShotgun = FindWID("WID_Shotgun_Standard_Athena_UC_Ore_T03");
            static auto RareTacticalShotgun = FindWID("WID_Shotgun_SemiAuto_Athena_VR_Ore_T03");
            static auto LegendaryHeavyShotgun = FindWID("WID_Shotgun_SlugFire_Athena_SR");
            static auto EpicHeavyShotgun = FindWID("WID_Shotgun_SlugFire_Athena_VR");
            static auto LegendaryDoubleBarrelShotgun = FindWID("WID_Shotgun_BreakBarrel_Athena_SR_Ore_T03");
            static auto EpicDoubleBarrelShotgun = FindWID("WID_Shotgun_BreakBarrel_Athena_VR_Ore_T03");
            static auto CompactSMG = FindWID("WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03");
            static auto RareTacSMG = FindWID("WID_Pistol_Scavenger_Athena_R_Ore_T03");
            static auto EpicTacSMG = FindWID("WID_Pistol_Scavenger_Athena_VR_Ore_T03");
            static auto RareSilencedSMG = FindWID("WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03");
            static auto RareOGSMG = FindWID("WID_Pistol_AutoHeavy_Athena_R_Ore_T03");
            static auto RareAR = FindWID("WID_Assault_Auto_Athena_R_Ore_T03");
            static auto EpicAR = FindWID("WID_Assault_AutoHigh_Athena_VR_Ore_T03");
            static auto LegendaryAR = FindWID("WID_Assault_AutoHigh_Athena_SR_Ore_T03");
            EpicWeapons.push_back(EpicAR);
            LegendaryWeapons.push_back(LegendaryAR);
            static auto CommonBurstAR = FindWID("WID_Assault_SemiAuto_Athena_C_Ore_T02");
            static auto UncommonBurstAR = FindWID("WID_Assault_SemiAuto_Athena_UC_Ore_T03");
            static auto RareBurstAR = FindWID("WID_Assault_SemiAuto_Athena_R_Ore_T03");
            static auto BoogieBomb = FindWID("Athena_DanceGrenade");
            static auto Clinger = FindWID("Athena_StickyGrenade");
            static auto StinkBomb = FindWID("Athena_KnockGrenade");
            //static auto StinkBomb = FindWID("Athena_GasGrenade");
            static auto ImpulseGrenade = FindWID("Athena_KnockGrenade");
            static auto Bush = FindWID("Athena_Bush");
            static auto SmokeBomb = FindWID("Athena_SmokeGrenade");
            static auto Bandage = FindWID("Athena_Bandage");
            static auto Medkit = FindWID("Athena_Medkit");
            static auto Slurp = FindWID("Athena_PurpleStuff");
            static auto MiniShield = FindWID("Athena_ShieldSmall");
            static auto BigShield = FindWID("Athena_Shields");
            static auto ChugJug = FindWID("Athena_SuperMedkit");
            static auto Wood = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
            static auto Stone = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
            static auto Metal = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");

            /* Floor Loot (its shit)
            static auto FloorLoot = UObject::FindClass("BlueprintGeneratedClass Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");
            TArray<AActor*> Actors;
            GetGameplayStatics()->STATIC_GetAllActorsOfClass(GetWorld(), FloorLoot, &Actors);
            for (int i = 0; i < Actors.Num(); i++)
            {
                auto Loc = Actors[i]->K2_GetActorLocation();
                auto Loot = GetWeaponLoot();
                SummonPickup(nullptr, Loot, 1, Loc);
                SummonPickup(nullptr, Loot->GetAmmoWorldItemDefinition_BP(), 1, Loc);
            }
            */
            AllConsumables = {
                ImpulseGrenade,
                Bush,
                SmokeBomb,
                Bandage,
                Medkit,
                Slurp,
                MiniShield,
                BigShield,
                ChugJug
            };
            Shotguns = {
                RarePumpShotgun,
                RareTacticalShotgun,
                LegendaryHeavyShotgun,
                EpicHeavyShotgun,
                EpicDoubleBarrelShotgun,
                LegendaryDoubleBarrelShotgun,
            };
            SMGs = {
                CompactSMG,
                RareTacSMG,
                EpicTacSMG,
                RareSilencedSMG,
                RareOGSMG
            };
            Rifles = {
                RareAR,
                EpicAR,
                LegendaryAR,
                CommonBurstAR,
                UncommonBurstAR,
                RareBurstAR
            };
            FunConsumables = {
                BoogieBomb,
                Clinger,
                StinkBomb,
                ImpulseGrenade,
                Bush,
                SmokeBomb
            };
            Heals = {
                Bandage,
                Medkit,
                Slurp,
                MiniShield,
                BigShield,
                ChugJug
            };
            Materials = {
                Wood,
                Stone,
                Metal
            };
        }
    }
}









/* 
auto LootTierData = GameInstance->AthenaDataTables.LootTierData;
auto LootTierData = SoftObjectToObject(Playlist->LootTierData);

std::cout << "LootTierData: " << LootTierData << '\n';

if (LootTierData)
    std::cout << "LootTierData Name: " << LootTierData->GetFullName() << '\n';

if (LootTierData && LootTierData->Class)
    std::cout << "LootTierData Class: " << LootTierData->Class->GetFullName() << '\n';
	
*/