#pragma once

#include "ZeroGUI.h"
#include <format>
#include <mutex>
#include "hidenseek.h"
#include "playground.h"

bool bStartedBus = false;
bool bRestart = false;

// GUI VARIABLES
// game
bool bBusOnLocations = true;
bool bBuildingAllowed = true;
bool bRespawn = false;
bool bHideAndSeek = false;
bool bPlayground = false;
bool bReadyToStart = false;
// pregame
bool bCosmetics = true;
bool bLoadoutRegular = true;
bool bLoadoutExplosives = false;
bool bLoadoutSnipers = false;
bool bLoadoutRandom = false;
bool bAllowStorm = true;

bool bSafeZoneBased = false;


FVector BusLocation;
FVector CenterMagic;
FVector CenterMagic2;
FVector CenterMagic3;
FVector CenterMagic4;

UObject* Object;



AAthena_GameState_C* GameState;
AFortPlayerStateAthena* Seeker;
static UFortPlaylistAthena* SoloPlaylist;




enum class WeaponLoadout
{
    REGULAR,
    EXPLOSIVES,
    SNIPERS,
    RANDOM
};

WeaponLoadout loadoutToUse = WeaponLoadout::REGULAR;

namespace GUI
{
    /* auto getRandomLocation()
    {
        FVector LocationPool[25] = 
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
            { 25000, -127121, 17525 }, // Snobby Shores
            { 5000, -60121, 10748 }, // shifty shafts
            { 110088, -115332, 17525 }, // Haunted Hills
            { 119126, -86354, 17525 }, // Junk Houses
            { 130036, -105092, 17525 }, // Junk Junction
            { 39781, 61621, 17525 }, // Moisty Mire
            { -68000, -63521, 17525 }, // Flush Factory
            { 3502, -9183, 10500 }, // Salty Springs
            { 7760, 76702, 10525 }, //race track
            { 38374, -94726, 10525 }, //Soccer field
            { 70000, -40121, 17525 }, // Loot Lake
            { -26479, 41847, 5700 }, //Prison
            { 56771, 32818, 6525 }, //Containers/crates
            { -75353, -8694, 4354 },
            { -123778, -112480, 17525 } //Spawn Island
        };

        return LocationPool[rand() % 26];
    }*/
    static FVector getRandomLocation()
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
            { 56131, -106880, 25000}, //Snobby Mountain (Before Villain Lair)
            { 56131, -106880, 25000 }, //Snobby Mountain (Before Villain Lair) DUPE
            { 29197, -109347, 25000 }, //Snobby Mountain 2
            { 29197, -109347, 25000 }, //Snobby Mountain 2 DUPE
            { -29734, -60767, 25000 }, //chair
            { -29734, -60767, 25000 }, //chair DUPE
            { -19903, -26194, 25000}, //Grandma's house
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
            { -123778, -112480, 20000 } //Spawn Island 
        };

        auto Location = Locations[rand() % Locations.size()];
        return Location;
        
    }
    static FVector getRandomPlaygroundLocation() //locations used for playground storm
    {
        static std::vector<FVector> Locations = {

            { 24426, 37710, 17525 }, // retail row
            { 50018, 73844, 17525 }, // lonely lodge
            { 39781, 61621, 17525 }, // Moisty Mire
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { 39781, 61621, 17525 }, // Moisty Mire DUPLICATE
            { -26479, 41847, 5700 }, // Prison
            { 56771, 32818, 6525 }, // Containers/crates
            { -75353, -8694, 4354 }, //Lucky Landing
            { 34278, 867, 9500 }, // dusty depot / factories
            { 79710, 15677, 17525 }, // tomato town
            { 103901, -20203, 17525 }, // ANARCHY acres
            { 86766, -83071, 17525 }, // pleasant park
            { 2399, -96255, 17525 }, // greasy grove
            { -35037, -463, 13242 }, // fatal fields
            { 83375, 50856, 17525 }, // Wailing Woods
            { 35000, -60121, 20525 }, // Tilted Towers
            { 25000, -127121, 17525 }, // Snobby Shores
            { 5000, -60121, 10748 }, // shifty shafts
            { 110088, -115332, 17525 }, // Haunted Hills
            { 119126, -86354, 17525 }, // Junk Houses
            { 130036, -105092, 17525 }, // Junk Junction
            { -68000, -63521, 17525 }, // Flush Factory
            { 3502, -9183, 10500 }, // Salty Springs
            { 7760, 76702, 17525 }, //race track
            { 38374, -94726, 17525 }, //Soccer field
            { 70000, -40121, 17525 }, // Loot Lake
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
            { -6702, 33251, 25000 } //Random Location 3/bridge DUPE
            //{ -123778, -112480, 17525 } //Spawn Island
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

        if (ZeroGUI::Window(L"Raider || Modified by ozne#4492 and jeppy#3076", &pos, FVector2D { 500.0f, 700.0f }, menu_opened))
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
                            KickController((APlayerController*)GameState->PlayerArray[PlayerIndex]->Owner, L"You have been kicked by the server. Stop hacking. If you weren't hacking, then don't be a dingus, you dingus.");

                            mtx.lock();
                            PlayerIndex = -1;
                            mtx.unlock();
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
                                if (GameState->PlayerArray.Num() <= 0)
                                    return;

                                if (!bBusOnLocations)
                                    bSafeZoneBased = false;

                                for (auto build : PlayerBuilds) //this is here to break all builds on spawn island regardless of gamemode
                                {
                                    if (build)
                                        build->K2_DestroyActor();
                                }
                                PlayerBuilds.clear();

                                if (static_cast<AAthena_GameState_C*>(GetWorld()->GameState)->GamePhase >= EAthenaGamePhase::Aircraft)
                                {
                                    printf("The bus has already started!\n");
                                    bStartedBus = true;
                                }

                                //GameState->GetBattleBusForPlayer();
                                GameState->bGameModeWillSkipAircraft = false;
                                GameState->AircraftStartTime = 0;
                                GameState->WarmupCountdownEndTime = 0;
                                auto Aircraft = GameState->GetAircraft(0);

                                static auto Kismet = GetKismetSystem();
                                Kismet->STATIC_ExecuteConsoleCommand(GetWorld(), L"startaircraft", nullptr);

                                if (bBusOnLocations)
                                {
                                    auto RandomLocation = getRandomLocation();
                                    auto PlaygroundLocation = getRandomPlaygroundLocation();

                                    BusLocation = RandomLocation;
                                    CenterMagic = PlaygroundLocation;

                                    auto Aircraft = GameState->GetAircraft(0);
                                    GameState->AircraftStartTime = 0;

                                    GameState->GetAircraft(0)->FlightInfo.TimeTillDropStart = 0;
                                    GameState->bAircraftIsLocked = false;
                                    Aircraft->ExitLocation = RandomLocation;
                                    GameState->GetAircraft(0)->FlightInfo.FlightStartLocation = FVector_NetQuantize100(RandomLocation);
                                    GameState->GetAircraft(0)->FlightInfo.FlightSpeed = 0;

                                    if (bPlayground)
                                    {
                                        Playground().InitializePlayground(SoloPlaylist, GameState);
                                        auto GameMode = reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode);
                                    }
                                }

                                //HideAndSeek().InitializeHideAndSeek();
                                printf("The bus has been started!\n");
                                bStartedBus = true;
                            }
                        }

                        if (bAllowStorm)
                        {
                            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZoneActive = true;
                            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = false;
                        }

                        if (!bAllowStorm)
                        {
                            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZoneActive = false;
                            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = true;
                        }
                        if (bAllowStorm)
                        {
                            if (ZeroGUI::Button(L"Skip SafeZone", FVector2D { 100, 25 }))
                            {
                                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"skipshrinksafezone", nullptr);
                            }
                        }
                        if (bAllowStorm)
                        {
                            if (ZeroGUI::Button(L"Reset SafeZone", FVector2D { 100, 25 }))
                            {
                                //auto Indicator = (ASafeZoneIndicator_C*)Object;
                                //Indicator->SafeZoneStartShrinkTime = 8.0f;
                                GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"startsafezone", nullptr);
                            }
                        }
                        if (ZeroGUI::Button(L"Demospeed 1", FVector2D { 100, 25 }))
                        {
                            GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"demospeed 1", nullptr);
                        }
                        if (ZeroGUI::Button(L"Demospeed 10", FVector2D { 100, 25 }))
                        {
                            GetKismetSystem()->STATIC_ExecuteConsoleCommand(GetWorld(), L"demospeed 10", nullptr);
                        }

                        ZeroGUI::Checkbox(L"Spawn bus on a random location?", &bBusOnLocations);

                        if (bBusOnLocations)
                        {
                            ZeroGUI::Checkbox(L"Base storm around random location?", &bSafeZoneBased);
                        }

                        ZeroGUI::Checkbox(L"Allow Storm?", &bAllowStorm);

                        if (!bHideAndSeek)
                        {
                            ZeroGUI::Checkbox(L"Playground GameMode?", &bPlayground);
                        }

                        ZeroGUI::Checkbox(L"Allow players to build?", &bBuildingAllowed);
                        if (ZeroGUI::Button(L"Destroy All Builds", FVector2D { 100, 25 }))
                        {
                            for (auto build : PlayerBuilds)
                            {
                                if (build)
                                    build->K2_DestroyActor();
                            }
                            PlayerBuilds.clear();
                        }
                        if (ZeroGUI::Button(L"Stop Server", FVector2D { 100, 25 }))
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
                        reinterpret_cast<AFortGameModeAthena*>(GetWorld()->AuthorityGameMode)->ReadyToStartMatch();
                    }
                    ZeroGUI::Text(L"Player Loadout", false, true);
                    ZeroGUI::Checkbox(L"Allow Cosmetics?", &bCosmetics);
                    ZeroGUI::Checkbox(L"Regular Loadout", &bLoadoutRegular);
                    ZeroGUI::Checkbox(L"Rockets Loadout", &bLoadoutExplosives);
                    ZeroGUI::Checkbox(L"Snipers Loadout", &bLoadoutSnipers);
                    ZeroGUI::Checkbox(L"Random Loadout", &bLoadoutRandom);

                    if (bLoadoutRegular)
                    {
                        loadoutToUse = WeaponLoadout::REGULAR;
                        bLoadoutExplosives = false;
                        bLoadoutSnipers = false;
                        bLoadoutRandom = false;
                    }
                    if (bLoadoutExplosives)
                    {
                        loadoutToUse = WeaponLoadout::EXPLOSIVES;
                        bLoadoutRegular = false;
                        bLoadoutSnipers = false;
                        bLoadoutRandom = false;
                    }
                    if (bLoadoutSnipers)
                    {
                        loadoutToUse = WeaponLoadout::SNIPERS;
                        bLoadoutRegular = false;
                        bLoadoutExplosives = false;
                        bLoadoutRandom = false;
                    }
                    if (bLoadoutRandom)
                    {
                        loadoutToUse = WeaponLoadout::RANDOM;
                        bLoadoutRegular = false;
                        bLoadoutExplosives = false;
                        bLoadoutSnipers = false;
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