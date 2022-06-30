#pragma once

#include "ZeroGUI.h"
#include <format>
#include <mutex>
#include "hidenseek.h"
#include "playground.h"

static bool bStartedBus = false;

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
// pregame
bool bCosmetics = true;
bool bLoadoutRegular = true;
bool bLoadoutExplosives = false;
bool bLoadoutSnipers = false;

AFortPlayerStateAthena* Seeker;
static UFortPlaylistAthena* SoloPlaylist;


enum class WeaponLoadout
{
    REGULAR,
    EXPLOSIVES,
    SNIPERS
};

WeaponLoadout loadoutToUse = WeaponLoadout::REGULAR;

namespace GUI
{
    auto getRandomLocation()
    {
        static std::vector<FVector> Locations = {

            { 24426, 37710, 17525 }, // retail row
            { 50018, 73844, 17525 }, // lonely lodge
            { 34278, 867, 9500 }, // dusty depot / factories
            { 79710, 15677, 17525 }, // tomato town
            { 103901, -20203, 17525 }, // ANARCHY acres
            { 86766, -83071, 17525 }, // pleasant park
            { 2399, -96255, 17525 }, // greasy grove
            { -35037, -463, 13242 }, // fatal fields
            { 83375, 50856, 17525 }, // Wailing Woods
            { 35000, -60121, 20525 }, // Tilted Towers
            { 40000, -127121, 17525 }, // Snobby Shores
            { 5000, -60121, 10748 }, // shifty shafts
            { 110088, -115332, 17525 }, // Haunted Hills
            { 119126, -86354, 17525 }, // Junk Houses
            { 130036, -105092, 17525 }, // Junk Junction
            { 39781, 61621, 17525 }, // Moisty Mire
            { -68000, -63521, 17525 }, // Flush Factory
            { 3502, -9183, 10500 }, // Salty Springs
            { 7760, 76702, 17525 }, //race track
            { 38374, -94726, 17525 }, //Soccer field
            { 70000, -40121, 17525 }, // Loot Lake
            { -123778, -112480, 17525 } //Spawn Island
        };

        static auto Location = Locations[rand() % Locations.size()];
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
                static int PlayerIndex = -1;

                // This is bad, but works for now.
                if (PlayerIndex != -1)
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

                        ZeroGUI::Text(std::format(L"Current Player: {}", GameState->PlayerArray[PlayerIndex]->GetPlayerName().c_str()).c_str());

                        if (ZeroGUI::Button(L"Kick", { 60.0f, 25.0f }))
                        {
                            KickController((APlayerController*)GameState->PlayerArray[PlayerIndex]->Owner, L"You have been kicked by the server.");

                            mtx.lock();
                            PlayerIndex = -1;
                            mtx.unlock();
                        }

                        if (!Seeker && bHideAndSeek)
                        {
                            if (ZeroGUI::Button(L"Make Seeker?", { 60.0f, 25.0f }))
                            {
                                auto State = static_cast<AFortPlayerStateAthena*>(GameState->PlayerArray[PlayerIndex]);
                                //State->GetCurrentPawn();

                                Seeker = State;
                            }
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
                                if (static_cast<AAthena_GameState_C*>(GetWorld()->GameState)->GamePhase >= EAthenaGamePhase::Aircraft)
                                {
                                    printf("The bus has already started!\n");
                                    bStartedBus = true;
                                }

                                GameState->bGameModeWillSkipAircraft = false;
                                GameState->AircraftStartTime = 0;
                                GameState->WarmupCountdownEndTime = 0;

                                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startaircraft", nullptr);

                                if (bBusOnLocations)
                                {
                                    auto RandLocation = getRandomLocation();
                                    std::cout << RandLocation << "\n";

                                    GameState->AircraftStartTime = 0;
                                    GameState->GetAircraft(0)->FlightStartTime = 0;
                                    GameState->GetAircraft(0)->DropStartTime = 0;
                                    GameState->GetAircraft(0)->FlightInfo.TimeTillDropStart = 0;
                                    GameState->bAircraftIsLocked = false;
                                    GameState->GetAircraft(0)->FlightInfo.FlightStartLocation = FVector_NetQuantize100(RandLocation);
                                    GameState->GetAircraft(0)->FlightInfo.FlightSpeed = 0;
                                    if (bHideAndSeek)
                                    {
                                        HideAndSeek().InitializeHideAndSeek();
                                    }
                                    if (bPlayground)
                                    {
                                        Playground().InitializePlayground(SoloPlaylist, GameState);
                                    }
                                }

                                //HideAndSeek().InitializeHideAndSeek();
                                printf("The bus has been started!\n");
                                bStartedBus = true;
                            }

                            ZeroGUI::Checkbox(L"Spawn bus on a random location?", &bBusOnLocations);

                            if (!bPlayground)
                            {
                                ZeroGUI::Checkbox(L"Hide & Seek GameMode?", &bHideAndSeek);
                            }
                            if (!bHideAndSeek)
                            {
                                ZeroGUI::Checkbox(L"Playground GameMode?", &bPlayground);
                            }
                        }


                        ZeroGUI::Checkbox(L"Allow players to build?", &bBuildingAllowed);
                        
                        ZeroGUI::Checkbox(L"Safe Zone Paused?", &((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused);

                        if (ZeroGUI::Button(L"Stop Server", FVector2D {100,25}))
                        {
                            bRestart = true;
                        }

                        break;
                    }
                    case 1:
                    {
                        std::wstring ConnectedPlayers = std::format(L"Connected Players: {}\n", GameState->PlayerArray.Num());

                        ZeroGUI::Text(ConnectedPlayers.c_str());

                        for (auto i = 0; i < GameState->PlayerArray.Num(); i++)
                        {
                            auto PlayerState = GameState->PlayerArray[i];
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
                                Seeker = (AFortPlayerStateAthena*)GameState->PlayerArray[rand() % GameState->PlayerArray.Num()];
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
                    }
                    ZeroGUI::Text(L"Player Loadout", false, true);
                    ZeroGUI::Checkbox(L"Allow Cosmetics?", &bCosmetics);
                    ZeroGUI::Checkbox(L"Regular Loadout", &bLoadoutRegular);
                    ZeroGUI::Checkbox(L"Rockets Loadout", &bLoadoutExplosives);
                    ZeroGUI::Checkbox(L"Snipers Loadout", &bLoadoutSnipers);

                    if (bLoadoutRegular)
                    {
                        loadoutToUse = WeaponLoadout::REGULAR;
                        bLoadoutExplosives = false;
                        bLoadoutSnipers = false;
                    }
                    if (bLoadoutExplosives)
                    {
                        loadoutToUse = WeaponLoadout::EXPLOSIVES;
                        bLoadoutRegular = false;
                        bLoadoutSnipers = false;
                    }
                    if (bLoadoutSnipers)
                    {
                        loadoutToUse = WeaponLoadout::SNIPERS;
                        bLoadoutRegular = false;
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