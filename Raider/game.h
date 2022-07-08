#pragma once

#include "ue4.h"
#include "gui.h"

namespace Game
{
    void Start()
    {
        GetPlayerController()->SwitchLevel(L"Athena_Terrain?game=/Game/Athena/Athena_GameMode.Athena_GameMode_C");
        bTraveled = true;
    }

    void OnReadyToStartMatch()
    {
        GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
        auto GameMode = reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode);
        auto GameInstance = reinterpret_cast<UFortGameInstance*>(GetWorld()->OwningGameInstance);

        auto GameSomethin = reinterpret_cast<UFortGameData*>(UFortGameData::StaticClass);

        auto PlayerState = reinterpret_cast<AFortPlayerState*>(AFortPlayerState::StaticClass);

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

      
        
        
        //UObject::FindClass("BlueprintGeneratedClass GA_Athena_PurpleStuff.GA_Athena_PurpleStuff_C");
        


        auto Playlist = SoloPlaylist;

        if (Playlist)
        {
            //Playlist->SafeZoneStartUp = ESafeZoneStartUp::StartsWithAirCraft;
            
            Playlist->bNoDBNO = true;
            Playlist->FriendlyFireType = EFriendlyFireType::On;
            GameState->CurrentPlaylistData = Playlist;
            GameState->OnRep_CurrentPlaylistData();
        }

        GameMode->FriendlyFireType = EFriendlyFireType::Off;

        GameMode->StartPlay();
        GameState->DefaultBattleBus = UObject::FindObject<UAthenaBattleBusItemDefinition>("AthenaBattleBusItemDefinition BBID_PurpleBus.BBID_PurpleBus");
        GameState->bAllowUserPickedCosmeticBattleBus = true;

        

        //PlayerState->bInitializedPlayerCustomizationOptionsFromClientSettings = true;

        


        //TArray<UAthenaCosmeticItemDefinition*> Cosmetics;
        //Cosmetics.Add(UObject::FindObject<UAthenaGliderItemDefinition>("UAthenaGliderItemDefinition Glider_ID_026_ShuttleB.Glider_ID_026_ShuttleB"));


        //auto Glider = UObject::FindObject<UAthenaCosmeticItemDefinition>("AthenaGliderItemDefinition Glider_ID_026_ShuttleB.Glider_ID_026_ShuttleB");

        //GameMode->GliderCosmetics = Cosmetics;
        //GameMode->GliderLoadTestIndex = 1;

        //GameSomethin->DefaultGliderSkin = UObject::FindObject<UAthenaGliderItemDefinition>("UAthenaGliderItem Glider_ID_026_ShuttleB.Glider_ID_026_ShuttleB");

        GameState->bReplicatedHasBegunPlay = true;

        GameState->OnRep_ReplicatedHasBegunPlay();

        GameMode->StartMatch();
        GameMode->bAlwaysDBNO = false;

        


        
        //GameMode->GliderLoadTestIndex = 1;

    }

    auto GetDeathCause(FFortPlayerDeathReport DeathReport)
    {
        static std::map<std::string, EDeathCause> DeathCauses {
            { "weapon.ranged.shotgun", EDeathCause::Shotgun },
            { "weapon.ranged.assault", EDeathCause::Rifle },
            { "Gameplay.Damage.Environment.Falling", EDeathCause::FallDamage },
            { "weapon.ranged.sniper", EDeathCause::Sniper },
            { "Weapon.Ranged.SMG", EDeathCause::SMG },
            { "weapon.ranged.heavy.rocket_launcher", EDeathCause::RocketLauncher },
            { "weapon.ranged.heavy.grenade_launcher", EDeathCause::GrenadeLauncher },
            { "Weapon.ranged.heavy.grenade", EDeathCause::Grenade },
            { "Weapon.Ranged.Heavy.Minigun", EDeathCause::Minigun },
            { "Weapon.Ranged.Crossbow", EDeathCause::Bow },
            { "trap.floor", EDeathCause::Trap },
            { "weapon.ranged.pistol", EDeathCause::Pistol },
            { "Gameplay.Damage.OutsideSafeZone", EDeathCause::OutsideSafeZone },
            { "Weapon.Melee.Impact.Pickaxe", EDeathCause::Melee }
        };

        for (int i = 0; i < DeathReport.Tags.GameplayTags.Num(); i++)
        {
            auto TagName = DeathReport.Tags.GameplayTags[i].TagName.ToString();

            for (auto Map : DeathCauses)
            {
                if (TagName == Map.first)
                    return Map.second;
                else
                    continue;
            }
        }

        return EDeathCause::Unspecified;
    }
}
