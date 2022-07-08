#pragma once

#include "gui.h"
#include "ufunctionhooks.h"
#include "framework.h"
#include <chrono>
#include <thread>
#include <map>
#include <chrono>
#include <random>
// #define LOGGING



#define HEALTH 100
#define SHIELD 100
namespace Hooks
{

    
    bool LocalPlayerSpawnPlayActor(ULocalPlayer* Player, const FString& URL, FString& OutError, UWorld* World) // prevent server's pc from spawning
    {
        if (bTraveled)
            return true;
        else
            return Native::LocalPlayer::SpawnPlayActor(Player, URL, OutError, World);
    }

    uint64 GetNetMode(UWorld* World) // PlayerController::SendClientAdjustment checks if the netmode is not client
    {
        return 2; // ENetMode::NM_ListenServer;
    }

    void TickFlush(UNetDriver* NetDriver, float DeltaSeconds)
    {
        if (!NetDriver)
            return;

		if (bMapFullyLoaded)
        {
            if (NetDriver->IsA(UIpNetDriver::StaticClass()) && NetDriver->ClientConnections.Num() > 0 && NetDriver->ClientConnections[0]->InternalAck == false)
            {
                Replication::ServerReplicateActors(NetDriver);
            }
        }

        Native::NetDriver::TickFlush(NetDriver, DeltaSeconds);
    }

    void WelcomePlayer(UWorld* World, UNetConnection* IncomingConnection)
    {
        Native::World::WelcomePlayer(GetWorld(), IncomingConnection);
    }

    char KickPlayer(__int64 a1, __int64 a2, __int64 a3)
    {
        return 0;
    }

