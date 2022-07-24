#pragma once

#include "ZeroGUI.h"
#include <format>
#include <mutex>
#include "gungame.h"
#include "hidenseek.h"
#include "playground.h"
#include "framework.h"

bool bStartedBus = false;

// GUI VARIABLES
// game
bool bBusOnLocations = false;
bool bBuildingAllowed = true;
bool bRespawn = false;
bool bHideAndSeek = false;
bool bPlayground = false;
bool bReadyToStart = false;
bool bStormPaused = false;
bool bRestart = false;
bool bSafeZoneBased = false;
bool bLooting = true;
bool bFloorLoot = false;

FVector BusLocation;
// pregame
bool bLoadoutRandom = false;
bool bLoadoutIni = false;
bool bLoadoutExplosives = false;
bool bLoadoutSnipers = false;

AFortPlayerStateAthena* Seeker;
static UFortPlaylistAthena* SoloPlaylist;


enum class WeaponLoadout
{
    NONE,
    RANDOM,
    INI,
    EXPLOSIVES,
    SNIPERS
};

WeaponLoadout loadoutToUse = WeaponLoadout::NONE;

namespace GUI
{
    auto getRandomLocation()
    {
        static std::vector<FVector> Locations = {

            { 24426, 37710, 25000 }, // retail row
            { 50018, 73844, 25000 }, // lonely lodge
            { 39781, 61621, 25000 }, // Moisty Mire
            { 39781, 61621, 25000 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 25000 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 25000 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 25000 }, // Moisty Mire DUPLICATE
            { -26479, 41847, 20000 }, // Prison
            { -26479, 41847, 20000 }, // Prison DUPLICATE
            { -26479, 41847, 20000 }, // Prison DUPLICATE
            { 56771, 32818, 20000 }, // Containers/crates
            { -75353, -8694, 20000 }, //Lucky Landing
            { 34278, 867, 25000 }, // dusty depot / factories
            { 79710, 15677, 25000 }, // tomato town
            { 103901, -20203, 25000 }, // ANARCHY acres
            { 86766, -83071, 25000 }, // pleasant park
            { 2399, -96255, 25000 }, // greasy grove
            { -35037, -463, 25000 }, // fatal fields
            { 83375, 50856, 25000 }, // Wailing Woods
            { 35000, -60121, 25000 }, // Tilted Towers
            { 25000, -127121, 25000 }, // Snobby Shores
            { 5000, -60121, 25000 }, // shifty shafts
            { 110088, -115332, 25000 }, // Haunted Hills
            { 119126, -86354, 25000 }, // Junk Houses
            { 130036, -105092, 25000 }, // Junk Junction
            { -68000, -63521, 25000 }, // Flush Factory
            { 3502, -9183, 25000 }, // Salty Springs
            { 7760, 76702, 25000 }, //race track
            { 38374, -94726, 25000 }, //Soccer field
            { 70000, -40121, 25000 }, // Loot Lake
            //New Locations: 7/4/22
            { 117215, -53654, 25000 }, //motel
            { 117215, -53654, 25000 }, //motel DUPE
            { 106521, -69597, 25000 }, //Pleasant Park Mountain
            { 106521, -69597, 25000 }, //Pleasant Park Mountain DUPE
            { 86980, -105015, 25000 }, //Pleasant Park Mountain 2
            { 86980, -105015, 25000 }, //Pleasant Park Mountain 2 DUPE
            { 76292, -104977, 25000 }, //Haunted/Pleasant House
            { 76292, -104977, 25000 }, //Haunted/Pleasant House DUPE
            { 56131, -106880, 25000 }, //Snobby Mountain (Before Villain Lair)
            { 56131, -106880, 25000 }, //Snobby Mountain (Before Villain Lair) DUPE
            { 29197, -109347, 25000 }, //Snobby Mountain 2
            { 29197, -109347, 25000 }, //Snobby Mountain 2 DUPE
            { -29734, -60767, 25000 }, //chair
            { -29734, -60767, 25000 }, //chair DUPE
            { -19903, -26194, 25000 }, //Grandma's house
            { -19903, -26194, 25000 }, //Grandma's house DUPE
            { -26851, 16299, 25000 }, //Tunnel near Fatal Fields
            { -26851, 16299, 25000 }, //Tunnel near Fatal Fields DUPE
            { -63592, 35933, 25000 }, //Random bush circle I've never seen before
            { -63592, 35933, 25000 }, //Random bush circle I've never seen before DUPE
            { -75810, 33594, 25000 }, //Crab behind Moisty
            { -75810, 33594, 25000 }, //Crab behind Moisty DUPE
            { 28374, -94726, 25000 }, //Soccer mountain
            { 28374, -94726, 25000 }, //Soccer mountain DUPE
            { 73770, -19009, 25000 }, //Random Location 1
            { 73770, -19009, 25000 }, //Random Location 1 DUPE
            { 29050, -21225, 25000 }, //Dusty Mountain
            { 29050, -21225, 25000 }, //Dusty Mountain DUPE
            { 18325, -17881, 25000 }, //Salty Mountain
            { 18325, -17881, 25000 }, //Salty Mountain DUPE
            { 6621, 18784, 25000 }, //Random Location 2
            { 6621, 18784, 25000 }, //Random Location 2 DUPE
            { -6702, 33251, 25000 }, //Random Location 3/bridge
            { -6702, 33251, 25000 }, //Random Location 3/bridge DUPE
            //Off map
            { 137767, 40939, 25000 }, //off map near where risky would be
            { 137767, 40939, 25000 }, //off map near where risky would be DUPE
            { 136084, -46013, 25000 }, //off map near motel
            { 136084, -46013, 25000 }, //off map near motel DUPE
            { -2450, -127394, 25000 }, //off map bottom left
            { -2450, -127394, 25000 }, //off map bottom left DUPE
            { -26584, -90150, 25000 }, //off map bottom left 2
            { -26584, -90150, 25000 }, //off map bottom left 2 DUPE
            { -123778, -112480, 17525 } //Spawn Island
        };

        auto Location = Locations[rand() % Locations.size()];
        return Location;
    }

