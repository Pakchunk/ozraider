#pragma once

#include "gui.h"
#include "ufunctionhooks.h"
#include "inireader.h"

// #define LOGGING

namespace Hooks
{
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

    void Beacon_NotifyControlMessage(AOnlineBeaconHost* Beacon, UNetConnection* Connection, uint8 MessageType, int64* Bunch)
    {
        if (bStartedBus && !bPlayground)
        {
            KickController(Connection->PlayerController, L"Game has already started.");
            LOG_INFO("[LogRaider] {} tried to join, but cannot because the game has already started.", Utils::DelimiterParse(Utils::DelimiterParse(Connection->RequestURL.ToString(), "?")[1], "=")[1]);
            return;
        }

        //printf("Recieved control message %i\n", MessageType);

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

            LOG_INFO("[LogRaider] Recieved connection from user: '{}', player is now joining.", Utils::DelimiterParse(Utils::DelimiterParse(Connection->RequestURL.ToString(), "?")[1], "=")[1]);

            Native::World::WelcomePlayer(GetWorld(), Connection);

            return;
        }
        case 15: // NMT_PCSwap
            // return;
            break;
        }

        Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch);
    }

    bool LocalPlayerSpawnPlayActor(ULocalPlayer* Player, const FString& URL, FString& OutError, UWorld* World) { return bTraveled ? true : Native::LocalPlayer::SpawnPlayActor(Player, URL, OutError, World); }
    void WelcomePlayer(UWorld* World, UNetConnection* IncomingConnection) { Native::World::WelcomePlayer(GetWorld(), IncomingConnection); }
    uint64 GetNetMode(UWorld* World) { return 2; }
    char KickPlayer(__int64 a1, __int64 a2, __int64 a3) { return 0; }
    void World_NotifyControlMessage(UWorld* World, UNetConnection* Connection, uint8 MessageType, void* Bunch){ Native::World::NotifyControlMessage(GetWorld(), Connection, MessageType, Bunch); }
    uint8 Beacon_NotifyAcceptingConnection(AOnlineBeacon* Beacon) { return Native::World::NotifyAcceptingConnection(GetWorld()); }
    void* SeamlessTravelHandlerForWorld(UEngine* Engine, UWorld* World) { return Native::Engine::SeamlessTravelHandlerForWorld(Engine, GetWorld()); }
    void* NetDebug(UObject* _this) { return nullptr; }
    void PostRender(UGameViewportClient* _this, UCanvas* Canvas) { ZeroGUI::SetupCanvas(Canvas); GUI::Tick(); Native::GameViewportClient::PostRender(_this, Canvas); }
    __int64 CollectGarbage(__int64 a1) { return 0; };

    void* OnReloadHook(AFortWeapon* a1, int a2)// cr: averymadness
    {
        if (!bInfiniteAmmo)
        {
            auto Pawn = (AFortPlayerPawnAthena*)a1->GetOwner();
            auto PlayerController = (AFortPlayerControllerAthena*)Pawn->Controller;

            bool Successful = true;

            if (a1 && Successful)
            {
                auto AmmoDef = a1->WeaponData->GetAmmoWorldItemDefinition_BP();

                if (!AmmoDef || a1->WeaponData->GetName().find("TID") != -1)
                    AmmoDef = a1->WeaponData;

                auto Inventory = PlayerController->WorldInventory;

                auto ReplicatedEntries = Inventory->Inventory.ReplicatedEntries;
                auto ItemInstances = Inventory->Inventory.ItemInstances;

                for (int i = 0; i < Inventory->Inventory.ReplicatedEntries.Num(); i++)
                {
                    if (Inventory->Inventory.ReplicatedEntries[i].ItemDefinition == AmmoDef)
                    {
                        Inventory->Inventory.ReplicatedEntries[i].Count -= a2;
                        Inventory->Inventory.ReplicatedEntries[i].ReplicationKey++;

                        if (Inventory->Inventory.ReplicatedEntries[i].Count <= 0)
                        {
                            Inventory->Inventory.ReplicatedEntries.RemoveSingle(i);

                            for (int j = 0; j < ItemInstances.Num(); j++)
                            {
                                auto ItemInstance = ItemInstances[j];

                                if (ItemInstance && ItemInstance->GetItemDefinitionBP() == AmmoDef)
                                {
                                    ItemInstances.RemoveSingle(i);
                                }
                            }
                        }

                        Inventory::Update(PlayerController, 0, true);
                    }
                }
            }
        }

        Native::Fort::OnReload(a1, a2);
    }

    // eliminates the chance of two players being on the same team randomly.
    auto ApplyTeam()
    {
        if (!hasSetup)
        {
            hasSetup = true;
            for (int i = 2; i < 104; i++)
            {
                teamsmap.insert_or_assign((EFortTeam)i, false);
            }
        }

        for (auto team : teamsmap)
        {
            if (team.second)
                continue;

            teamsmap.insert_or_assign(team.first, true);

            return team.first;
        }
    }

    APlayerController* SpawnPlayActor(UWorld* World, UPlayer* NewPlayer, ENetRole RemoteRole, FURL& URL, void* UniqueId, SDK::FString& Error, uint8 NetPlayerIndex)
    {
        auto PlayerController = (AFortPlayerControllerAthena*)Native::World::SpawnPlayActor(GetWorld(), NewPlayer, RemoteRole, URL, UniqueId, Error, NetPlayerIndex);
        NewPlayer->PlayerController = PlayerController;

        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;

        auto Pawn = (APlayerPawn_Athena_C*)SpawnActorTrans(APlayerPawn_Athena_C::StaticClass(), GetPlayerStart(PlayerController), PlayerController);
        
        PlayerController->Pawn = Pawn;
        PlayerController->AcknowledgedPawn = Pawn;
        Pawn->Owner = PlayerController;
        Pawn->OnRep_Owner();
        PlayerController->OnRep_Pawn();
        PlayerController->Possess(Pawn);

        Pawn->SetMaxHealth(HEALTH);
        Pawn->SetMaxShield(SHIELD);
        Pawn->ShieldRegenGameplayEffect = nullptr;
        Pawn->ShieldRegenDelayGameplayEffect = nullptr;
        Pawn->HealthRegenGameplayEffect = nullptr;
        Pawn->HealthRegenDelayGameplayEffect = nullptr;

        PlayerController->bHasClientFinishedLoading = true;
        PlayerController->bHasServerFinishedLoading = true;
        PlayerController->bHasInitiallySpawned = true;
        PlayerController->OnRep_bHasServerFinishedLoading();

        PlayerState->bHasFinishedLoading = true;
        PlayerState->bHasStartedPlaying = true;
        PlayerState->OnRep_bHasStartedPlaying();

        Pawn->MovementSet->GravityZScale.CurrentValue = PlayerGravity;

        static auto FortRegisteredPlayerInfo = ((UFortGameInstance*)GetWorld()->OwningGameInstance)->RegisteredPlayers[0];
        if (FortRegisteredPlayerInfo)
        {
            auto Hero = FortRegisteredPlayerInfo->AthenaMenuHeroDef;

            if (Hero)
            {
                UFortHeroType* HeroType = Hero->GetHeroTypeBP();
                PlayerState->HeroType = HeroType;
                PlayerState->OnRep_HeroType();

                static auto Head = Utils::FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
                static auto Body = Utils::FindObjectFast<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");

                PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Head] = Head;
                PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Body] = Body;
                PlayerState->OnRep_CharacterParts();
            }
        }

        static UFortWeaponRangedItemDefinition* Pickaxe = FindWID("WID_Harvest_Pickaxe_Athena_C_T01");

        InitInventory(PlayerController, loadoutToUse != WeaponLoadout::NONE);
        switch (loadoutToUse)
        {
            case WeaponLoadout::NONE:
            {
                EquipLoadout(PlayerController, { Pickaxe });
                break;
            }
            case WeaponLoadout::RANDOM:
            {
                Looting::ShuffleItems();
                PlayerLoadout FortLoadout = 
                {
                    Pickaxe,
                    Looting::ShotgunPool[GetMath()->STATIC_RandomInteger(Looting::ShotgunPool.size())],
                    Looting::SMGPool[GetMath()->STATIC_RandomInteger(Looting::SMGPool.size())],
                    Looting::AssaultPool[GetMath()->STATIC_RandomInteger(Looting::AssaultPool.size())],
                    Looting::GetRandomConsumable(),
                    Looting::GetRandomConsumable()
                };

                EquipLoadout(PlayerController, FortLoadout);
                break;
            }
            case WeaponLoadout::INI:
            {
                PlayerLoadout FortLoadout = 
                {
                    Pickaxe,
                    IniSlot2,
                    IniSlot3,
                    IniSlot4,
                    IniSlot5,
                    IniSlot6
                };

                EquipLoadout(PlayerController, FortLoadout);
                break;
            }
            case WeaponLoadout::EXPLOSIVES:
            {
                static auto Rocket1 = FindWID("WID_Launcher_Rocket_Athena_R_Ore_T03");
                static auto Rocket2 = FindWID("WID_Launcher_Rocket_Athena_R_Ore_T03");
                static auto GrenadeLauncher = FindWID("WID_Launcher_Grenade_Athena_SR_Ore_T03");
                static auto C4 = FindWID("Athena_C4", false);
                static auto Consumable = FindWID("Athena_PurpleStuff", false);
                PlayerLoadout FortLoadout = 
                {
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
                static auto Consumable = FindWID("Athena_PurpleStuff", false);
                PlayerLoadout FortLoadout = 
                {
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
        }
        PlayerController->OverriddenBackpackSize = 100;

        auto CheatManager = CreateCheatManager(PlayerController);
        if (bInfiniteAmmo)
        {
            CheatManager->ToggleInfiniteAmmo();
            CheatManager->ToggleInfiniteDurability();
        }

        if (!bStartedBus)
            Pawn->HealthSet->Health.Minimum = 100;

        if (PlayerController->Pawn)
        {
            if (PlayerController->Pawn->PlayerState)
            {
                PlayerState->TeamIndex = ApplyTeam();
                PlayerState->OnRep_PlayerTeam();
            }
        }

        if (Pawn && Pawn->AbilitySystemComponent)
            ApplyAbilities(Pawn);

        auto Drone = (ABP_VictoryDrone_C*)SpawnActor(ABP_VictoryDrone_C::StaticClass(), Pawn->K2_GetActorLocation());
        Drone->Owner = nullptr;
        Drone->OnRep_Owner();
        Drone->TriggerPlayerSpawnEffects();

        return PlayerController;
    }

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
        DetourAttachE(Native::Fort::OnReload, OnReloadHook);
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
        DetourDetachE(Native::Fort::OnReload, OnReloadHook);
        DETOUR_END
    }

    void ProcessEventHook(UObject* Object, UFunction* Function, void* Parameters)
    {
        auto FullFuncName = Function->GetFullName();
        auto FuncName = Function->GetName();

        if (!bPlayButton)
        {
            static auto PlayButtonFn = UObject::FindObject<UFunction>("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature");

            // if (FunctionName.find("BP_PlayButton") != -1)
            if (Function == PlayButtonFn)
            {
                bPlayButton = true;
                Game::Start();
                LOG_INFO("[Game::Start] Done");

                InitNetworkHooks();
                LOG_INFO("[InitNetworkHooks] Done");
                TimesInGame++;
            }
        }

        if (FuncName.find("Tick") != -1 && bPlayground && PlayerBuilds.size() >= 600)
        {
            for (auto build : PlayerBuilds)
            {
                if (build != nullptr)
                    build->K2_DestroyActor();
            }
            PlayerBuilds.clear();
        }

        if (FullFuncName.find("Tick") != -1 && bStartedBus && HostBeacon->NetDriver->ClientConnections.Num() <= 0)
        {
            bRestart = true;
        }

        // mfw im too lazy to include a file
        if (FuncName.find("Tick") != -1 && bRestart)
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
            bInfiniteAmmo = true;
            bLooting = false;
            bLoadoutRandom = false;
            bLoadoutIni = false;
            bLoadoutExplosives = false;
            bLoadoutSnipers = false;
            bBusOnLocations = false;
            bBuildingAllowed = true;
            bRespawn = false;
            bFloorLoot = false;
            bHideAndSeek = false;
            bPlayground = false;
            bStormPaused = false;
            bSafeZoneBased = false;
            PlayersJumpedFromBus = 0;
            ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->bSafeZonePaused = false;
            PlayerBuilds.clear();
            for (auto team : teamsmap)
            {
                if (team.second)
                    teamsmap.insert_or_assign(team.first, false);
                else
                    break;
            }
            DetachNetworkHooks();
            GetGameplayStatics()->STATIC_OpenLevel(GetWorld(), GetKismetString()->STATIC_Conv_StringToName(L"Frontend"), true, L"");
        }

        if (FullFuncName == "Function FortniteGame.FortPlayerController.ClientReportDamagedResourceBuilding")
        {
            auto Controller = (AFortPlayerControllerAthena*)Object;
            auto Params = (AFortPlayerController_ClientReportDamagedResourceBuilding_Params*)Parameters;
            auto ResourceType = Params->PotentialResourceType.GetValue();
            UFortResourceItemDefinition* WorldItemDefinition = nullptr;
            static auto WoodDefinition = Utils::FindObjectFast<UFortResourceItemDefinition>(ResourcePoolNames[0]);
            static auto StoneDefinition = Utils::FindObjectFast<UFortResourceItemDefinition>(ResourcePoolNames[1]);
            static auto MetalDefinition = Utils::FindObjectFast<UFortResourceItemDefinition>(ResourcePoolNames[2]);

            switch (ResourceType)
            {
                case EFortResourceType::Wood:
                    WorldItemDefinition = WoodDefinition;
                    break;
                case EFortResourceType::Stone:
                    WorldItemDefinition = StoneDefinition;
                    break;
                case EFortResourceType::Metal:
                    WorldItemDefinition = MetalDefinition;
                    break;
            }

            bool bFound = false;

            if (WorldItemDefinition)
            {
                for (int i = 0; i < Controller->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
                {
                    if (Controller->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == WorldItemDefinition)
                    {
                        bFound = true;
                        Controller->WorldInventory->Inventory.ReplicatedEntries[i].Count += Params->PotentialResourceCount;
                        Controller->WorldInventory->Inventory.ReplicatedEntries[i].ReplicationKey++;

                        if (Controller->WorldInventory->Inventory.ReplicatedEntries[i].Count <= 0)
                        {
                            Controller->WorldInventory->Inventory.ReplicatedEntries.RemoveSingle(i);

                            for (int j = 0; j < Controller->WorldInventory->Inventory.ItemInstances.Num(); j++)
                            {
                                auto ItemInstance = Controller->WorldInventory->Inventory.ItemInstances[j];

                                if (ItemInstance && ItemInstance->GetItemDefinitionBP() == WorldItemDefinition)
                                {
                                    Controller->WorldInventory->Inventory.ItemInstances.RemoveSingle(i);
                                    break;
                                }
                                else
                                    continue;
                            }
                        }
                        else if (Controller->WorldInventory->Inventory.ReplicatedEntries[i].Count > 999)
                        {
                            
                            SummonPickup((AFortPlayerPawn*)Controller->Pawn, WorldItemDefinition, Controller->WorldInventory->Inventory.ReplicatedEntries[i].Count - 999, Controller->Pawn->K2_GetActorLocation());
                            Controller->WorldInventory->Inventory.ReplicatedEntries[i].Count = 999;
                            Inventory::Update(Controller, 0, true);
                        }


                        Inventory::Update(Controller, 0, true);
                        break;
                    }
                    else
                        continue;
                }

                if (!bFound)
                {
                    for (int i = 0; i < Controller->QuickBars->SecondaryQuickBar.Slots.Num(); i++)
                    {
                        if (!Controller->QuickBars->SecondaryQuickBar.Slots[i].Items.Data) // Checks if the slot is empty
                        {
                            Inventory::AddItemToSlot(Controller, WorldItemDefinition, i, EFortQuickBars::Secondary, Params->PotentialResourceCount);
                            break;
                        }
                    }
                }
            }
        }

        // ToDo: Proper material gather amount and weak spot multiplier (probably impossible? as this is improper^^)
        // real todo: find out the proper way of doing material farming
        if (FuncName == "OnDamagePlayEffects" || FuncName == "OnDeathPlayEffects")
        {
            if (!Object->IsA(AFortPlayerPawnAthena::StaticClass()))
            {
                auto Params = (ABuildingActor_OnDamagePlayEffects_Params*)Parameters;
                if (Params->InstigatedBy)
                {
                    auto Controller = (AFortPlayerControllerAthena*)Params->InstigatedBy->Controller;
                    if (Controller && Params->DamageCauser->GetFullName().find("Melee") != -1)
                    {
                        auto Obj = (ABuildingSMActor*)Object;
                        if (Obj->bPlayerPlaced != true)
                            Controller->ClientReportDamagedResourceBuilding(Obj, Obj->ResourceType, GetMath()->STATIC_RandomIntegerInRange(3, 8), FuncName == "OnDeathPlayEffects", false);
                    }
                }
            }
        }

        if (FullFuncName == "Function BP_VictoryDrone.BP_VictoryDrone_C.OnSpawnOutAnimEnded")
        {
            if (!bPlayground)
            {
                auto drone = (ABP_VictoryDrone_C*)Object;

                if (drone->Owner != nullptr && drone->PlayerState != nullptr)
                {
                    if (((AFortPlayerControllerAthena*)drone->Owner)->Pawn)
                        ((AFortPlayerControllerAthena*)drone->Owner)->Pawn->K2_DestroyActor();

                    if (((AFortPlayerControllerAthena*)drone->Owner)->NetConnection && ((AFortPlayerControllerAthena*)drone->PlayerState->Owner)->Pawn)
                        Spectate(((AFortPlayerControllerAthena*)drone->Owner)->NetConnection, (AFortPlayerStateAthena*)drone->PlayerState);
                }
                
                drone->K2_DestroyActor();
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

        // todo: proper looting
        if (FullFuncName == "Function FortniteGame.FortPlayerController.ServerAttemptInteract" && bLooting && bStartedBus && !bHideAndSeek)
        {
            auto Params = (AFortPlayerController_ServerAttemptInteract_Params*)Parameters;
            if (Params->ReceivingActor->IsA(ABuildingContainer::StaticClass()) || Params->ReceivingActor->IsA(AFortAthenaSupplyDrop::StaticClass()))
            {
                auto Container = (ABuildingContainer*)Params->ReceivingActor;
                if (Container->bAlreadySearched != true)
                {
                    Container->bAlreadySearched = true;
                    Container->bStartAlreadySearched_Athena = true;
                    Container->OnRep_bAlreadySearched();
                    if (Params->ReceivingActor->GetFullName().find("Tiered_Chest") != -1)
                    {
                        auto LootArray = Looting::GetChestLoot();
                        for (int i = 0; i < LootArray.Num(); i++)
                        {
                            int Count = 1;
                            if (LootArray[i]->IsA(UFortAmmoItemDefinition::StaticClass()))
                                Count = ((UFortAmmoItemDefinition*)LootArray[i])->DropCount;
                            if (LootArray[i]->IsA(UFortResourceItemDefinition::StaticClass()))
                                Count = 30;

                            auto Pickup = SummonPickup(nullptr, LootArray[i], Count, Container->K2_GetActorLocation());
                            //LOG_INFO("{}", ((UFortWeaponItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition)->GetWeaponActorClass()->GetName());
                            if (Pickup->PrimaryPickupItemEntry.ItemDefinition->IsA(UFortWeaponItemDefinition::StaticClass()))
                            {
                                auto WeaponWid = (UFortWeaponItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition;
                                auto ActorClass = (AFortWeapon*)WeaponWid->GetWeaponActorClass();

                                Pickup->PrimaryPickupItemEntry.LoadedAmmo = ActorClass->GetBulletsPerClip();
                            }
                        }
                        LootArray.FreeArray();
                    }
                    if (Params->ReceivingActor->GetFullName().find("Tiered_Short_Ammo") != -1)
                    {
                        auto AmmoArray = Looting::GetAmmoBoxLoot();
                        for (int i = 0; i < AmmoArray.Num(); i++)
                            SummonPickup(nullptr, AmmoArray[i], AmmoArray[i]->DropCount, Container->K2_GetActorLocation());
                        AmmoArray.FreeArray();
                    }
                    if (Params->ReceivingActor->GetFullName().find("AthenaSupplyDrop") != -1)
                    {
                        auto LootArray = Looting::GetSupplyDropLoot();
                        for (int i = 0; i < LootArray.Num(); i++)
                        {
                            int Count = 1;
                            if (LootArray[i]->IsA(UFortAmmoItemDefinition::StaticClass()))
                                Count = ((UFortAmmoItemDefinition*)LootArray[i])->DropCount;
                            if (LootArray[i]->IsA(UFortResourceItemDefinition::StaticClass()))
                                Count = 100;

                            SummonPickup(nullptr, LootArray[i], Count, Params->ReceivingActor->K2_GetActorLocation());
                        }
                        LootArray.FreeArray();
                    }
                }
            }

            if (Params->ReceivingActor->GetFullName().find("Prop_QuestInteractable_GniceGnome") != -1)
            {
                //KickController((APlayerController*)Object, L"dont fuck with the gnome");
            }
        }

        if (FullFuncName == "Function FortniteGame.FortPlayerControllerZone.ClientOnPawnDied" && bLooting && bStartedBus && !bPlayground && !bHideAndSeek)
        {
            auto Params = static_cast<AFortPlayerControllerZone_ClientOnPawnDied_Params*>(Parameters);
            auto DeadPC = (AFortPlayerControllerAthena*)Object;
            if (DeadPC->NetConnection)
            {
                for (int i = 0; i < DeadPC->WorldInventory->Inventory.ItemInstances.Num(); i++)
                {
                    auto Definition = DeadPC->WorldInventory->Inventory.ItemInstances[i]->GetItemDefinitionBP();
                    if (Definition->Class->GetName().find("FortEditToolItemDefinition") != -1 || Definition->Class->GetName().find("FortWeaponMeleeItemDefinition") != -1 || Definition->Class->GetName().find("FortBuildingItemDefinition") != -1)
                        continue;

                    auto Pickup = SummonPickup((AFortPlayerPawn*)DeadPC->Pawn, Definition, 1, DeadPC->Pawn->K2_GetActorLocation());
                    Pickup->PrimaryPickupItemEntry.LoadedAmmo = DeadPC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.LoadedAmmo;
                    Pickup->OnRep_PrimaryPickupItemEntry();
                }

                if (DeadPC->QuickBars)
                    DeadPC->QuickBars->K2_DestroyActor();
            }
        }

        if (FullFuncName == "Function SafeZoneIndicator.SafeZoneIndicator_C.OnSafeZoneStateChange" && bSafeZoneBased)
        {
            auto Indicator = (ASafeZoneIndicator_C*)Object;
            auto SafeZonePhase = ((AFortGameModeAthena*)GetWorld()->AuthorityGameMode)->SafeZonePhase;
            Indicator->NextCenter = (FVector_NetQuantize100)BusLocation;
            switch (SafeZonePhase)
            {
            case 0:
                Indicator->Radius = 15000;
                Indicator->NextRadius = 450;
                break;
            case 1:
                Indicator->NextRadius = 450;
                break;
            default:
                Indicator->NextRadius = 50;
                break;
            }

            /*
            switch (SafeZonePhase)
            {
            case 0:
                
                Indicator->Radius = 15000;
                Indicator->NextRadius = 11000;
                break;
            case 1:
                Indicator->NextRadius = 7000;
                break;
            case 2:
                Indicator->NextRadius = 4000;
                break;
            case 3:
                Indicator->NextRadius = 1000;
                break;
            case 4:
                Indicator->NextRadius = 500;
                break;
            default:
                Indicator->NextRadius = 50;
                break;
            }
            */
        }

        if (FullFuncName == "Function FortniteGame.FortPlayerControllerZone.ClientOnPawnDied" && bStartedBus)
        {
            auto Params = static_cast<AFortPlayerControllerZone_ClientOnPawnDied_Params*>(Parameters);
            auto DeadPC = (AFortPlayerControllerAthena*)Object;

            if (DeadPC)
            {
                auto DeadPlayerState = (AFortPlayerStateAthena*)DeadPC->PlayerState;
                bool IsKiller = Params->DeathReport.KillerPlayerState ? true : false;
                auto KillerPawn = Params->DeathReport.KillerPawn;
                auto KillerPlayerState = (AFortPlayerStateAthena*)Params->DeathReport.KillerPlayerState;

                if (!bPlayground)
                {
                    GameState->PlayersLeft--;
                    GameState->TotalPlayers--;
                    RemovePlayerState(DeadPlayerState);
                    if (GameState->PlayersLeft != GameState->TotalPlayers)
                        GameState->PlayersLeft = GameState->TotalPlayers;
                    GameState->OnRep_PlayersLeft();
                }
                FVector DeadPawnLocation = DeadPC->Pawn->K2_GetActorLocation();
                auto Drone = SpawnActor(ABP_VictoryDrone_C::StaticClass(), DeadPawnLocation);
                Drone->Owner = DeadPC;
                Drone->OnRep_Owner();
                ((ABP_VictoryDrone_C*)Drone)->PlayerState = IsKiller ? Params->DeathReport.KillerPlayerState : nullptr;
                ((ABP_VictoryDrone_C*)Drone)->OnRep_PlayerState();

                FDeathInfo deathInfo;
                deathInfo.bDBNO = false;
                deathInfo.DeathLocation = DeadPawnLocation;
                deathInfo.Distance = Params->DeathReport.KillerPawn ? Params->DeathReport.KillerPawn->GetDistanceTo(DeadPC->Pawn) : 0;
                deathInfo.DeathCause = Game::GetDeathReason(Params->DeathReport);
                deathInfo.FinisherOrDowner = KillerPlayerState ? KillerPlayerState : DeadPlayerState;
                DeadPlayerState->DeathInfo = deathInfo;
                DeadPlayerState->OnRep_DeathInfo();

                if (IsKiller && KillerPlayerState != DeadPlayerState)
                {
                    KillerPlayerState->ClientReportKill(DeadPlayerState);
                    KillerPlayerState->KillScore++;
                    KillerPlayerState->TeamKillScore++;
                    KillerPlayerState->OnRep_Kills();
                }
                else
                    DeadPlayerState->ClientReportKill(DeadPlayerState);

                // sometimes checking if the killer is the dead player is important
                // if you dont and u die in storm and you were gliding it would loop this (no joke) 2,147,483,647 times, thus causing a stack overflow
                if (bPlayground && ((AFortPlayerPawnAthena*)DeadPC->Pawn)->IsSkydiving() && IsKiller && KillerPlayerState != DeadPC->PlayerState)
                    ((AFortPlayerControllerAthena*)KillerPawn->Controller)->ClientOnPawnDied(Params->DeathReport);

                if (DeadPC->Pawn)
                    DeadPC->Pawn->K2_DestroyActor();

                if (bPlayground && IsKiller)
                    Playground().OnDeath((AFortPlayerPawnAthena*)KillerPawn);

                if (GameState->PlayersLeft == 1)
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
            auto FunctionName = FuncName;
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