    void World_NotifyControlMessage(UWorld* World, UNetConnection* Connection, uint8 MessageType, void* Bunch)
    {
        Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch);
    }

    auto ApplyTeam() {
        if (!hasSetup)
        {
            hasSetup = true;
            for (int i = 0; i < 104; i++)
            {
                teamsmap.insert_or_assign((EFortTeam)i, false);

            }
            for (auto team : teamsmap)
            {
                if (team.second)
                    continue;
                teamsmap.insert_or_assign(team.first, true);
                return team.first;
            }
        }
    }
    
    
    
    auto ChooseRandomPickaxeIndex()
    {
        int Index = Index = rand() % 42 + 1;
        //if (Index == 42)
            //Index -= 2;

        return Index;
    }

    /* auto ChooseRandomAssaultRifle()
    {
        int Index2 = Index2 = rand() % 17 +1 ; // one more than actual number.
       // if (Index2 == 17)
           // Index2 -= 2;

        return Index2;
    }
    auto ChooseRandomShotgun()
    {
        int Index3 = Index3 = rand() % 7 + 1;
       // if (Index3 == 7)
        //    Index3 -= 2;

        return Index3;
    }
    auto ChooseRandomSniper()
    {
        int Index4 = Index4 = rand() % 18 + 1;
       // if (Index4 == 18)
        //    Index4 -= 2;

        return Index4;
    }
    auto ChooseRandomFourthSlot()
    {
        int Index5 = Index5 = rand() % 5 + 1;
       // if (Index5 == 5)
        //    Index5 -= 2;

        return Index5;
    }
    auto ChooseRandomFifthSlot()
    {
        int Index6 = Index6 = rand() % 4 + 1;
      //  if (Index6 == 4)
       //     Index6 -= 2;

        return Index6;
    }*/
        
    
    

    
    

    APlayerController* SpawnPlayActor(UWorld* World, UPlayer* NewPlayer, ENetRole RemoteRole, FURL& URL, void* UniqueId, SDK::FString& Error, uint8 NetPlayerIndex)
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Native::World::SpawnPlayActor(GetWorld(), NewPlayer, RemoteRole, URL, UniqueId, Error, NetPlayerIndex);
        NewPlayer->PlayerController = PlayerController;

        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;

        InitInventory(PlayerController, true);

        auto Pawn = (APlayerPawn_Athena_C*)SpawnActorTrans(APlayerPawn_Athena_C::StaticClass(), GetPlayerStart(PlayerController), PlayerController);

        PlayerController->Pawn = Pawn;
        PlayerController->AcknowledgedPawn = Pawn;
        Pawn->Owner = PlayerController;
        Pawn->OnRep_Owner();
        PlayerController->OnRep_Pawn();
        PlayerController->Possess(Pawn);

        constexpr static auto Health = 100;
        const static auto Shield = 100;
        Pawn->SetMaxHealth(HEALTH);
        Pawn->SetMaxShield(SHIELD);
        Pawn->HealthRegenGameplayEffect = nullptr;
        Pawn->HealthRegenDelayGameplayEffect = nullptr;

        PlayerController->bHasClientFinishedLoading = true; 
        PlayerController->bHasServerFinishedLoading = true;
        PlayerController->bHasInitiallySpawned = true;
        PlayerController->OnRep_bHasServerFinishedLoading();

        PlayerState->bHasFinishedLoading = true;
        PlayerState->bHasStartedPlaying = true;
        PlayerState->OnRep_bHasStartedPlaying();



        static auto FortRegisteredPlayerInfo = ((UFortGameInstance*)GetWorld()->OwningGameInstance)->RegisteredPlayers[0]; // UObject::FindObject<UFortRegisteredPlayerInfo>("FortRegisteredPlayerInfo Transient.FortEngine_0_1.FortGameInstance_0_1.FortRegisteredPlayerInfo_0_1");

        if (FortRegisteredPlayerInfo)
        {
            auto Hero = FortRegisteredPlayerInfo->AthenaMenuHeroDef;

            if (Hero)
            {
                if (bCosmetics)
                {
                    PlayerState->HeroType = Hero->GetHeroTypeBP();
                    PlayerState->OnRep_HeroType();

                    for (auto i = 0; i < Hero->CharacterParts.Num(); i++)
                    {
                        auto Part = Hero->CharacterParts[i];

                        if (!Part)
                            continue;

                        PlayerState->CharacterParts[i] = Part;
                    }

                    PlayerState->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
                    Pawn->CharacterBodyType = Hero->CharacterParts[1]->BodyTypesPermitted;
                    Pawn->CharacterGender = Hero->CharacterParts[1]->GenderPermitted;
                    PlayerState->OnRep_CharacterBodyType();
                    PlayerState->OnRep_CharacterParts();
                }
                else
                {
                    UFortHeroType* HeroType = Hero->GetHeroTypeBP(); // UObject::FindObject<UFortHeroType>("FortHeroType HID_Outlander_015_F_V1_SR_T04.HID_Outlander_015_F_V1_SR_T04");
                    PlayerState->HeroType = HeroType;
                    PlayerState->OnRep_HeroType();

                    static auto Head = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
                    static auto Body = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");

                    PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Head] = Head;
                    PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Body] = Body;
                    PlayerState->OnRep_CharacterParts();
                }
            }
        }
        static std::vector<std::string> PickaxePool = {
            "WID_Harvest_Pickaxe_Prismatic",
            "WID_Harvest_Pickaxe_Anchor_Athena",
            "WID_Harvest_Pickaxe_ArtDeco",
            "WID_Harvest_Pickaxe_Assassin",
            "WID_Harvest_Pickaxe_Assassin",
            "WID_Harvest_Pickaxe_BoltOn_Athena_C_T01",
            "WID_Harvest_Pickaxe_Brite",
            "WID_Harvest_Pickaxe_Carrot",
            "WID_Harvest_Pickaxe_CircuitBreaker",
            "WID_Harvest_Pickaxe_CuChulainn",
            "WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01",
            "WID_Harvest_Pickaxe_Disco_Athena",
            "WID_Harvest_Pickaxe_District_Athena",
            "WID_Harvest_Pickaxe_Dragon",
            "WID_Harvest_Pickaxe_Flamingo_Athena_C_T01",
            "WID_Harvest_Pickaxe_Heart_Athena",
            "WID_Harvest_Pickaxe_HolidayCandyCane_Athena",
            "WID_Harvest_Pickaxe_HolidayGiftWrap_Athena",
            "WID_Harvest_Pickaxe_IcePick_Athena_C_T01",
            "WID_Harvest_Pickaxe_Keg_Athena",
            "WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01",
            "WID_Harvest_Pickaxe_Medieval_Athena",
            "WID_Harvest_Pickaxe_Megalodon_Athena",
            "WID_Harvest_Pickaxe_PajamaParty",
            "WID_Harvest_Pickaxe_Pizza",
            "WID_Harvest_Pickaxe_Plunger",
            "WID_Harvest_Pickaxe_PotOfGold",
            "WID_Harvest_Pickaxe_Prismatic",
            "WID_Harvest_Pickaxe_RockerPunk",
            "WID_Harvest_Pickaxe_Scavenger",
            "WID_Harvest_Pickaxe_Shark_Athena",
            "WID_Harvest_Pickaxe_SickleBat_Athena_C_T01",
            "WID_Harvest_Pickaxe_SkiBoot",
            "WID_Harvest_Pickaxe_Smiley_Athena_C_T01",
            "WID_Harvest_Pickaxe_Space",
            "WID_Harvest_Pickaxe_Spikey_Athena_C_T01",
            "WID_Harvest_Pickaxe_Squeak",
            "WID_Harvest_Pickaxe_Stealth",
            "WID_Harvest_Pickaxe_Tactical",
            "WID_Harvest_Pickaxe_TacticalBlack",
            "WID_Harvest_Pickaxe_TacticalUrban",
            "WID_Harvest_Pickaxe_Teslacoil_Athena",
            "WID_Harvest_Pickaxe_WinterCamo_Athena"
        };
        

        static std::vector<std::string> AssaultRiflePool = {
                //"WID_Assault_Auto_Athena_C_Ore_T03",
                //"WID_Assault_Auto_Athena_UC_Ore_T03",
                "WID_Assault_Auto_Athena_R_Ore_T03",
                "WID_Assault_AutoHigh_Athena_VR_Ore_T03",
                "WID_Assault_AutoHigh_Athena_SR_Ore_T03",
                //"WID_Assault_SemiAuto_Athena_C_Ore_T02",
                "WID_Assault_SemiAuto_Athena_R_Ore_T03",
                //"WID_Assault_SemiAuto_Athena_UC_Ore_T03",
                //"WID_Pistol_AutoHeavy_Athena_C_Ore_T02",
                "WID_Pistol_AutoHeavy_Athena_R_Ore_T03",
                //"WID_Pistol_AutoHeavy_Athena_UC_Ore_T03",
                //"WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02",
                "WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03",
                //"WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03",
                "WID_Pistol_Scavenger_Athena_R_Ore_T03",
                //"WID_Pistol_Scavenger_Athena_UC_Ore_T03",
                "WID_Pistol_Scavenger_Athena_VR_Ore_T03"

            };
        static std::vector<std::string> ShotgunPool = {
            //"WID_Shotgun_SemiAuto_Athena_R_Ore_T03",
            //"WID_Shotgun_SemiAuto_Athena_UC_Ore_T03",
            "WID_Shotgun_SemiAuto_Athena_VR_Ore_T03",
            "WID_Shotgun_SlugFire_Athena_SR",
            "WID_Shotgun_SlugFire_Athena_VR",
            "WID_Shotgun_Standard_Athena_C_Ore_T03",
            "WID_Shotgun_Standard_Athena_UC_Ore_T03"
        };
        static std::vector<std::string> SniperPool = {
            "WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03",
            "WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",
            "WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03",
            "WID_Sniper_Crossbow_Athena_R_Ore_T03",
            "WID_Sniper_Crossbow_Athena_VR_Ore_T03",
            "WID_Sniper_NoScope_Athena_R_Ore_T03",
            //"WID_Sniper_NoScope_Athena_UC_Ore_T03",
            "WID_Sniper_Standard_Scope_Athena_SR_Ore_T03",
            "WID_Sniper_Standard_Scope_Athena_VR_Ore_T03",
            //"WID_Pistol_SixShooter_Athena_C_Ore_T02",
            "WID_Pistol_SixShooter_Athena_R_Ore_T03",
            //"WID_Pistol_SixShooter_Athena_UC_Ore_T03",
            "WID_Assault_LMG_Athena_SR_Ore_T03",
            "WID_Assault_LMG_Athena_VR_Ore_T03",
            "WID_Assault_LMGSAW_Athena_R_Ore_T03",
            "WID_Assault_LMGSAW_Athena_VR_Ore_T03",
            "WID_Pistol_HandCannon_Athena_SR_Ore_T03",
            "WID_Pistol_HandCannon_Athena_VR_Ore_T03"
        };
        static std::vector<std::string> FourthSlotPool = {
            "Athena_Bush",
            "Athena_DanceGrenade",
            "Athena_GasGrenade",
            "Athena_KnockGrenade",
            "Athena_StickyGrenade"

        };

        static std::vector<std::string> FifthSlotPool = {
            "Athena_PurpleStuff",
            "Athena_Medkit",
            "Athena_Bandage",
            "Athena_SuperMedkit"
        };

        /* std::vector<UFortWeaponRangedItemDefinition*> FortRLoadout1 = {
            Pickaxe,
            FindWID("WID_Assault_AutoHigh_Athena_VR_Ore_T03"),
            FindWID("WID_Shotgun_Standard_Athena_UC_Ore_T03"),
            FindWID("WID_Pistol_HandCannon_Athena_VR_Ore_T03"),
            FindWID("Athena_Bush"),
            FindWID("Athena_Bandage")
        };
        EquipLoadout(PlayerController, FortRLoadout1);
        std::vector<UFortWeaponRangedItemDefinition*> FortRLoadout2 = {
            Pickaxe,
            FindWID("WID_Pistol_Scavenger_Athena_UC_Ore_T03"),
            FindWID("WID_Shotgun_SlugFire_Athena_SR"),
            FindWID("WID_Assault_Auto_Athena_R_Ore_T03"),
            FindWID("Athena_DanceGrenade"),
            FindWID("Athena_PurpleStuff")
        };
        EquipLoadout(PlayerController, FortRLoadout2);
        std::vector<UFortWeaponRangedItemDefinition*> FortRLoadout3 = {
            Pickaxe,
            FindWID("WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03"),
            FindWID("WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"),
            FindWID("WID_Assault_SemiAuto_Athena_R_Ore_T03"),
            FindWID("Athena_GasGrenade"),
            FindWID("Athena_SuperMedkit")
        };
        EquipLoadout(PlayerController, FortRLoadout3);
        std::vector<UFortWeaponRangedItemDefinition*> FortRLoadout3 = {
            Pickaxe,
            FindWID("WID_Assault_Auto_Athena_UC_Ore_T03"),
            FindWID("WID_Shotgun_Standard_Athena_UC_Ore_T03"),
            FindWID("WID_Sniper_NoScope_Athena_R_Ore_T03"),
            FindWID("Athena_KnockGrenade"),
            FindWID("Athena_Medkit")
        };
        EquipLoadout(PlayerController, FortRLoadout3);*/
        




         static auto Default = FindWID("WID_Harvest_Pickaxe_Athena_C_T01");
        auto RandomPickaxe = FindWID(PickaxePool[rand() % PickaxePool.size()]); //making every player have a unique random pickaxe
        UFortWeaponRangedItemDefinition* Pickaxe;
        Pickaxe = bCosmetics ? RandomPickaxe : Default;

        auto RandomAssaultRifle = FindWID(AssaultRiflePool[rand() % AssaultRiflePool.size()]); //making every player have a unique random AR
        UFortWeaponRangedItemDefinition* AssaultRifle;
        AssaultRifle = RandomAssaultRifle;

        auto RandomShotgun = FindWID(ShotgunPool[rand() % ShotgunPool.size()]); //making every player have a unique random Shotgun
        UFortWeaponRangedItemDefinition* Shotgun;
        Shotgun = RandomShotgun;

        auto RandomSniper = FindWID(SniperPool[rand() % SniperPool.size()]); //making every player have a unique random Sniper
        UFortWeaponRangedItemDefinition* Sniper;
        Sniper = RandomSniper;

        auto RandomFourthSlot = FindWID(FourthSlotPool[rand() % FourthSlotPool.size()]); //making every player have a unique random FourthSlot
        UFortWeaponRangedItemDefinition* FourthSlot;
        FourthSlot = RandomFourthSlot;

        auto RandomFifthSlot = FindWID(FifthSlotPool[rand() % FifthSlotPool.size()]); //making every player have a unique random FifthSlot
        UFortWeaponRangedItemDefinition* FifthSlot;
        FifthSlot = RandomFifthSlot;

        

        

        //UFortWeaponRangedItemDefinition* Pickaxe = bCosmetics ? FindWID(PickaxePool[Index]) : FindWID("WID_Harvest_Pickaxe_Athena_C_T01");
        /* UFortWeaponRangedItemDefinition* RAssaultRifle = FindWID(AssaultRiflePool[Index2]);
        UFortWeaponRangedItemDefinition* RShotgun = FindWID(ShotgunPool[Index3]);
        UFortWeaponRangedItemDefinition* RSniper = FindWID(SniperPool[Index4]);
        UFortWeaponRangedItemDefinition* RFourSlot = FindWID(FourthSlotPool[Index5]);
        UFortWeaponRangedItemDefinition* RFiveSlot = FindWID(FifthSlotPool[Index6]);*/


        
        


         

        switch (loadoutToUse)
        {
            case WeaponLoadout::REGULAR:
            {
                static auto Shotgun = FindWID("WID_Shotgun_Standard_Athena_UC_Ore_T03");
                static auto SMG = FindWID("WID_Pistol_AutoHeavy_Athena_R_Ore_T03");
                static auto Rifle = FindWID("WID_Assault_AutoHigh_Athena_SR_Ore_T03");
                static auto Sniper = FindWID("WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03");
                static auto Consumable = FindWID("Athena_PurpleStuff");
                std::vector<UFortWeaponRangedItemDefinition*> FortLoadout = {
                    Pickaxe,
                    SMG,
                    Shotgun,
                    Rifle,
                    Sniper,
                    Consumable
                };
                
                EquipLoadout(PlayerController, FortLoadout);
                //EquipLoadout(PlayerController, FortGliderLoadout);
                break;
            }
            case WeaponLoadout::EXPLOSIVES:
            {
                static auto Rocket1 = FindWID("WID_Launcher_Rocket_Athena_R_Ore_T03");
                static auto Rocket2 = FindWID("WID_Launcher_Rocket_Athena_R_Ore_T03");
                static auto GrenadeLauncher = FindWID("WID_Launcher_Grenade_Athena_SR_Ore_T03");
                static auto C4 = FindWID("Athena_C4");
                static auto Consumable = FindWID("Athena_PurpleStuff");
                std::vector<UFortWeaponRangedItemDefinition*> FortLoadout = {
                    Pickaxe,
                    Rocket1,
                    Rocket2,
                    GrenadeLauncher,
                    C4,
                    Consumable
                };
                EquipLoadout(PlayerController, FortLoadout);
                break;
            }
            case WeaponLoadout::SNIPERS:
            {
                static auto BoltAction = FindWID("WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03");
                static auto Automatic = FindWID("WID_Sniper_Standard_Scope_Athena_SR_Ore_T03");
                static auto Hunting = FindWID("WID_Sniper_NoScope_Athena_R_Ore_T03");
                static auto Crossbow = FindWID("WID_Sniper_Crossbow_Athena_VR_Ore_T03");
                static auto Consumable = FindWID("Athena_PurpleStuff");
                std::vector<UFortWeaponRangedItemDefinition*> FortLoadout = {
                    Pickaxe,
                    BoltAction,
                    Automatic,
                    Hunting,
                    Crossbow,
                    Consumable
                };
                EquipLoadout(PlayerController, FortLoadout);
                break;
            }
            case WeaponLoadout::RANDOM:
            {
                std::vector<UFortWeaponRangedItemDefinition*> FortLoadout = {
                    Pickaxe,
                    AssaultRifle,
                    Shotgun,
                    Sniper,
                    FourthSlot,
                    FifthSlot
                };
                EquipLoadout(PlayerController, FortLoadout);
                break;
            }
        }
            


            

        auto CheatManager = CreateCheatManager(PlayerController);
        CheatManager->ToggleInfiniteAmmo();
        CheatManager->ToggleInfiniteDurability();
        if (reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState)->GamePhase == EAthenaGamePhase::Warmup)
            CheatManager->God();
            

        if (PlayerController->Pawn)
        {
            if (PlayerController->Pawn->PlayerState)
            {
                PlayerState->TeamIndex = ApplyTeam(); //EFortTeam(rand() % 101);
                PlayerState->OnRep_PlayerTeam();
            }
        }

        PlayerController->OverriddenBackpackSize = 100;

        if (Pawn && Pawn->AbilitySystemComponent)
        {
            ApplyAbilities(Pawn);
        }

        auto Drone = (ABP_VictoryDrone_C*)SpawnActor(ABP_VictoryDrone_C::StaticClass(), Pawn->K2_GetActorLocation());
        Drone->TriggerPlayerSpawnEffects();

        return PlayerController;
    }

    void Beacon_NotifyControlMessage(AOnlineBeaconHost* Beacon, UNetConnection* Connection, uint8 MessageType, int64* Bunch)
    {

        if (((AAthena_GameState_C*)GetWorld()->GameState)->GamePhase != EAthenaGamePhase::Warmup && !bPlayground)
        {
            KickController(Connection->PlayerController, L"Game has already started.");
            printf("LogRaider: Player tried to join, but cannot because the game has already started.\n");
            return;
        }

        printf("Recieved control message %i\n", MessageType);

        switch (MessageType)
        {
        case 4: // NMT_Netspeed
            Connection->CurrentNetSpeed = 30000;
            return;
        case 5: // NMT_Login
        {
            Bunch[7] += (16 * 1024 * 1024);

            auto OnlinePlatformName = FString(L"");

            Native::NetConnection::ReceiveFString(Bunch, Connection->ClientResponse);
            Native::NetConnection::ReceiveFString(Bunch, Connection->RequestURL);
            Native::NetConnection::ReceiveUniqueIdRepl(Bunch, Connection->PlayerID);
            Native::NetConnection::ReceiveFString(Bunch, OnlinePlatformName);

            Bunch[7] -= (16 * 1024 * 1024);

            Native::World::WelcomePlayer(GetWorld(), Connection);
			
            return;
        }
        case 15: // NMT_PCSwap
            // return;
            break;
        }

        Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch);
    }

    uint8 Beacon_NotifyAcceptingConnection(AOnlineBeacon* Beacon)
    {
        return Native::World::NotifyAcceptingConnection(GetWorld());
    }

    void* SeamlessTravelHandlerForWorld(UEngine* Engine, UWorld* World)
    {
        return Native::Engine::SeamlessTravelHandlerForWorld(Engine, GetWorld());
    }

    void* NetDebug(UObject* _this)
    {
        return nullptr;
    }

    void PostRender(UGameViewportClient* _this, UCanvas* Canvas)
    {
        ZeroGUI::SetupCanvas(Canvas);
        GUI::Tick();

        return Native::GameViewportClient::PostRender(_this, Canvas);
    }

    __int64 CollectGarbage(__int64 a1)
    {
        return 0;
    };

    void InitNetworkHooks()
    {
        DETOUR_START
        DetourAttachE(Native::World::WelcomePlayer, WelcomePlayer);
        DetourAttachE(Native::Actor::GetNetMode, GetNetMode);
        DetourAttachE(Native::World::NotifyControlMessage, World_NotifyControlMessage);
        DetourAttachE(Native::World::SpawnPlayActor, SpawnPlayActor);
        DetourAttachE(Native::OnlineBeaconHost::NotifyControlMessage, Beacon_NotifyControlMessage);
        DetourAttachE(Native::OnlineSession::KickPlayer, KickPlayer);
        DetourAttachE(Native::GameViewportClient::PostRender, PostRender);
        DetourAttachE(Native::GC::CollectGarbage, CollectGarbage);
        DETOUR_END
    }

    void DetachNetworkHooks()
    {
        DETOUR_START
        DetourDetachE(Native::World::WelcomePlayer, WelcomePlayer);
        DetourDetachE(Native::Actor::GetNetMode, GetNetMode);
        DetourDetachE(Native::World::NotifyControlMessage, World_NotifyControlMessage);
        DetourDetachE(Native::World::SpawnPlayActor, SpawnPlayActor);
        DetourDetachE(Native::OnlineBeaconHost::NotifyControlMessage, Beacon_NotifyControlMessage);
        DetourDetachE(Native::OnlineSession::KickPlayer, KickPlayer);
        DetourDetachE(Native::GameViewportClient::PostRender, PostRender);
        DetourDetachE(Native::GC::CollectGarbage, CollectGarbage);
        DETOUR_END
    }

    UNetConnection* SpectatorConnection;
    AFortPlayerStateAthena* ToSpectatePlayerState;

    auto UpdateSpecvars(UNetConnection* NetConnection, AFortPlayerStateAthena* PlayerState)
    {
        SpectatorConnection = NetConnection;
        ToSpectatePlayerState = PlayerState;
    }

    void ProcessEventHook(UObject* Object, UFunction* Function, void* Parameters)
    {
        if (!bPlayButton)
        {
            static auto PlayButtonFn = UObject::FindObject<UFunction>("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature");

            // if (FunctionName.find("BP_PlayButton") != -1)
            if (Function == PlayButtonFn)
            {
                bPlayButton = true;
                Game::Start();
                printf("[Game::Start] Done\n");

                InitNetworkHooks();
                printf("[InitNetworkHooks] Done\n");
            }
        }


        if (Function->GetName().find("Tick") != std::string::npos && bPlayground && PlayerBuilds.size() >= 600)
        {
            for (auto build : PlayerBuilds)
            {
                if (build != nullptr)
                    build->K2_DestroyActor();
            }
            PlayerBuilds.clear();
        }

        if (Function->GetName().find("Tick") != std::string::npos && bRestart)
        {
            bRestart = false;
            bTraveled = false;
            bPlayButton = false;
            bListening = false;
            bReadyToStart = false;
            bSpawnedFloorLoot = false;
            bMapFullyLoaded = false;
            bStartedBus = false;
            HostBeacon = nullptr;
            bCosmetics = true;
            bLoadoutRegular = true;
            bLoadoutExplosives = false;
            bLoadoutSnipers = false;
            bLoadoutRandom = false;
            bBusOnLocations = false;
            bBuildingAllowed = true;
            bRespawn = false;
            bHideAndSeek = false;
            bPlayground = false;
            bAllowStorm = true;
            bSafeZoneBased = false;
            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = false;
            SpectatorConnection = nullptr;
            ToSpectatePlayerState = nullptr;
            PlayerBuilds.clear();
            for (auto team : teamsmap)
            {
                if (team.second)
                    teamsmap.insert_or_assign(team.first, false);
                else
                    break;
            }
            DetachNetworkHooks();
            GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"open frontend", GetPlayerController());
        }
        


        if (Function->GetFullName() == "Function BP_VictoryDrone.BP_VictoryDrone_C.OnSpawnOutAnimEnded")
        {
            if (!bPlayground)
            {
                auto drone = (ABP_VictoryDrone_C*)Object;
                if (Object->IsA(ABP_VictoryDrone_C::StaticClass()))
                    drone->K2_DestroyActor();

                if (SpectatorConnection && ToSpectatePlayerState)
                    Spectate(SpectatorConnection, ToSpectatePlayerState);
            }
            else
            {
                auto drone = (ABP_VictoryDrone_C*)Object;
                if (Object->IsA(ABP_VictoryDrone_C::StaticClass()))
                {
                    if (drone->Owner)
                    {
                        auto DronePC = (AFortPlayerControllerAthena*)drone->Owner;
                        if (DronePC->NetConnection)
                            Playground().DroneAnimEnded(DronePC, drone->K2_GetActorLocation());
                    }

                    drone->K2_DestroyActor();
                }
            }
        }
        


        

        


        if (Function->GetFullName() == "Function SafeZoneIndicator.SafeZoneIndicator_C.OnSafeZoneStateChange" && bSafeZoneBased && !bPlayground)
        {
            auto Indicator = (ASafeZoneIndicator_C*)Object;
            auto SafeZonePhase = ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->SafeZonePhase;
            //static auto GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
            
            
            
            //std::cout << "SafeZonePhase: " << SafeZonePhase << "\n";
             
            switch (SafeZonePhase)
            {
            case 0:
                Indicator->Radius = 15000;
                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startshrinksafezone", nullptr);
                Indicator->NextRadius = 9500;
                Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
                break;
            case 1:
                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startshrinksafezone", nullptr);
                Indicator->NextRadius = 9500;
                Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
                break;
            case 2:
                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startshrinksafezone", nullptr);
                Indicator->NextRadius = 1000;
                Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
                break;
            case 3:
                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startshrinksafezone", nullptr);
                Indicator->NextRadius = 0;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            default:
                //Indicator->NextRadius = 5;
                break;
            }
        }
        if (Function->GetFullName() == "Function SafeZoneIndicator.SafeZoneIndicator_C.OnSafeZoneStateChange" && bSafeZoneBased && bPlayground)
        {
            auto Indicator = (ASafeZoneIndicator_C*)Object;
            auto SafeZonePhase = ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->SafeZonePhase;

            
            Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
            //std::cout << "SafeZonePhase: " << SafeZonePhase << "\n";

            switch (SafeZonePhase)
            {
            case 0:
                Indicator->Radius = 110000;
                Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
                Indicator->NextRadius = 110000;
                break;
            case 1:
                Indicator->NextRadius = 96000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 2:
                Indicator->NextRadius = 86000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 3:
                Indicator->NextRadius = 55000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 4:
                Indicator->NextRadius = 35000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 5:
                Indicator->NextRadius = 12000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 6:
                Indicator->NextRadius = 10000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 7:
                Indicator->NextRadius = 10000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 8:
                Indicator->NextRadius = 10000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            case 9:
                Indicator->NextRadius = 10000;
                Indicator->NextCenter = (FVector_NetQuantize100)CenterMagic;
                break;
            default:
                Indicator->LastRadius = 100000;
                Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
                break;
            }
        }

        if (Function->GetFullName() == "Function PlayerPawn_Athena.PlayerPawn_Athena_C.GameplayCue.Athena.OutsideSafeZone" && bSafeZoneBased)
        {
            // TODO: somehow find a better way to do this, or fix how the player gets damaged when the zone is created
            if (((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->SafeZonePhase > 0)
                ((APlayerPawn_Athena_C*)Object)->SetHealth(((APlayerPawn_Athena_C*)Object)->GetHealth() - 1);
        }
        
                          
                      
        
        if (Function->GetFullName() == "Function FortniteGame.FortPlayerControllerZone.ClientOnPawnDied")
        {
            auto Params = static_cast<AFortPlayerControllerZone_ClientOnPawnDied_Params*>(Parameters);
            auto DeadPC = (AFortPlayerControllerAthena*)Object;
            auto DeadPlayerState = (AFortPlayerStateAthena*)DeadPC->PlayerState;
            bool IsKiller;

            if (DeadPC && Params)
            {
                
                //TArray<AActor*> Pawns;
                //static auto GameplayStatics = (UGameplayStatics*)UGameplayStatics::StaticClass()->CreateDefaultObject();
                //GameplayStatics->STATIC_GetAllActorsOfClass(GetWorld(), APlayerPawn_Athena_C::StaticClass(), &Pawns);
                //auto GameState = (AAthena_GameState_C*)GetWorld()->AuthorityGameMode->GameState;
                IsKiller = Params->DeathReport.KillerPlayerState ? true : false;
                auto KillerPawn = Params->DeathReport.KillerPawn;
                auto KillerPlayerState = (AFortPlayerStateAthena*)Params->DeathReport.KillerPlayerState;

                if (!bPlayground)
                {
                    GameState->PlayersLeft--;
                    GameState->TotalPlayers--;
                    GameState->OnRep_PlayersLeft();
                }
                FVector DeadPawnLocation = DeadPC->Pawn->K2_GetActorLocation();
                auto Drone = SpawnActor(ABP_VictoryDrone_C::StaticClass(), DeadPawnLocation);
                Drone->Owner = DeadPC;
                Drone->OnRep_Owner();

               /*
                if (!bPlayground)
                    //GameState->PlayerArray.RemoveSingle(DeadPC->NetPlayerIndex);
                {
                    std::cout << DeadPC->NetPlayerIndex << "Player Index 1\n";
                    for (int i = 0; i < GameState->PlayerArray.Num(); i++)
                    {
                        if (GameState->PlayerArray[i] == DeadPlayerState)
                        {
                            std::cout << i << "Player Index 2\n";
                            GameState->PlayerArray.RemoveAt(i);
                        }
                    }
                    //GameState->PlayerArray.RemoveSingle(DeadPC->NetIndex);
                }
                */

                FDeathInfo deathInfo;
                deathInfo.bDBNO = false;
                deathInfo.DeathLocation = DeadPawnLocation;
                deathInfo.Distance = Params->DeathReport.KillerPawn ? Params->DeathReport.KillerPawn->GetDistanceTo(DeadPC->Pawn) : 0;
                deathInfo.DeathCause = Game::GetDeathCause(Params->DeathReport); // Death cause fix
                deathInfo.FinisherOrDowner = KillerPlayerState ? KillerPlayerState : DeadPlayerState;
                DeadPlayerState->DeathInfo = deathInfo;
                DeadPlayerState->OnRep_DeathInfo();

                if (IsKiller)
                {
                    KillerPlayerState->ClientReportKill(DeadPlayerState);
                    KillerPlayerState->KillScore++;
                    KillerPlayerState->TeamKillScore++;
                    KillerPlayerState->OnRep_Kills();
                    UpdateSpecvars(DeadPC->NetConnection, KillerPlayerState);
                    //Spectate(DeadPC->NetConnection, KillerPlayerState);
                }
                else
                {
                    deathInfo.DeathCause = EDeathCause::Banhammer;
                    deathInfo.FinisherOrDowner = DeadPlayerState;
                    DeadPlayerState->ClientReportKill(DeadPlayerState);
                    if (GameState->PlayersLeft > 0 && !bPlayground)
                    {
                        TArray<AFortPlayerPawn*> Pawns;
                        GetFortKismet()->STATIC_GetAllFortPlayerPawns(GetWorld(), &Pawns);
                        auto RandomPawn = (AFortPlayerPawnAthena*)Pawns[rand() % Pawns.Num()];
                        if (RandomPawn)
                            UpdateSpecvars(DeadPC->NetConnection, (AFortPlayerStateAthena*)RandomPawn->Controller->PlayerState);
                        Pawns.FreeArray();
                        //Spectate(DeadPC->NetConnection, (AFortPlayerStateAthena*)RandomPawn->Controller->PlayerState);
                    }
                }
                if (bPlayground && ((AFortPlayerPawnAthena*)DeadPC->Pawn)->IsSkydiving() && IsKiller && KillerPlayerState != DeadPC->PlayerState)
                    ((AFortPlayerControllerAthena*)KillerPawn->Controller)->ClientOnPawnDied(Params->DeathReport);

                if (DeadPC->Pawn)
                    DeadPC->Pawn->K2_DestroyActor();

                if (bPlayground && IsKiller)
                    Playground().OnDeath((AFortPlayerPawnAthena*)KillerPawn);

                if (GameState->PlayersLeft == 1 && bStartedBus)
                {
                    if (!bPlayground)
                    {
                        TArray<AActor*> Pawns;
                        static auto GameplayStatics = (UGameplayStatics*)UGameplayStatics::StaticClass()->CreateDefaultObject();
                        GameplayStatics->STATIC_GetAllActorsOfClass(GetWorld(), APlayerPawn_Athena_C::StaticClass(), &Pawns);
                        auto WinnerPawn = (AFortPlayerPawnAthena*)Pawns[0];
                        Pawns.FreeArray();
                        auto WinnerPC = (AFortPlayerControllerAthena*)WinnerPawn->Controller;
                        auto WinnerState = (AFortPlayerStateAthena*)WinnerPC->PlayerState;
                        if (!WinnerPC->IsClientNotifiedOfWin())
                        {
                            auto GameMode = (AFortGameModeAthena*)GameState->AuthorityGameMode;
                            GameState->WinningPlayerName = WinnerState->PlayerName;
                            GameState->OnRep_WinningPlayerName();
                            GameMode->ReadyToEndMatch();
                            GameMode->EndMatch();

                            reinterpret_cast<UFortCheatManager*>(WinnerPawn->Controller)->AthenaForceVictory(true);
                            WinnerPC->ClientNotifyWon();
                            WinnerPC->ClientNotifyTeamWon();
                            WinnerPC->PlayWinEffects();
                            WinnerPC->bClientNotifiedOfWin = true;
                            WinnerPC->bClientNotifiedOfTeamWin = true;
                            WinnerState->bHasWonAGame = true;
                            WinnerPC->ClientGameEnded(WinnerPawn, true);
                        }
                    }
                    //auto CM = CreateCheatManager((APlayerController*)WinnerPawn->Controller);
                    //CM->AthenaForceVictory(true);
                }
            }
        }

        if (bTraveled)
        {
#ifdef LOGGING
            auto FunctionName = Function->GetName();
            if (Function->FunctionFlags & 0x00200000 || (Function->FunctionFlags & 0x01000000 && FunctionName.find("Ack") == -1 && FunctionName.find("AdjustPos") == -1))
            {
                if (FunctionName.find("ServerUpdateCamera") == -1 && FunctionName.find("ServerMove") == -1)
                {
                    std::cout << "RPC Called: " << FunctionName << '\n';
                }
            }
#endif

            for (int i = 0; i < UFunctionHooks::toHook.size(); i++)
            {
                if (Function == UFunctionHooks::toHook[i])
                {
                    if (UFunctionHooks::toCall[i](Object, Parameters))
                    {
                        return;
                    }
                    break;
                }
            }
        }

        return ProcessEvent(Object, Function, Parameters);
    }
}