    std::mutex mtx;
    void Tick()
    {
        ZeroGUI::Input::Handle();

        static bool menu_opened = true;

        if (GetAsyncKeyState(VK_F2) & 1)
            menu_opened = !menu_opened;

        static auto pos = FVector2D { 200.f, 250.0f };

        if (ZeroGUI::Window(L"Raider || Modified by ozne#4492", &pos, FVector2D { 500.0f, 700.0f }, menu_opened))
        {
            if (bListening && HostBeacon)
            {
                /* static auto*/ GameState = (AAthena_GameState_C*)GetWorld()->GameState;
                static APlayerState* currentPlayer = nullptr;
                static int GravityIndex = 0;
                static int PlayerIndex = -1;

                // This is bad, but works for now.
                if (PlayerIndex != -1 && HostBeacon->NetDriver->ClientConnections[PlayerIndex] != nullptr)
                {
                    if (ZeroGUI::Button(L"<", { 25.0f, 25.0f }))
                    {
                        mtx.lock();
                        //currentPlayer = nullptr;
                        //smth = false;
                        PlayerIndex = -1;
                        mtx.unlock();
                    }
                    if (PlayerIndex != -1)
                    {
                        ZeroGUI::NextColumn(90.0f);

                        ZeroGUI::Text(std::format(L"Current Player: {}", HostBeacon->NetDriver->ClientConnections[PlayerIndex]->PlayerController->PlayerState->GetPlayerName().c_str()).c_str());

                        if (ZeroGUI::Button(L"Kick", { 60.0f, 25.0f }))
                        {
                            static std::wstring Reason(Utils::Ini().Get("Players", "DefaultKickText", "You have been kicked by the server.").begin(), Utils::Ini().Get("Players", "DefaultKickText", "You have been kicked by the server.").end());
                            KickController(HostBeacon->NetDriver->ClientConnections[PlayerIndex]->PlayerController, Reason.c_str());

                            mtx.lock();
                            PlayerIndex = -1;
                            mtx.unlock();
                        }

                        if (ZeroGUI::Button(L"Destroy Player's Builds", { 60, 25 }))
                        {
                            for (auto build : PlayerBuilds)
                            {
                                if (build->Team == ((AFortPlayerStateAthena*)HostBeacon->NetDriver->ClientConnections[PlayerIndex]->PlayerController->PlayerState)->TeamIndex)
                                    build->K2_DestroyActor();
                                else
                                    continue;
                            }
                        }

                        if (ZeroGUI::Button(L"Force Win", {60,25}))
                        {
                            auto WinnerState = (AFortPlayerStateAthena*)HostBeacon->NetDriver->ClientConnections[PlayerIndex]->PlayerController->PlayerState;
                            auto WinnerPawn = (AFortPlayerPawnAthena*)WinnerState->GetCurrentPawn();
                            auto WinnerPC = (AFortPlayerControllerAthena*)WinnerPawn->Controller;
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

                        if (bHideAndSeek)
                        {
                            if (ZeroGUI::Button(L"Make Seeker?", { 60.0f, 25.0f }))
                                Seeker = (AFortPlayerStateAthena*)HostBeacon->NetDriver->ClientConnections[PlayerIndex]->PlayerController->PlayerState;
                        }
                    }
                }
                else
                {
                    static int tab = 0;
                    if (ZeroGUI::ButtonTab(L"Game", FVector2D { 110, 25 }, tab == 0))
                        tab = 0;
                    if (ZeroGUI::ButtonTab(L"Players", FVector2D { 110, 25 }, tab == 1))
                        tab = 1;

                    ZeroGUI::NextColumn(130.0f);

                    switch (tab)
                    {
                    case 0:
                    {
                        if (!bStartedBus)
                        {
                            if (ZeroGUI::Button(L"Start Bus", FVector2D { 100, 25 }))
                            {
                                if (HostBeacon->NetDriver->ClientConnections.Num() <= 0)
                                    return;

                                if (!bBusOnLocations)
                                    bSafeZoneBased = false;

                                if (static_cast<AAthena_GameState_C*>(GetWorld()->GameState)->GamePhase >= EAthenaGamePhase::Aircraft)
                                {
                                    LOG_INFO("[LogRaider] The bus has already started.");
                                    bStartedBus = true;
                                }

                                for (auto build : PlayerBuilds)
                                {
                                    if (build)
                                        build->K2_DestroyActor();
                                }
                                PlayerBuilds.clear();

                                GameState->bGameModeWillSkipAircraft = false;
                                GameState->AircraftStartTime = 0;
                                GameState->WarmupCountdownEndTime = 0;

                                static auto Kismet = GetKismetSystem();
                                Kismet->STATIC_ExecuteConsoleCommand(GetWorld(), L"startaircraft", nullptr);

                                if (bBusOnLocations || bHideAndSeek)
                                {
                                    auto RandomLocation = getRandomLocation();
                                    BusLocation = RandomLocation;
                                    GameState->AircraftStartTime = 0;
                                    GameState->GetAircraft(0)->FlightStartTime = 0;
                                    GameState->GetAircraft(0)->DropStartTime = 0;
                                    GameState->GetAircraft(0)->FlightInfo.TimeTillDropStart = 0;
                                    GameState->bAircraftIsLocked = false;
                                    GameState->GetAircraft(0)->FlightInfo.FlightStartLocation = FVector_NetQuantize100(RandomLocation);
                                    GameState->GetAircraft(0)->FlightInfo.FlightSpeed = 0;
                                    if (bHideAndSeek)
                                        HideAndSeek().InitializeHideAndSeek();
                                }

                                if (bPlayground)
                                    Playground().InitializePlayground(GameState);

                                LOG_INFO("[LogRaider] The bus has been started.");
                                bStartedBus = true;
                            }

                            if (!bHideAndSeek)
                                ZeroGUI::Checkbox(L"Spawn bus on a random location?", &bBusOnLocations);

                            if (bBusOnLocations && !bHideAndSeek)
                                ZeroGUI::Checkbox(L"Base storm around random location?", &bSafeZoneBased);
                        }


                        ZeroGUI::Checkbox(L"Allow players to build?", &bBuildingAllowed);
                        
                        ZeroGUI::Checkbox(L"Safe Zone Paused?", &((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused);

                        if (ZeroGUI::Button(L"Destroy All Builds", FVector2D { 100, 25 }))
                        {
                            for (auto build : PlayerBuilds)
                            {
                                if (build)
                                    build->K2_DestroyActor();
                            }
                            PlayerBuilds.clear();
                        }

                        if (ZeroGUI::Button(L"Destroy All Pickups", FVector2D {100,25}))
                        {
                            TArray<AActor*> Actors;
                            GetGameplayStatics()->STATIC_GetAllActorsOfClass(GetWorld(), AFortPickup::StaticClass(), &Actors);
                            for (int i = 0; i < Actors.Num(); i++)
                            {
                                Actors[i]->K2_DestroyActor();
                            }
                            Actors.FreeArray();
                        }

                        if (bFloorLoot)
                        {
                            if (ZeroGUI::Button(L"Spawn Floor Loot (UNSTABLE)", FVector2D { 135, 25 }))
                            {
                                bFloorLoot = false;
                                static auto FloorLoot = UObject::FindClass("BlueprintGeneratedClass Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");
                                TArray<AActor*> Actors;
                                GetGameplayStatics()->STATIC_GetAllActorsOfClass(GetWorld(), FloorLoot, &Actors);
                                for (int i = 0; i < Actors.Num(); i++)
                                {
                                    auto Loc = Actors[i]->K2_GetActorLocation();
                                    Actors[i]->K2_DestroyActor();
                                    auto Loot = Looting::GetWeapon();
                                    SummonPickup(nullptr, Loot, 1, Loc);
                                    SummonPickup(nullptr, Loot->GetAmmoWorldItemDefinition_BP(), ((UFortAmmoItemDefinition*)Loot->GetAmmoWorldItemDefinition_BP())->DropCount, Loc);
                                }
                                Actors.FreeArray();
                            }
                        }

                        if (ZeroGUI::Button(L"Stop Server", FVector2D {100,25}))
                        {
                            bRestart = true;
                        }

                        break;
                    }
                    case 1:
                    {
                        std::wstring ConnectedPlayers = std::format(L"Connected Players: {}\n", HostBeacon->NetDriver->ClientConnections.Num());

                        ZeroGUI::Text(ConnectedPlayers.c_str());

                        for (auto i = 0; i < HostBeacon->NetDriver->ClientConnections.Num(); i++)
                        {
                            auto PlayerState = HostBeacon->NetDriver->ClientConnections[i]->PlayerController->PlayerState;
                            if (PlayerState)
                            {
                                if (ZeroGUI::Button(PlayerState->GetPlayerName().c_str(), { 100, 25 }))
                                {
                                    //smth = true;
                                    //currentPlayer = PlayerState;
                                    PlayerIndex = i;
                                }
                            }
                        }

                        if (bHideAndSeek && !bStartedBus)
                        {
                            if (ZeroGUI::Button(L"Choose Random Seeker", { 100, 25 }))
                            {
                                Seeker = (AFortPlayerStateAthena*)HostBeacon->NetDriver->ClientConnections[rand() % HostBeacon->NetDriver->ClientConnections.Num()]->PlayerController->PlayerState;
                            }
                        }

                        break;
                    }
                    }
                }
            }
            else
            {
                if (!bReadyToStart)
                {
                    if (ZeroGUI::Button(L"Start Match", { 100, 25 }))
                    {
                        bReadyToStart = true;
                        ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->ReadyToStartMatch();
                        PlayerGravity = Utils::Ini().GetFloat("PlayerPawn", "GravityScale", 1);
                        if (loadoutToUse == WeaponLoadout::INI)
                        {
                            IniSlot2 = FindWID(Utils::Ini().Get("Inventory", "Slot2", "nullptr"), false);
                            IniSlot3 = FindWID(Utils::Ini().Get("Inventory", "Slot3", "nullptr"), false);
                            IniSlot4 = FindWID(Utils::Ini().Get("Inventory", "Slot4", "nullptr"), false);
                            IniSlot5 = FindWID(Utils::Ini().Get("Inventory", "Slot5", "nullptr"), false);
                            IniSlot6 = FindWID(Utils::Ini().Get("Inventory", "Slot6", "nullptr"), false);
                        }
                        bBusOnLocations = bHideAndSeek;
                        bSafeZoneBased = bHideAndSeek;
                    }
                    ZeroGUI::Text(L"Game", false, true);
                    ZeroGUI::Checkbox(L"Playground GameMode? (Most Stable)", &bPlayground);
                    ZeroGUI::Checkbox(L"Hide & Seek GameMode?", &bHideAndSeek);
                    if (bPlayground)
                        bHideAndSeek = false;
                    if (bHideAndSeek)
                        bPlayground = false;
                    ZeroGUI::Text(L"Player Loadout", false, true);
                    ZeroGUI::Checkbox(L"Infinite Ammo?", &bInfiniteAmmo);
                    ZeroGUI::Checkbox(L"Random Loadout", &bLoadoutRandom);
                    ZeroGUI::Checkbox(L"Loadout From Ini", &bLoadoutIni);
                    ZeroGUI::Checkbox(L"Rockets Loadout", &bLoadoutExplosives);
                    ZeroGUI::Checkbox(L"Snipers Loadout", &bLoadoutSnipers);
                    ZeroGUI::Text(L"World", false, true);
                    ZeroGUI::Checkbox(L"Enable Looting?", &bLooting);

                    if (!bLoadoutRandom && !bLoadoutIni && !bLoadoutExplosives && !bLoadoutSnipers)
                    {
                        loadoutToUse = WeaponLoadout::NONE;
                    }
                    if (bLoadoutRandom)
                    {
                        loadoutToUse = WeaponLoadout::RANDOM;
                        bLoadoutIni = false;
                        bLoadoutExplosives = false;
                        bLoadoutSnipers = false;
                    }
                    if (bLoadoutIni)
                    {
                        loadoutToUse = WeaponLoadout::INI;
                        bLoadoutRandom = false;
                        bLoadoutExplosives = false;
                        bLoadoutSnipers = false;
                    }
                    if (bLoadoutExplosives)
                    {
                        loadoutToUse = WeaponLoadout::EXPLOSIVES;
                        bLoadoutRandom = false;
                        bLoadoutIni = false;
                        bLoadoutSnipers = false;
                    }
                    if (bLoadoutSnipers)
                    {
                        loadoutToUse = WeaponLoadout::SNIPERS;
                        bLoadoutRandom = false;
                        bLoadoutIni = false;
                        bLoadoutExplosives = false;
                    }
                }
                else
                {
                    if (!bMapFullyLoaded)
                    {
                        ZeroGUI::Text(L"People cannot join yet.", true, true);
                    }
                }
            }
        }

        ZeroGUI::Render();
    }
}