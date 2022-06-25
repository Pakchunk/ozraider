#pragma once

#include "ZeroGUI.h"
#include <format>
#include <mutex>
#include "hidenseek.h"
#include "playground.h"

static bool bStartedBus = false;
bool bBusOnLocations = false;
bool bBuildingAllowed = true;
bool bRespawn = false;
AFortPlayerStateAthena* Seeker;
bool bHideAndSeek = false;
bool bPlayground = false;

AAthena_GameState_C* GameState;
static UFortPlaylistAthena* SoloPlaylist;

enum class CustomMode
{
    NONE,
    JUGGERNAUT,
    // Gives the players 500 health and makes you slower.
    LATEGAME,
    // TODO: You know what late game is.
    LIFESTEAL,
    // TODO: You know what life steal is, but this might be a stupid idea.
    SPACE,
    // Sets gravity like the moon // BUG: Unfortunately, the gravityscale variable doesn't update for the client, making them rubberband and making it look weird.
    SIPHON // Gives 50 shield/health whenever you finish someone. (Late game also has this)
};

// constexpr CustomMode Mode = CustomMode::NONE;

namespace GUI
{
    auto getRandomLocation()
    {
        FVector LocationPool[21] = 
        {
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
            //{ -123778, -112480, 17525 } //Spawn Island
        };

        return LocationPool[rand() % 21];
    }

    std::mutex mtx;
    void Tick()
    {
        ZeroGUI::Input::Handle();

        static bool menu_opened = true;

        if (GetAsyncKeyState(VK_F2) & 1)
            menu_opened = !menu_opened;

        static auto pos = FVector2D { 200.f, 250.0f };

        if (ZeroGUI::Window(L"Raider", &pos, FVector2D { 500.0f, 700.0f }, menu_opened))
        {
            if (bListening && HostBeacon)
            {
                static auto GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
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
                    //if (ZeroGUI::ButtonTab(L"Server", FVector2D { 110, 25 }, tab == 2))
                        //tab = 2;

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

                                    GameState->AircraftStartTime = 0;
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
                // ZeroGUI::Text(L"Waiting for map to load...");
            }
        }

        ZeroGUI::Render();
        // ZeroGUI::Draw_Cursor(menu_opened);
    }
}

/*
static bool bStartedBus = false;
bool bBusOnLocations = false;
bool bBuildingAllowed = true;
bool bRespawn = false;

const char* thistext = "Allow players to build?";

enum class CustomMode
{
    NONE,
    JUGGERNAUT, // Gives the players 500 health and makes you slower.
    LATEGAME, // TODO: You know what late game is.
    LIFESTEAL, // TODO: You know what life steal is, but this might be a stupid idea.
    SPACE, // Sets gravity like the moon // BUG: Unfortunately, the gravityscale variable doesn't update for the client, making them rubberband and making it look weird.
    SIPHON // Gives 50 shield/health whenever you finish someone. (Late game also has this)
};

constexpr CustomMode Mode = CustomMode::NONE;
AAthena_GameState_C* GameState;

namespace GUI
{
    void Tick()
    {
        ZeroGUI::Input::Handle();

        static bool menu_opened = true;

        if (GetAsyncKeyState(VK_F2) & 1)
            menu_opened = !menu_opened;

        auto pos = FVector2D{ 200.f, 250.0f };

        if (ZeroGUI::Window((char*)"Raider", &pos, FVector2D{ 500.0f, 400.0f }, menu_opened))
        {
            if (bListening && HostBeacon && bMapFullyLoaded)
            {
                static auto GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);
                std::string ConnectedPlayers = std::format("Connected Players: {}\n", GameState->PlayerArray.Num());

                ZeroGUI::Text((char*)ConnectedPlayers.c_str());

                if (!bStartedBus)
                {
                    if (ZeroGUI::Button((char*)"Start Bus", FVector2D{ 100, 25 }))
                    {
                        if (((AAthena_GameState_C*)GetWorld()->GameState)->GamePhase >= EAthenaGamePhase::Aircraft)
                        {
                            std::cout << "The bus is already started!\n";
                            bStartedBus = true;
                        }
			                                                              			
                        GameState->bGameModeWillSkipAircraft = false;
                        GameState->AircraftStartTime = 0;
                        GameState->WarmupCountdownEndTime = 0;

                        ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass())->STATIC_ExecuteConsoleCommand(GetWorld(), L"startaircraft", nullptr);

                        if (bBusOnLocations)
                        {
                            FVector_NetQuantize100 h[20] = {
                                { 24426, 37710, 17525 }, // retail row
                                { 50018, 73844, 17525 }, // lonely lodge
                                //{ 34278, 867, 9500 }, // dusty depot / factories
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
                                //{ 3502, -9183, 10500 }, // Salty Springs
                                { 7760, 76702, 17525 }, //race track
                                { 38374, -94726, 17525 }, //Soccer field
                                { 70000, -40121, 17525 }, // Loot Lake
                                { -123778, -112480, 17525 } //Spawn Island
                            };
                            GameState->AircraftStartTime = 0;
                            GameState->GetAircraft(0)->FlightInfo.TimeTillDropStart = 0;
                            GameState->bAircraftIsLocked = false;
                            GameState->GetAircraft(0)->FlightInfo.FlightStartLocation = h[rand() % 20];
                            GameState->GetAircraft(0)->FlightInfo.FlightSpeed = 0;
                        }
                        printf("Started Aircraft!\n");
                        bStartedBus = true;
                    }

                    ZeroGUI::Checkbox((char*)"Spawn bus on a random location?", &bBusOnLocations);
                }

                ZeroGUI::Checkbox((char*)thistext, &bBuildingAllowed);

                //ZeroGUI::Checkbox((char*)"Allow players to respawn?", &bRespawn);
            }
            else
            {
                // ZeroGUI::Text((char*)"Waiting for map to load...");
            }
        }

        ZeroGUI::Render();
        //ZeroGUI::Draw_Cursor(menu_opened);
    }
}
*/