#pragma once

#include <unordered_set>
#include <random>
#include "json.hpp"
#include "native.h"


constexpr auto PI = 3.1415926535897932f;
constexpr auto INV_PI = 0.31830988618f;
constexpr auto HALF_PI = 1.57079632679f;

inline bool bTraveled = false;
inline bool bPlayButton = false;
inline bool bListening = false;
static bool bSpawnedFloorLoot = false;
static bool bMapFullyLoaded = false;

std::map<EFortTeam, bool> teamsmap;
bool hasSetup = false;

static std::unordered_set<ABuildingSMActor*> Buildings;
std::vector<ABuildingSMActor*> PlayerBuilds;
static AFortOnlineBeaconHost* HostBeacon = nullptr;

inline UWorld* GetWorld()
{
    return GetEngine()->GameViewport->World;
    //return *(UWorld**)(Offsets::Imagebase + Offsets::GWorldOffset);
}

inline AAthena_PlayerController_C* GetPlayerController(int32 Index = 0)
{
    if (Index > GetWorld()->OwningGameInstance->LocalPlayers.Num())
    {
        std::cout << "WARNING! PlayerController out of range! (" << Index << " out of " << GetWorld()->OwningGameInstance->LocalPlayers.Num() << ")" << '\n';

        return (AAthena_PlayerController_C*)GetWorld()->OwningGameInstance->LocalPlayers[0]->PlayerController;
    }

    return (AAthena_PlayerController_C*)GetWorld()->OwningGameInstance->LocalPlayers[Index]->PlayerController;
}


struct FNetworkObjectInfo
{
    AActor* Actor;

    TWeakObjectPtr<AActor> WeakActor;

    double NextUpdateTime;

    double LastNetReplicateTime;

    float OptimalNetUpdateDelta;

    float LastNetUpdateTime;

    uint32 bPendingNetUpdate : 1;

    uint32 bForceRelevantNextUpdate : 1;

    TSet<TWeakObjectPtr<UNetConnection>> DormantConnections;

    TSet<TWeakObjectPtr<UNetConnection>> RecentlyDormantConnections;

    bool operator==(const FNetworkObjectInfo& Other)
    {
        return Actor == Other.Actor;
    }
};

class FNetworkObjectList
{
public:
    using FNetworkObjectSet = TSet<TSharedPtr<FNetworkObjectInfo>>;

    FNetworkObjectSet AllNetworkObjects;
    FNetworkObjectSet ActiveNetworkObjects;
    FNetworkObjectSet ObjectsDormantOnAllConnections;

    TMap<TWeakObjectPtr<UObject>, int32> NumDormantObjectsPerConnection;
};

FORCEINLINE int32& GetReplicationFrame(UNetDriver* Driver)
{
    return *(int32*)(int64(Driver) + 816); // Offsets::Net::ReplicationFrame);
}

FORCEINLINE auto& GetNetworkObjectList(UObject* NetDriver)
{
    return *(*(TSharedPtr<FNetworkObjectList>*)(int64(NetDriver) + 0x508));
}

FORCEINLINE UGameplayStatics* GetGameplayStatics()
{
    return reinterpret_cast<UGameplayStatics*>(UGameplayStatics::StaticClass());
}

FORCEINLINE UKismetSystemLibrary* GetKismetSystem()
{
    return reinterpret_cast<UKismetSystemLibrary*>(UKismetSystemLibrary::StaticClass());
}

FORCEINLINE UFortKismetLibrary* GetFortKismet()
{
    return ((UFortKismetLibrary*)UFortKismetLibrary::StaticClass());
}

FORCEINLINE UKismetStringLibrary* GetKismetString()
{
    return (UKismetStringLibrary*)UKismetStringLibrary::StaticClass();
}

static FORCEINLINE void sinCos(float* ScalarSin, float* ScalarCos, float Value)
{
    float quotient = (INV_PI * 0.5f) * Value;
    if (Value >= 0.0f)
    {
        quotient = (float)((int)(quotient + 0.5f));
    }
    else
    {
        quotient = (float)((int)(quotient - 0.5f));
    }
    float y = Value - (2.0f * PI) * quotient;

    float sign;
    if (y > HALF_PI)
    {
        y = PI - y;
        sign = -1.0f;
    }
    else if (y < -HALF_PI)
    {
        y = -PI - y;
        sign = -1.0f;
    }
    else
    {
        sign = +1.0f;
    }

    float y2 = y * y;

    *ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

    float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
    *ScalarCos = sign * p;
}

static auto RotToQuat(FRotator Rotator)
{
    const float DEG_TO_RAD = PI / (180.f);
    const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
    float SP, SY, SR;
    float CP, CY, CR;

    sinCos(&SP, &CP, Rotator.Pitch * DIVIDE_BY_2);
    sinCos(&SY, &CY, Rotator.Yaw * DIVIDE_BY_2);
    sinCos(&SR, &CR, Rotator.Roll * DIVIDE_BY_2);

    FQuat RotationQuat;
    RotationQuat.X = CR * SP * SY - SR * CP * CY;
    RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
    RotationQuat.Z = CR * CP * SY - SR * SP * CY;
    RotationQuat.W = CR * CP * CY + SR * SP * SY;

    return RotationQuat;
}

static auto VecToRot(FVector Vector)
{
    FRotator R;

    R.Yaw = std::atan2(Vector.Y, Vector.X) * (180.f / PI);

    R.Pitch = std::atan2(Vector.Z, std::sqrt(Vector.X * Vector.X + Vector.Y * Vector.Y)) * (180.f / PI);

    // roll can't be found from vector
    R.Roll = 0;

    return R;
}

static AActor* SpawnActorTrans(UClass* StaticClass, FTransform SpawnTransform, AActor* Owner = nullptr, ESpawnActorCollisionHandlingMethod Flags = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
{
    AActor* FirstActor = GetGameplayStatics()->STATIC_BeginDeferredActorSpawnFromClass(GetWorld(), StaticClass, SpawnTransform, Flags, Owner);

    if (FirstActor)
    {
        AActor* FinalActor = GetGameplayStatics()->STATIC_FinishSpawningActor(FirstActor, SpawnTransform);

        if (FinalActor)
        {
            return FinalActor;
        }
    }

    return nullptr;
}

inline auto& GetItemInstances(AFortPlayerController* PC)
{
    return PC->WorldInventory->Inventory.ItemInstances;
}

inline AActor* SpawnActor(UClass* ActorClass, FVector Location = { 0.0f, 0.0f, 0.0f }, FRotator Rotation = { 0, 0, 0 }, AActor* Owner = nullptr, ESpawnActorCollisionHandlingMethod Flags = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
{
    FTransform SpawnTransform;

    SpawnTransform.Translation = Location;
    SpawnTransform.Scale3D = FVector{ 1, 1, 1 };
    SpawnTransform.Rotation = RotToQuat(Rotation);

    return SpawnActorTrans(ActorClass, SpawnTransform, Owner, Flags);
}

template <typename RetActorType = AActor>
inline RetActorType* SpawnActor(FVector Location = { 0.0f, 0.0f, 0.0f }, AActor* Owner = nullptr, FQuat Rotation = { 0, 0, 0 }, ESpawnActorCollisionHandlingMethod Flags = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
{
    FTransform SpawnTransform;

    SpawnTransform.Translation = Location;
    SpawnTransform.Scale3D = FVector{ 1, 1, 1 };
    SpawnTransform.Rotation = Rotation;

    return (RetActorType*)SpawnActorTrans(RetActorType::StaticClass(), SpawnTransform, Owner, Flags);
}

inline ABuildingSMActor* SpawnBuilding(UClass* BGAClass, FVector& Location, FRotator& Rotation, APlayerPawn_Athena_C* Pawn)
{
    FTransform Transform;
    Transform.Translation = Location;
    Transform.Scale3D = FVector{ 1, 1, 1 };
    Transform.Rotation = RotToQuat(Rotation);

    return (ABuildingSMActor*)GetFortKismet()->STATIC_SpawnBuildingGameplayActor(BGAClass, Transform, Pawn);
}

inline void CreateConsole()
{
    GetEngine()->GameViewport->ViewportConsole = (UConsole*)GetGameplayStatics()->STATIC_SpawnObject(UConsole::StaticClass(), GetEngine()->GameViewport);
}

inline auto CreateCheatManager(APlayerController* Controller)
{
    if (!Controller->CheatManager)
    {
        Controller->CheatManager = (UCheatManager*)GetGameplayStatics()->STATIC_SpawnObject(UFortCheatManager::StaticClass(), Controller); // lets just assume its gamemode athena
    }

    return (UFortCheatManager*)Controller->CheatManager;
}

DWORD WINAPI MapLoadThread(LPVOID) // thnak you mr rythm for giving me this
{
    // std::cout << "There is " << GetWorld()->StreamingLevels.Num() << " currently loading" << '\n';

    for (int i = 0; i < GetWorld()->StreamingLevels.Num(); i++)
    {
        auto StreamingLevel = GetWorld()->StreamingLevels[i];

		if (!StreamingLevel)
            continue;

        // std::cout << StreamingLevel->GetName() << " state: " << (StreamingLevel->IsLevelLoaded() ? "Loaded" : "Loading") << '\n';

        if (StreamingLevel->IsLevelLoaded())
            continue;

        Sleep(1000);
    }
		
	Native::OnlineBeacon::PauseBeaconRequests(HostBeacon, false);

    // Beacon->BeaconState = EBeaconState::AllowRequests;
    std::cout << "People can join now!\n";
	
    bMapFullyLoaded = true;

	return 0;
}

bool CanBuild(ABuildingSMActor* BuildingActor)
{
    /*
    FBuildingGridActorFilter filter { true, true, true, true};
    FBuildingNeighboringActorInfo OutActors;
    GameState->StructuralSupportSystem->K2_GetBuildingActorsInGridCell(Location, filter, &OutActors);
    auto Amount = OutActors.NeighboringCenterCellInfos.Num() + OutActors.NeighboringFloorInfos.Num() + OutActors.NeighboringWallInfos.Num();
    if (Amount == 0)
        return true; */

    for (const auto Building : Buildings)
    {
        if (!Building) // || Building->bDestroyed)
        {
            Buildings.erase(Building);
            continue;
        }

        if (Building->bDestroyed)
        {
            Buildings.erase(Building);
            break;
        }

        if (Building->K2_GetActorLocation() == BuildingActor->K2_GetActorLocation()) // If we use a vector of locations, I do not know how to track if the actor has been destroyed. // Maybe we could use a map so we dont get the location everytime
        {
             //if (!BuildingClass->IsA(APBWA_W1_StairW_C::StaticClass()) || (BuildingClass->IsA(APBWA_W1_StairW_C::StaticClass()) && Building->BuildingType == EFortBuildingType::Stairs))
            {
                return false;
            }
        }
    }

    return true;
}

bool CanBuild2(ABuildingSMActor* BuildingActor, AFortPlayerController_ServerCreateBuildingActor_Params* Params)
{
    static auto GameState = reinterpret_cast<AAthena_GameState_C*>(GetWorld()->GameState);

    TArray<ABuildingActor*> ExistingBuildings;
    EFortStructuralGridQueryResults bCanBuild = GameState->StructuralSupportSystem->K2_CanAddBuildingActorToGrid(GetWorld(), BuildingActor, BuildingActor->K2_GetActorLocation(), BuildingActor->K2_GetActorRotation(), Params->bMirrored, true, &ExistingBuildings);

	if (bCanBuild == EFortStructuralGridQueryResults::CanAdd && bCanBuild != EFortStructuralGridQueryResults::ExistingActor || ExistingBuildings.Num() == 0)
        return true;

    // this check allows u to build anywhere but sadly it does slow down building a bit :/
    if (bCanBuild == EFortStructuralGridQueryResults::ExistingActor || ExistingBuildings.Num() == 0)
        return true;

    return false;
}

inline void Build(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActor, AFortPlayerController_ServerCreateBuildingActor_Params* Params)
{
    if (BuildingActor && CanBuild2(BuildingActor, Params))
    {
        BuildingActor->DynamicBuildingPlacementType = EDynamicBuildingPlacementType::DestroyAnythingThatCollides;
        BuildingActor->SetMirrored(Params->bMirrored);
        BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, PC);
        auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
        BuildingActor->Team = PlayerState->TeamIndex;
        PlayerBuilds.push_back(BuildingActor);
    }
    else
    {
        BuildingActor->SetActorScale3D({});
        BuildingActor->SilentDie();
    }
}

inline FFortItemEntry GetEntryInSlot(AFortPlayerControllerAthena* Controller, int Slot, int Item = 0, EFortQuickBars QuickBars = EFortQuickBars::Primary)
{
    if (!Controller)
        return FFortItemEntry();

    auto& ItemInstances = Controller->WorldInventory->Inventory.ItemInstances;

    FGuid ToFindGuid;

    if (QuickBars == EFortQuickBars::Primary)
        ToFindGuid = Controller->QuickBars->PrimaryQuickBar.Slots[Slot].Items[Item];
    else if (QuickBars == EFortQuickBars::Secondary)
        ToFindGuid = Controller->QuickBars->SecondaryQuickBar.Slots[Slot].Items[Item];

    for (int j = 0; j < ItemInstances.Num(); j++)
    {
        auto ItemInstance = ItemInstances[j];

        if (!ItemInstance)
            continue;

        auto Guid = ItemInstance->ItemEntry.ItemGuid;

        if (ToFindGuid == Guid)
            return ItemInstance->ItemEntry;
    }

    return FFortItemEntry();
}

bool IsCurrentlyDisconnecting(UNetConnection* Connection)
{
    return false;
}

void Spectate(UNetConnection* SpectatingConnection, AFortPlayerStateAthena* StateToSpectate)
{
    if (!SpectatingConnection || !StateToSpectate)
        return;

    auto PawnToSpectate = StateToSpectate->GetCurrentPawn();
    auto DeadPC = (AFortPlayerControllerAthena*)SpectatingConnection->PlayerController;

    if (!DeadPC)
        return;

    auto DeadPlayerState = (AFortPlayerStateAthena*)DeadPC->PlayerState;

    if (!IsCurrentlyDisconnecting(SpectatingConnection) && DeadPlayerState && PawnToSpectate)
    {
        DeadPC->PlayerToSpectateOnDeath = PawnToSpectate;
        DeadPC->ClientSetSpectatorCamera(PawnToSpectate->K2_GetActorLocation(), PawnToSpectate->K2_GetActorRotation());
        DeadPC->SpectateOnDeath();

        DeadPlayerState->SpectatingTarget = StateToSpectate;
        DeadPlayerState->bIsSpectator = true;
        //DeadPC->SetViewTargetWithBlend(PawnToSpectate, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
        DeadPlayerState->OnRep_SpectatingTarget();

		// 95% of the code below here is useless, it was my attempt to fix the camera.
        
        auto SpectatorPC = SpawnActor<ABP_SpectatorPC_C>(PawnToSpectate->K2_GetActorLocation());
        SpectatorPC->SetNewCameraType(ESpectatorCameraType::DroneAttach, true);
        SpectatorPC->CurrentCameraType = ESpectatorCameraType::DroneAttach;
        SpectatorPC->ResetCamera();
        SpectatingConnection->PlayerController = SpectatorPC;
        SpectatingConnection->ViewTarget = PawnToSpectate;
		SpectatorPC->FollowedPlayerPrivate = StateToSpectate;
		SpectatorPC->HoveredPlayerPrivate = StateToSpectate;
        //SpectatorPC->ToggleSpectatorHUD();

		if (SpectatorPC->CurrentSpectatorCamComp)
           SpectatorPC->CurrentSpectatorCamComp->IntendedViewTarget = PawnToSpectate;

        auto SpectatorPawn = SpawnActor<AArenaCamPawn>(PawnToSpectate->K2_GetActorLocation(), PawnToSpectate);

        SpectatorPawn->SpectatorController = SpectatorPC;

        if (SpectatorPawn->SpectatorCameraComponent)
            SpectatorPawn->SpectatorCameraComponent->IntendedViewTarget = PawnToSpectate;
		
        SpectatorPC->SpectatorPawn = SpectatorPawn;
        SpectatorPC->Pawn = SpectatorPawn;
        SpectatorPC->AcknowledgedPawn = SpectatorPawn;
        SpectatorPawn->Owner = SpectatorPC;
        SpectatorPawn->OnRep_Owner();
        SpectatorPC->OnRep_Pawn();
        SpectatorPC->Possess(SpectatorPawn);
        DeadPC->K2_DestroyActor();

		SpectatorPC->Pawn->bUseControllerRotationPitch = true;
        SpectatorPC->Pawn->bUseControllerRotationYaw = true;
		SpectatorPC->Pawn->bUseControllerRotationRoll = true;

        if (DeadPC->QuickBars)
            DeadPC->QuickBars->K2_DestroyActor();
    }
}

inline void UpdateInventory(AFortPlayerController* PlayerController, int Dirty = 0, bool bRemovedItem = false)
{
    PlayerController->WorldInventory->bRequiresLocalUpdate = true;
    PlayerController->WorldInventory->HandleInventoryLocalUpdate();
    PlayerController->HandleWorldInventoryLocalUpdate();
    PlayerController->ForceUpdateQuickbar(EFortQuickBars::Primary);
    PlayerController->QuickBars->OnRep_PrimaryQuickBar();
    PlayerController->QuickBars->OnRep_SecondaryQuickBar();
    PlayerController->QuickBars->ForceNetUpdate();

    if (bRemovedItem)
        PlayerController->WorldInventory->Inventory.MarkArrayDirty();
}

inline auto AddItem(AFortPlayerController* PC, UFortItemDefinition* Def, int Slot, EFortQuickBars Bars = EFortQuickBars::Primary, int Count = 1)
{
    if (!PC || !Def)
        return FFortItemEntry();

    if (Def->IsA(UFortWeaponItemDefinition::StaticClass()))
        Count = 1;

    if (Slot < 0)
        Slot = 1;

    if (Bars == EFortQuickBars::Primary && Slot >= 6)
        Slot = 5;

    auto& QuickBarSlots = PC->QuickBars->PrimaryQuickBar.Slots;

    auto TempItemInstance = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 1);

    if (TempItemInstance)
    {
        TempItemInstance->SetOwningControllerForTemporaryItem(PC);

        TempItemInstance->ItemEntry.Count = Count;
        TempItemInstance->OwnerInventory = PC->WorldInventory;

        auto& ItemEntry = TempItemInstance->ItemEntry;

        auto Idx = PC->WorldInventory->Inventory.ReplicatedEntries.Add(ItemEntry);

        GetItemInstances(PC).Add((UFortWorldItem*)TempItemInstance);
        PC->QuickBars->ServerAddItemInternal(ItemEntry.ItemGuid, Bars, Slot);

        if (Idx && PC->WorldInventory->Inventory.ReplicatedEntries.Num() >= Idx)
            PC->WorldInventory->Inventory.MarkItemDirty(PC->WorldInventory->Inventory.ReplicatedEntries[Idx]);

        return ItemEntry;
    }

    return FFortItemEntry();
}

inline auto AddItemWithUpdate(AFortPlayerController* PC, UFortItemDefinition* Def, int Slot, EFortQuickBars Bars = EFortQuickBars::Primary, int Count = 1)
{
    auto ItemEntry = AddItem(PC, Def, Slot, Bars, Count);

    UpdateInventory(PC);

    return ItemEntry;
}

inline UFortWorldItem* GetInstanceFromGuid(AController* PC, const FGuid& ToFindGuid)
{
    auto& ItemInstances = GetItemInstances((AFortPlayerController*)PC);
    for (int j = 0; j < ItemInstances.Num(); j++)
    {
        auto ItemInstance = ItemInstances[j];

        if (!ItemInstance)
            continue;

        auto Guid = ItemInstance->ItemEntry.ItemGuid;

        if (ToFindGuid == Guid)
        {
            return ItemInstance;
        }
    }

    return nullptr;
}

inline FFortItemEntry GetEntryFromGuid(AController* PC, const FGuid& ToFindGuid)
{
    return GetInstanceFromGuid(PC, ToFindGuid)->ItemEntry;
}

inline UFortItemDefinition* GetDefInSlot(AFortPlayerControllerAthena* PC, int Slot, int Item = 0)
{
    return GetEntryFromGuid(PC, PC->QuickBars->PrimaryQuickBar.Slots[Slot].Items[Item]).ItemDefinition;
}

inline bool IsGuidInInventory(AFortPlayerControllerAthena* Controller, const FGuid& Guid)
{
    auto& QuickBarSlots = Controller->QuickBars->PrimaryQuickBar.Slots;

    for (int i = 0; i < QuickBarSlots.Num(); i++)
    {
        if (QuickBarSlots[i].Items.Data)
        {
            auto items = QuickBarSlots[i].Items;

            for (int i = 0; items.Num(); i++)
            {
                if (items[i] == Guid)
                    return true;
            }
        }
    }

    return false;
}

inline AFortWeapon* EquipWeaponDefinition(APawn* dPawn, UFortWeaponItemDefinition* Definition, const FGuid& Guid, int Ammo = -1, bool bEquipWithMaxAmmo = false) // don't use, use EquipInventoryItem // not too secure
{
    auto weaponClass = Definition->GetWeaponActorClass();
    auto Pawn = (APlayerPawn_Athena_C*)dPawn;
    if (Pawn && Definition && weaponClass)
    {
        auto Controller = (AFortPlayerControllerAthena*)Pawn->Controller;
        auto Instance = GetInstanceFromGuid(Controller, Guid);

        if (!IsGuidInInventory(Controller, Guid))
            return nullptr;

		AFortWeapon* Weapon = nullptr;

		// we have to do this for the traptool idk why

		if (weaponClass->GetFullName() == "BlueprintGeneratedClass TrapTool.TrapTool_C") // (weaponClass->IsA(AFortTrapTool::StaticClass()))
        {
            Weapon = (AFortWeapon*)SpawnActorTrans(weaponClass, {}, Pawn); // Other people can't see their weapon.
			
            if (Weapon)
            {
                Weapon->bReplicates = true;
                Weapon->bOnlyRelevantToOwner = false;

                ((AFortTrapTool*)Weapon)->ItemDefinition = Definition;	
            }
        }
		
        else
          Weapon = Pawn->EquipWeaponDefinition(Definition, Guid);

        if (Weapon)
        {
            Weapon->WeaponData = Definition;
            Weapon->ItemEntryGuid = Guid;
			
            if (bEquipWithMaxAmmo)
                Weapon->AmmoCount = Weapon->GetBulletsPerClip();
			else if (Ammo != -1)
				Weapon->AmmoCount = Instance->ItemEntry.LoadedAmmo;

            Instance->ItemEntry.LoadedAmmo = Weapon->AmmoCount;

            Weapon->SetOwner(dPawn);
            Weapon->OnRep_ReplicatedWeaponData();
            Weapon->OnRep_AmmoCount();
            Weapon->ClientGivenTo(Pawn);
            Pawn->ClientInternalEquipWeapon(Weapon);
            Pawn->OnRep_CurrentWeapon(); // i dont think this is needed but alr
        }

        return Weapon;
    }

    return nullptr;
}

inline void EquipInventoryItem(AFortPlayerControllerAthena* PC, FGuid& Guid)
{
    if (!PC || PC->IsInAircraft())
        return;

    auto& ItemInstances = GetItemInstances(PC);
	
    for (int i = 0; i < ItemInstances.Num(); i++)
    {
        auto CurrentItemInstance = ItemInstances[i];

        if (!CurrentItemInstance)
            continue;

        auto Def = (UFortWeaponItemDefinition*)CurrentItemInstance->GetItemDefinitionBP();

        if (CurrentItemInstance->GetItemGuid() == Guid && Def)
        {
            EquipWeaponDefinition((APlayerPawn_Athena_C*)PC->Pawn, Def, Guid, -1, true); // CurrentItemInstance->ItemEntry.LoadedAmmo);
            break;
        }
    }
}

inline void DumpObjects()
{
    std::ofstream objects("ObjectsDump.txt");

    if (objects)
    {
        for (int i = 0; i < UObject::GObjects->Num(); i++)
        {
            auto Object = UObject::GObjects->GetByIndex(i);

            if (!Object)
                continue;

            objects << '[' + std::to_string(Object->InternalIndex) + "] " + Object->GetFullName() << '\n';
        }
    }

    objects.close();

    std::cout << "Finished dumping objects!\n";
}

static AFortPickup* SummonPickup(AFortPlayerPawn* Pawn, auto ItemDef, int Count, FVector Location)
{
    auto FortPickup = SpawnActor<AFortPickup>(Location, Pawn);

    FortPickup->bReplicates = true; // should be autmoatic but eh

    FortPickup->PrimaryPickupItemEntry.Count = Count;
    FortPickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;

    FortPickup->OnRep_PrimaryPickupItemEntry();
    FortPickup->TossPickup(Location, Pawn, 6, true);

    return FortPickup;
}

static void SummonPickupFromChest(auto ItemDef, int Count, FVector Location)
{
    auto FortPickup = SpawnActor<AFortPickup>(Location);

    FortPickup->bReplicates = true; // should be autmoatic but eh
    FortPickup->bTossedFromContainer = true;

    FortPickup->PrimaryPickupItemEntry.Count = Count;
    FortPickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;

    FortPickup->OnRep_PrimaryPickupItemEntry();
    FortPickup->OnRep_TossedFromContainer();
}

inline void SpawnPickupFromFloorLoot(auto ItemDef, int Count, FVector Location)
{
    auto FortPickup = SpawnActor<AFortPickup>(Location);

    FortPickup->bReplicates = true; // should be autmoatic but eh

    FortPickup->PrimaryPickupItemEntry.Count = Count;
    FortPickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;

    FortPickup->OnRep_PrimaryPickupItemEntry();
}

static void InitInventory(AFortPlayerController* PlayerController, bool SecondaryItems)
{
    PlayerController->QuickBars = SpawnActor<AFortQuickBars>({ -280, 400, 3000 }, PlayerController);
    auto QuickBars = PlayerController->QuickBars;
    PlayerController->OnRep_QuickBar();
    
    if (SecondaryItems)
    {
        static auto Wall = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
        static auto Stair = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
        static auto Cone = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
        static auto Floor = UObject::FindObject<UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
        static auto Wood = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
        static auto Stone = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
        static auto Metal = UObject::FindObject<UFortResourceItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
        static auto Shells = UObject::FindObject<UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataShells.AthenaAmmoDataShells");
        static auto Medium = UObject::FindObject<UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
        static auto Light = UObject::FindObject<UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
        static auto Heavy = UObject::FindObject<UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
        static auto EditTool = UObject::FindObject<UFortAmmoItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
        static auto LaunchPad = UObject::FindObject<UFortTrapItemDefinition>("FortTrapItemDefinition TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena");
        // static auto Trap = UObject::FindObject<UFortTrapItemDefinition>("FortTrapItemDefinition TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena");
        // static auto Trap2 = UObject::FindObject<UFortTrapItemDefinition>("FortTrapItemDefinition TID_Wall_Electric_Athena_R_T03.TID_Wall_Electric_Athena_R_T03");
        // static auto Trap3 = UObject::FindObject<UFortTrapItemDefinition>("FortTrapItemDefinition TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03");

        // we should probably only update once

        AddItem(PlayerController, Wall, 0, EFortQuickBars::Secondary, 1);
        AddItem(PlayerController, Floor, 1, EFortQuickBars::Secondary, 1);
        AddItem(PlayerController, Stair, 2, EFortQuickBars::Secondary, 1);
        AddItem(PlayerController, Cone, 3, EFortQuickBars::Secondary, 1);
        AddItem(PlayerController, LaunchPad, 4, EFortQuickBars::Secondary, 1);
        // AddItem(PlayerController, Trap, 4, EFortQuickBars::Secondary, 1);
        // AddItem(PlayerController, Trap2, 5, EFortQuickBars::Secondary, 1);
        // AddItem(PlayerController, Trap3, 6, EFortQuickBars::Secondary, 1);

        AddItem(PlayerController, Wood, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Stone, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Metal, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Shells, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Medium, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Light, 0, EFortQuickBars::Secondary, 999);
        AddItem(PlayerController, Heavy, 0, EFortQuickBars::Secondary, 999);

        AddItemWithUpdate(PlayerController, EditTool, 0, EFortQuickBars::Primary, 1);
    }

    QuickBars->ServerActivateSlotInternal(EFortQuickBars::Primary, 0, 0, true);
}

template <typename Class>
static FFortItemEntry FindItemInInventory(AFortPlayerControllerAthena* PC, bool& bFound)
{
    auto& ItemInstances = GetItemInstances(PC);

    for (int i = 0; i < ItemInstances.Num(); i++)
    {
        auto ItemInstance = ItemInstances[i];

        if (!ItemInstance)
            continue;

        auto Def = ItemInstance->ItemEntry.ItemDefinition;

        if (Def && Def->IsA(Class::StaticClass()))
        {
            //std::cout << i << " Pickaxe Index\n";
            bFound = true;
            return ItemInstance->ItemEntry;
        }
    }

    bFound = false;
    return FFortItemEntry();
}

FGameplayAbilitySpec* UAbilitySystemComponent_FindAbilitySpecFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
    auto Specs = AbilitySystemComponent->ActivatableAbilities.Items;

    for (int i = 0; i < Specs.Num(); i++)
    {
        auto& Spec = Specs[i];

        if (Spec.Handle.Handle == Handle.Handle)
        {
            return &Spec;
        }
    }

    return nullptr;
}

void UAbilitySystemComponent_ConsumeAllReplicatedData(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey)
{
    /*
    FGameplayAbilitySpecHandleAndPredictionKey toFind { AbilityHandle, AbilityOriginalPredictionKey.Current };

    auto MapPairsData = AbilitySystemComponent->AbilityTargetDataMap;
    */
}

auto TryActivateAbility(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, FPredictionKey* PredictionKey, FGameplayEventData* TriggerEventData)
{
    auto Spec = UAbilitySystemComponent_FindAbilitySpecFromHandle(AbilitySystemComponent, AbilityToActivate);

    if (!Spec)
    {
        printf("InternalServerTryActiveAbility. Rejecting ClientActivation of ability with invalid SpecHandle!\n");
        AbilitySystemComponent->ClientActivateAbilityFailed(AbilityToActivate, PredictionKey->Current);
        return;
    }

    // UAbilitySystemComponent_ConsumeAllReplicatedData(AbilitySystemComponent, AbilityToActivate, *PredictionKey);

    UGameplayAbility* InstancedAbility = nullptr;
    Spec->InputPressed = true;

    if (Native::AbilitySystemComponent::InternalTryActivateAbility(AbilitySystemComponent, AbilityToActivate, *PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
    {
        // TryActivateAbility handles notifying the client of success
    }
    else
    {
        printf("InternalServerTryActiveAbility. Rejecting ClientActivation of %s. InternalTryActivateAbility failed\n", Spec->Ability->GetName().c_str());
        AbilitySystemComponent->ClientActivateAbilityFailed(AbilityToActivate, PredictionKey->Current);
        Spec->InputPressed = false;
        return;
    }

    Native::AbilitySystemComponent::MarkAbilitySpecDirty(AbilitySystemComponent, *Spec);
}

static auto GrantGameplayAbility(APlayerPawn_Athena_C* TargetPawn, UClass* GameplayAbilityClass)
{
    auto AbilitySystemComponent = TargetPawn->AbilitySystemComponent;

    if (!AbilitySystemComponent)
        return;

    auto GenerateNewSpec = [&]() -> FGameplayAbilitySpec
    {
        FGameplayAbilitySpecHandle Handle{ rand() };

        FGameplayAbilitySpec Spec{ -1, -1, -1, Handle, (UGameplayAbility*)GameplayAbilityClass->CreateDefaultObject(), 1, -1, nullptr, 0, false, false, false };

        return Spec;
    };

    auto Spec = GenerateNewSpec();
	
    for (int i = 0; i < AbilitySystemComponent->ActivatableAbilities.Items.Num(); i++)
    {
        auto& CurrentSpec = AbilitySystemComponent->ActivatableAbilities.Items[i];

        if (CurrentSpec.Ability == Spec.Ability)
            return;
    }

    auto Handle = Native::AbilitySystemComponent::GiveAbility(AbilitySystemComponent, &Spec.Handle, Spec);
}

static bool KickController(APlayerController* PC, FString Message)
{
    if (PC && Message.Data)
    {
        FText text = reinterpret_cast<UKismetTextLibrary*>(UKismetTextLibrary::StaticClass())->STATIC_Conv_StringToText(Message);
        return Native::OnlineSession::KickPlayer(GetWorld()->AuthorityGameMode->GameSession, PC, text);	
    }

    return false;
}

// template <typename SoftType>
UObject* SoftObjectToObject(TSoftObjectPtr<UObject*> SoftPtr)
{
    static auto KismetSystem = GetKismetSystem();
    static auto fn = UObject::FindObject<UFunction>("Function Engine.KismetSystemLibrary.Conv_SoftClassReferenceToClass");

    struct
    {
        TSoftObjectPtr<UObject*> SoftClassReference;
        UObject* Class;
    } params;

    params.SoftClassReference = SoftPtr;

    auto flags = fn->FunctionFlags;
    fn->FunctionFlags |= 0x400;

    ProcessEvent(KismetSystem, fn, &params);

    fn->FunctionFlags = flags;

    return params.Class;
}

auto GetAllActorsOfClass(UClass* Class)
{
    TArray<AActor*> OutActors;

    static auto GameplayStatics = (UGameplayStatics*)UGameplayStatics::StaticClass()->CreateDefaultObject();
    GameplayStatics->STATIC_GetAllActorsOfClass(GetWorld(), Class, &OutActors);

    return OutActors;
}

FTransform GetPlayerStart(AFortPlayerControllerAthena* PC)
{
    TArray<AActor*> OutActors = GetAllActorsOfClass(AFortPlayerStartWarmup::StaticClass());

    auto ActorsNum = OutActors.Num();

    auto SpawnTransform = FTransform();
    SpawnTransform.Scale3D = FVector(1, 1, 1);
    SpawnTransform.Rotation = FQuat();
    SpawnTransform.Translation = FVector { 1250, 1818, 3284 }; // Next to salty

    auto GamePhase = ((AAthena_GameState_C*)GetWorld()->GameState)->GamePhase;

    if (ActorsNum != 0
        && (GamePhase == EAthenaGamePhase::Setup || GamePhase == EAthenaGamePhase::Warmup))
    {
        auto ActorToUseNum = rand() % ActorsNum;
        auto ActorToUse = (OutActors)[ActorToUseNum];

        while (!ActorToUse)
        {
            ActorToUseNum = rand() % ActorsNum;
            ActorToUse = (OutActors)[ActorToUseNum];
        }

        auto Location = ActorToUse->K2_GetActorLocation();
        SpawnTransform.Translation = ActorToUse->K2_GetActorLocation();

        PC->WarmupPlayerStart = (AFortPlayerStartWarmup*)ActorToUse;
    }

    return SpawnTransform;

    // return (GetWorld()->AuthorityGameMode->FindPlayerStart(PC, IncomingName))->K2_GetActorLocation();
}

inline UKismetMathLibrary* GetMath()
{
    return (UKismetMathLibrary*)UKismetMathLibrary::StaticClass();
}

FVector RotToVec(const FRotator& Rotator)
{
    float CP, SP, CY, SY;
    sinCos(&SP, &CP, GetMath()->STATIC_DegreesToRadians(Rotator.Pitch));
    sinCos(&SY, &CY, GetMath()->STATIC_DegreesToRadians(Rotator.Yaw));
    auto V = FVector(CP * CY, CP * SY, SP);

    return V;
}

inline auto ApplyAbilities(APawn* _Pawn) // TODO: Check if the player already has the ability.
{
    auto Pawn = (APlayerPawn_Athena_C*)_Pawn;
	
    /*
    
Ability: Class FortniteGame.FortGameplayAbility_Jump
Ability: BlueprintGeneratedClass GA_DefaultPlayer_Consumable.GA_DefaultPlayer_Consumable_C
Ability: Class FortniteGame.FortGameplayAbility_Sprint
Ability: BlueprintGeneratedClass GA_DefaultPlayer_BuildingCreated.GA_DefaultPlayer_BuildingCreated_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_BuildingRepaired.GA_DefaultPlayer_BuildingRepaired_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_Death.GA_DefaultPlayer_Death_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_Stunned.GA_DefaultPlayer_Stunned_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_ApplyKnockback.GA_DefaultPlayer_ApplyKnockback_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_InteractUse.GA_DefaultPlayer_InteractUse_C
Ability: BlueprintGeneratedClass GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C
Ability: BlueprintGeneratedClass GA_Default_KilledEnemy.GA_Default_KilledEnemy_C
Ability: BlueprintGeneratedClass GAB_PlayerDBNO.GAB_PlayerDBNO_C
Ability: BlueprintGeneratedClass GAB_AthenaDBNO.GAB_AthenaDBNO_C
Ability: BlueprintGeneratedClass GAB_AthenaDBNORevive.GAB_AthenaDBNORevive_C
Ability: BlueprintGeneratedClass GAB_PlayerDBNOResurrect.GAB_PlayerDBNOResurrect_C
Ability: Class FortniteGame.FortGameplayAbility_PlayConversation
Ability: BlueprintGeneratedClass GA_DanceGrenade_Stun.GA_DanceGrenade_Stun_C
Ability: BlueprintGeneratedClass GA_AthenaEnterVehicle.GA_AthenaEnterVehicle_C
Ability: BlueprintGeneratedClass GA_AthenaExitVehicle.GA_AthenaExitVehicle_C
Ability: BlueprintGeneratedClass GA_AthenaInVehicle.GA_AthenaInVehicle_C
    
    */

	/* auto DefaultAbilityKit = UObject::FindObject<UFortAbilitySet>("FortAbilitySet GAS_DefaultPlayer.GAS_DefaultPlayer");
	
    for (int i = 0; i < DefaultAbilityKit->GameplayAbilities.Num(); i++)
    {
        auto Ability = DefaultAbilityKit->GameplayAbilities[i];

		if (!Ability)
            continue;

		GrantGameplayAbility(Pawn, Ability);
    } */
	
    static auto SprintAbility = UObject::FindClass("Class FortniteGame.FortGameplayAbility_Sprint");
    static auto ReloadAbility = UObject::FindClass("Class FortniteGame.FortGameplayAbility_Reload");
    static auto RangedWeaponAbility = UObject::FindClass("Class FortniteGame.FortGameplayAbility_RangedWeapon");
    static auto JumpAbility = UObject::FindClass("Class FortniteGame.FortGameplayAbility_Jump");
    static auto DeathAbility = UObject::FindClass("BlueprintGeneratedClass GA_DefaultPlayer_Death.GA_DefaultPlayer_Death_C");
    static auto InteractUseAbility = UObject::FindClass("BlueprintGeneratedClass GA_DefaultPlayer_InteractUse.GA_DefaultPlayer_InteractUse_C");
    static auto InteractSearchAbility = UObject::FindClass("BlueprintGeneratedClass GA_DefaultPlayer_InteractSearch.GA_DefaultPlayer_InteractSearch_C");
    static auto EmoteAbility = UObject::FindClass("BlueprintGeneratedClass GAB_Emote_Generic.GAB_Emote_Generic_C");
    static auto TrapAbility = UObject::FindClass("BlueprintGeneratedClass GA_TrapBuildGeneric.GA_TrapBuildGeneric_C");
    static auto DanceGrenadeAbility = UObject::FindClass("BlueprintGeneratedClass GA_DanceGrenade_Stun.GA_DanceGrenade_Stun_C");
    static auto RCRocketAbility = UObject::FindClass("BlueprintGeneratedClass GA_Athena_RCRocket.GA_Athena_RCRocket_C");

    GrantGameplayAbility(Pawn, SprintAbility);
    GrantGameplayAbility(Pawn, ReloadAbility);
    GrantGameplayAbility(Pawn, RangedWeaponAbility);
    GrantGameplayAbility(Pawn, JumpAbility);
    GrantGameplayAbility(Pawn, DeathAbility);
    GrantGameplayAbility(Pawn, InteractUseAbility);
    GrantGameplayAbility(Pawn, InteractSearchAbility);
    GrantGameplayAbility(Pawn, EmoteAbility);
    GrantGameplayAbility(Pawn, TrapAbility);
    GrantGameplayAbility(Pawn, DanceGrenadeAbility);
    GrantGameplayAbility(Pawn, RCRocketAbility);
}

static void InitPawn(AFortPlayerControllerAthena* PlayerController, FVector Loc = FVector{ 1250, 1818, 3284 }, FQuat Rotation = FQuat(), bool bResetCharacterParts = true)
{
    if (PlayerController->Pawn)
        PlayerController->Pawn->K2_DestroyActor();

    auto SpawnTransform = FTransform();
    SpawnTransform.Scale3D = FVector(1, 1, 1);
    SpawnTransform.Rotation = Rotation;
    SpawnTransform.Translation = Loc;

    // SpawnTransform = GetPlayerStart(PlayerController);

    auto Pawn = (APlayerPawn_Athena_C*)SpawnActorTrans(APlayerPawn_Athena_C::StaticClass(), SpawnTransform, PlayerController);

    PlayerController->Pawn = Pawn;
    PlayerController->AcknowledgedPawn = Pawn;
    Pawn->Owner = PlayerController;
    Pawn->OnRep_Owner();
    PlayerController->OnRep_Pawn();
    PlayerController->Possess(Pawn);

    Pawn->SetMaxHealth(100);
    Pawn->SetMaxShield(100);
    
    Pawn->HealthRegenGameplayEffect = nullptr;
    Pawn->HealthRegenDelayGameplayEffect = nullptr;

    Pawn->bReplicateMovement = true;
    Pawn->OnRep_ReplicateMovement();

    static auto FortRegisteredPlayerInfo = ((UFortGameInstance*)GetWorld()->OwningGameInstance)->RegisteredPlayers[0]; // UObject::FindObject<UFortRegisteredPlayerInfo>("FortRegisteredPlayerInfo Transient.FortEngine_0_1.FortGameInstance_0_1.FortRegisteredPlayerInfo_0_1");

    if (bResetCharacterParts && FortRegisteredPlayerInfo)
    {
        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;
        static auto Hero = FortRegisteredPlayerInfo->AthenaMenuHeroDef;

        PlayerState->HeroType = Hero->GetHeroTypeBP();
        PlayerState->OnRep_HeroType();

        static auto Head = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
        static auto Body = UObject::FindObject<UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");

        PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Head] = Head;
        PlayerState->CharacterParts[(uint8_t)EFortCustomPartType::Body] = Body;
    }


    UpdateInventory(PlayerController);

    ApplyAbilities(Pawn);
}

void ClientMessage(AFortPlayerControllerAthena* PC, FString Message, bool bSay = false) // Send a message to the user's console.
{
    FName Type = FName(-1);

	if (bSay)
    {
        // https://github.com/EpicGames/UnrealEngine/blob/46544fa5e0aa9e6740c19b44b0628b72e7bbd5ce/Engine/Source/Runtime/Engine/Private/PlayerController.cpp#L1379
    }
	
    PC->ClientMessage(Message, Type, 10000);
}

auto toWStr(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

inline UFortWeaponRangedItemDefinition* FindWID(const std::string& WID)
{
    auto Def = UObject::FindObject<UFortWeaponRangedItemDefinition>("FortWeaponRangedItemDefinition " + WID + '.' + WID);

    if (!Def)
    {
        Def = UObject::FindObject<UFortWeaponRangedItemDefinition>("WID_Harvest_" + WID + "_Athena_C_T01" + ".WID_Harvest_" + WID + "_Athena_C_T01");
		
        if (!Def)
            Def = UObject::FindObject<UFortWeaponRangedItemDefinition>(WID + "." + WID);
    }

    return Def;
}

inline UAthenaGliderItemDefinition* FindGlideID(const std::string& Glider_ID)
{
    auto Def = UObject::FindObject<UAthenaGliderItemDefinition>("AthenaGliderItemDefinition " + Glider_ID + '.' + Glider_ID);

    if (!Def)
    {
        Def = UObject::FindObject<UAthenaGliderItemDefinition>("Glider_ID_" + Glider_ID + "_ShuttleB" + ".Glider_ID_" + Glider_ID + "_ShuttleB");

        if (!Def)
            Def = UObject::FindObject<UAthenaGliderItemDefinition>(Glider_ID + "." + Glider_ID);
    }

    return Def;
}

void EquipLoadout(AFortPlayerControllerAthena* Controller, std::vector<UFortWeaponRangedItemDefinition*> WIDS)
{
    FFortItemEntry pickaxeEntry;

    for (int i = 0; i < WIDS.size(); i++)
    {
        // if (i >= 6)
        // break;

        auto Def = WIDS[i];

        if (Def)
        {
            auto entry = AddItemWithUpdate(Controller, Def, i);
            // auto Instance = GetInstanceFromGuid(Controller, entry.ItemGuid); // todo: not get the entry twice
            // Instance->ItemEntry.LoadedAmmo = 
            EquipWeaponDefinition(Controller->Pawn, Def, entry.ItemGuid, -1, true); // kms

            if (i == 0)
                pickaxeEntry = entry;

            
        }
        
    }

    EquipInventoryItem(Controller, pickaxeEntry.ItemGuid);

}

auto RandomIntInRange(int min, int max)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    static std::uniform_int_distribution<> distr(min, max); // define the range

    return distr(gen);
}

auto GetRandomWID(int skip = 0)
{
    if (skip == 0)
        skip = RandomIntInRange(4, 100);

    return UObject::FindObject<UFortWeaponRangedItemDefinition>("FortWeaponRangedItemDefinition WID_", skip);
}

DWORD WINAPI SummonFloorLoot(LPVOID)
{
    if (!bSpawnedFloorLoot)
    {
        static auto FloorLootClass = UObject::FindObject<UClass>("BlueprintGeneratedClass Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");

        if (!FloorLootClass) // your summoning it too early
            return 1;

        static auto Scar = FindWID("WID_Assault_AutoHigh_Athena_SR_Ore_T03");
        auto FloorLootActors = GetAllActorsOfClass(FloorLootClass);

        /*

    static std::vector<UFortWeaponRangedItemDefinition*> Weapons = {
        GetRandomWID(7),
        GetRandomWID(11),
        GetRandomWID(15),
        GetRandomWID(19),
    };

    for (int i = 0; i < 100; i++)
    {
        // Weapons.push_back(GetRandomWID());
    }

    */

        // it also crashes sometimes if you spawn alot on like constructionscript
        auto AmountOfActorsToSpawn = 20; // FloorLootActors.Num(); // For now, without relevancy we just spawn some.
        int AmountSpawned = 0;

        for (int i = 0; i < AmountOfActorsToSpawn; i++)
        {
            auto FloorLootActor = FloorLootActors[i];
            auto weaponToSpawn = Scar; // Weapons[RandomIntInRange(0, 3)];
            auto Location = FloorLootActor->K2_GetActorLocation();

            if (!FloorLootActor || !weaponToSpawn)
                continue;

            // SpawnPickupFromFloorLoot(weaponToSpawn, 1, Location);
            SummonPickupFromChest(weaponToSpawn, 1, Location);
            AmountSpawned++;

            if (false) // (auto Ammo = weaponToSpawn->GetAmmoWorldItemDefinition_BP())
                SpawnPickupFromFloorLoot(nullptr, 10, Location); // Crashes sometimes idk why
        }

        std::cout << "Spawned " << AmountSpawned << " pickups!\n";
    }

    bSpawnedFloorLoot = true;

    return 0;
}

namespace Inventory // includes quickbars
{
    // todo?: choose a quickbar to update
    inline void Update(AFortPlayerController* Controller, int Dirty = 0, bool bRemovedItem = false) // we automatically do the updating in the inventory so no point of calling this function (besides when adding an item)
    {
        if (!Controller)
            return;

        Controller->WorldInventory->bRequiresLocalUpdate = true;
        Controller->WorldInventory->HandleInventoryLocalUpdate();
        Controller->WorldInventory->ForceNetUpdate();
        Controller->HandleWorldInventoryLocalUpdate();

        Controller->QuickBars->OnRep_PrimaryQuickBar();
        Controller->QuickBars->OnRep_SecondaryQuickBar();
        Controller->QuickBars->ForceNetUpdate();
        Controller->ForceUpdateQuickbar(EFortQuickBars::Primary);
        Controller->ForceUpdateQuickbar(EFortQuickBars::Secondary);

        if (bRemovedItem)
            Controller->WorldInventory->Inventory.MarkArrayDirty();

        if (Dirty != 0 && Controller->WorldInventory->Inventory.ReplicatedEntries.Num() >= Dirty)
            Controller->WorldInventory->Inventory.MarkItemDirty(Controller->WorldInventory->Inventory.ReplicatedEntries[Dirty]);
    }

    inline bool IsValidGuid(AFortPlayerControllerAthena* Controller, const FGuid& Guid)
    {
        if (!Controller)
            return false;

        auto& QuickBarSlots = Controller->QuickBars->PrimaryQuickBar.Slots;

        for (int i = 0; i < QuickBarSlots.Num(); i++)
        {
            if (QuickBarSlots[i].Items.Data)
            {
                auto items = QuickBarSlots[i].Items;

                for (int i = 0; items.Num(); i++)
                {
                    if (items[i] == Guid)
                        return true;
                }
            }
        }

        return false;
    }

    inline auto GetDefinitionInSlot(AFortPlayerControllerAthena* Controller, int Slot, int Item = 0, EFortQuickBars QuickBars = EFortQuickBars::Primary)
    {
        return GetEntryInSlot(Controller, Slot, Item, QuickBars).ItemDefinition;
    }

    inline FFortItemEntry AddItemToSlot(AFortPlayerControllerAthena* Controller, UFortWorldItemDefinition* Definition, int Slot, EFortQuickBars Bars = EFortQuickBars::Primary, int Count = 1, int* Idx = nullptr)
    {
        if (!Controller || !Definition)
            return FFortItemEntry();

        if (Definition->IsA(UFortWeaponItemDefinition::StaticClass()))
            Count = 1; // dont give more than 1 weapon to the same slot

        if (Slot < 0)
            Slot = 1;

        if (Bars == EFortQuickBars::Primary && Slot >= 6)
            Slot = 5;

        auto& QuickBarSlots = Controller->QuickBars->PrimaryQuickBar.Slots;

        auto TempItemInstance = (UFortWorldItem*)Definition->CreateTemporaryItemInstanceBP(Count, 1);

        if (TempItemInstance)
        {
            TempItemInstance->SetOwningControllerForTemporaryItem(Controller);

            TempItemInstance->ItemEntry.Count = Count;
            TempItemInstance->OwnerInventory = Controller->WorldInventory;

            auto& ItemEntry = TempItemInstance->ItemEntry;

            auto _Idx = Controller->WorldInventory->Inventory.ReplicatedEntries.Add(ItemEntry);

            if (Idx)
                *Idx = _Idx;

            Controller->WorldInventory->Inventory.ItemInstances.Add((UFortWorldItem*)TempItemInstance);
            Controller->QuickBars->ServerAddItemInternal(ItemEntry.ItemGuid, Bars, Slot);

			Inventory::Update(Controller, _Idx);

            return ItemEntry;
        }

        return FFortItemEntry();
    }

    inline void EquipSlot(AFortPlayerControllerAthena* Controller, int Slot)
    {
        if (!Controller)
            return;
    }

    inline bool RemoveItemFromSlot(AFortPlayerControllerAthena* Controller, int Slot, EFortQuickBars Quickbars = EFortQuickBars::Primary, int Amount = -1) // -1 for all items in the slot
    {
        if (!Controller)
            return false;

        auto& PrimarySlots = Controller->QuickBars->PrimaryQuickBar.Slots;
        auto& SecondarySlots = Controller->QuickBars->SecondaryQuickBar.Slots;

        bool bPrimaryQuickBar = (Quickbars == EFortQuickBars::Primary);

        if (Amount == -1)
        {
            if (bPrimaryQuickBar)
                Amount = PrimarySlots[Slot].Items.Num();
            else
                Amount = SecondarySlots[Slot].Items.Num();
        }

        bool bWasSuccessful = false;

        for (int i = 0; i < Amount; i++)
        {
            // todo add a check to make sure the slot has that amount of items
            auto& ToRemoveGuid = bPrimaryQuickBar ? PrimarySlots[Slot].Items[i] : SecondarySlots[Slot].Items[i];

            for (int j = 0; j < Controller->WorldInventory->Inventory.ItemInstances.Num(); j++)
            {
                auto ItemInstance = Controller->WorldInventory->Inventory.ItemInstances[j];

                if (!ItemInstance)
                    continue;

                auto Guid = ItemInstance->ItemEntry.ItemGuid;

                if (ToRemoveGuid == Guid)
                {
                    Controller->WorldInventory->Inventory.ItemInstances.RemoveAt(j);
                    bWasSuccessful = true;
                    // break;
                }
            }

            for (int x = 0; x < Controller->WorldInventory->Inventory.ReplicatedEntries.Num(); x++)
            {
                auto& ItemEntry = Controller->WorldInventory->Inventory.ReplicatedEntries[x];

                if (ItemEntry.ItemGuid == ToRemoveGuid)
                {
                    Controller->WorldInventory->Inventory.ReplicatedEntries.RemoveAt(x);
                    bWasSuccessful = true;

                    // break;
                }
            }

            Controller->QuickBars->ServerRemoveItemInternal(ToRemoveGuid, false, true);
            ToRemoveGuid.Reset();
        }

        if (bWasSuccessful) // Make sure it acutally removed from the ItemInstances and ReplicatedEntries
        {
            Controller->QuickBars->EmptySlot(Quickbars, Slot);

            if (bPrimaryQuickBar)
                PrimarySlots[Slot].Items.FreeArray();
            else
                SecondarySlots[Slot].Items.FreeArray();

            // bPrimaryQuickBar ? PrimarySlots[Slot].Items.FreeArray() : SecondarySlots[Slot].Items.FreeArray();
        }

        Inventory::Update(Controller, 0, true);

        return bWasSuccessful;
    }

    inline bool OnDrop(AFortPlayerControllerAthena* Controller, void* params)
    {
        auto Params = (AFortPlayerController_ServerAttemptInventoryDrop_Params*)params;

        if (!Params || !Controller)
            return false;

        auto& ItemInstances = Controller->WorldInventory->Inventory.ItemInstances;
        auto QuickBars = Controller->QuickBars;

        auto& PrimaryQuickBarSlots = QuickBars->PrimaryQuickBar.Slots;
        auto& SecondaryQuickBarSlots = QuickBars->SecondaryQuickBar.Slots;

        bool bWasSuccessful = false;
		
        for (int i = 1; i < PrimaryQuickBarSlots.Num(); i++)
        {
            if (PrimaryQuickBarSlots[i].Items.Data)
            {
                for (int j = 0; j < PrimaryQuickBarSlots[i].Items.Num(); j++)
                {
                    if (PrimaryQuickBarSlots[i].Items[j] == Params->ItemGuid)
                    {
                        auto Instance = GetInstanceFromGuid(Controller, Params->ItemGuid);
                        
                        if (Instance)
                        {
                            auto Definition = Instance->ItemEntry.ItemDefinition;
                            auto SuccessfullyRemoved = Inventory::RemoveItemFromSlot(Controller, i, EFortQuickBars::Primary, j + 1);

                            if (Definition && SuccessfullyRemoved)
                            {
                                auto Pickup = SummonPickup((AFortPlayerPawn*)Controller->Pawn, Definition, 1, Controller->Pawn->K2_GetActorLocation());
                                Pickup->PrimaryPickupItemEntry.LoadedAmmo = Instance->GetLoadedAmmo();
                                bWasSuccessful = true;
                                break;
                            }
                            else
                                std::cout << "Could not find Definition!\n";
                        }
                    }
                }
            }
        }

        if (!bWasSuccessful)
        {
            for (int i = 0; i < SecondaryQuickBarSlots.Num(); i++)
            {
                if (SecondaryQuickBarSlots[i].Items.Data)
                {
                    for (int j = 0; j < SecondaryQuickBarSlots[i].Items.Num(); j++)
                    {
                        if (SecondaryQuickBarSlots[i].Items[j] == Params->ItemGuid)
                        {
                            auto Definition = Inventory::GetDefinitionInSlot(Controller, i, j, EFortQuickBars::Secondary);
                            auto bSucceeded = Inventory::RemoveItemFromSlot(Controller, i, EFortQuickBars::Secondary, j + 1);

                            if (Definition && bSucceeded)
                            {
                                SummonPickup((AFortPlayerPawn*)Controller->Pawn, Definition, 1, Controller->Pawn->K2_GetActorLocation());
                                bWasSuccessful = true;
                                break;
                            }
                            else
                                std::cout << "Could not find Definition!\n";
                        }
                    }
                }
            }
        }

        if (bWasSuccessful && PrimaryQuickBarSlots[0].Items.Data)
            EquipInventoryItem(Controller, PrimaryQuickBarSlots[0].Items[0]); // just select pickaxe for now

        /* for (int i = ItemInstances.Num(); i > 0; i--) // equip the item before until its valid
        {
            auto ItemInstance = ItemInstances[i];

            if (!ItemInstance)
                continue;

            auto Def = ItemInstance->ItemEntry.ItemDefinition;

            if (Def) // && Def->IsA(UFortWeaponItemDefinition::StaticClass()))
            {
                QuickBars->PrimaryQuickBar.CurrentFocusedSlot = i;
                // EquipInventoryItem(Controller, ItemInstance->ItemEntry.ItemGuid, ItemInstance->ItemEntry.Count);
                QuickBars->ServerActivateSlotInternal(EFortQuickBars::Primary, 0, 0, true);
                break;
            }
        } */

        return bWasSuccessful;
    }

    inline void OnPickup(AFortPlayerControllerAthena* Controller, void* params)
    {
        auto Params = (AFortPlayerPawn_ServerHandlePickup_Params*)params;

        if (!Controller || !Params)
            return;

        auto& ItemInstances = Controller->WorldInventory->Inventory.ItemInstances;

        if (Params->Pickup)
        {
            bool bCanGoInSecondary = true; // there is no way this is how you do it // todo: rename

            if (Params->Pickup->PrimaryPickupItemEntry.ItemDefinition->IsA(UFortWeaponItemDefinition::StaticClass()) && !Params->Pickup->PrimaryPickupItemEntry.ItemDefinition->IsA(UFortDecoItemDefinition::StaticClass()))
                bCanGoInSecondary = false;

            auto WorldItemDefinition = (UFortWorldItemDefinition*)Params->Pickup->PrimaryPickupItemEntry.ItemDefinition;

            if (!bCanGoInSecondary)
            {
                auto& PrimaryQuickBarSlots = Controller->QuickBars->PrimaryQuickBar.Slots;

                for (int i = 1; i < PrimaryQuickBarSlots.Num(); i++)
                {
                    if (!PrimaryQuickBarSlots[i].Items.Data) // Checks if the slot is empty
                    {
                        if (Params->Pickup->IsActorBeingDestroyed() || Params->Pickup->bPickedUp)
                            return;
						
                        if (i >= 6)
                        {
                            auto QuickBars = Controller->QuickBars;

                            auto FocusedSlot = QuickBars->PrimaryQuickBar.CurrentFocusedSlot;

                            if (FocusedSlot == 0) // don't replace the pickaxe
                                continue;

                            i = FocusedSlot;

                            FGuid& FocusedGuid = PrimaryQuickBarSlots[FocusedSlot].Items[0];

                            for (int j = 0; i < ItemInstances.Num(); j++)
                            {
                                auto ItemInstance = ItemInstances[j];

                                if (!ItemInstance)
                                    continue;

                                auto Def = ItemInstance->ItemEntry.ItemDefinition;
                                auto Guid = ItemInstance->ItemEntry.ItemGuid;

                                if (FocusedGuid == Guid)
                                {
                                    // if (Params->Pickup->MultiItemPickupEntries)
                                    SummonPickup((APlayerPawn_Athena_C*)Controller->Pawn, Def, 1 /* ItemInstance->ItemEntry.Count */, Controller->Pawn->K2_GetActorLocation());
                                    break;
                                }
                            }

                            Inventory::RemoveItemFromSlot(Controller, FocusedSlot, EFortQuickBars::Primary);
                        }

						int Idx = 0;
                        auto entry = Inventory::AddItemToSlot(Controller, WorldItemDefinition, i, EFortQuickBars::Primary, Params->Pickup->PrimaryPickupItemEntry.Count, &Idx);
                        // auto& Entry = Controller->WorldInventory->Inventory.ReplicatedEntries[Idx];
                        auto Instance = GetInstanceFromGuid(Controller, entry.ItemGuid);
                        Params->Pickup->K2_DestroyActor();

						Params->Pickup->bPickedUp = true;
                        Params->Pickup->OnRep_bPickedUp();

                        Instance->ItemEntry.LoadedAmmo = Params->Pickup->PrimaryPickupItemEntry.LoadedAmmo;
						
                        Inventory::Update(Controller);

                        break;
                    }
                }
            }

            else
            {
                auto& SecondaryQuickBarSlots = Controller->QuickBars->SecondaryQuickBar.Slots;

                for (int i = 0; i < SecondaryQuickBarSlots.Num(); i++)
                {
                    if (!SecondaryQuickBarSlots[i].Items.Data) // Checks if the slot is empty
                    {
                        auto entry = Inventory::AddItemToSlot(Controller, WorldItemDefinition, i, EFortQuickBars::Secondary, Params->Pickup->PrimaryPickupItemEntry.Count);
                        Params->Pickup->K2_DestroyActor();

                        break;
                    }
                }
            }
        }
    }
}

void EquipTrapTool(AController* Controller)
{
    static auto TrapDef = UObject::FindObject<UFortTrapItemDefinition>("FortTrapItemDefinition TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena");

    auto TrapTool = (AFortTrapTool*)SpawnActorTrans(TrapDef->GetWeaponActorClass(), {}, Controller);
	
    if (TrapTool && TrapDef)
    {
        TrapTool->ItemDefinition = TrapDef;

        auto Pawn = (APlayerPawn_Athena_C*)Controller->Pawn;
        if (Pawn) // && weaponClass)
        {
            if (TrapTool)
            {
                TrapTool->WeaponData = TrapDef;
                TrapTool->SetOwner(Pawn);
                TrapTool->OnRep_ReplicatedWeaponData();
                TrapTool->ClientGivenTo(Pawn);
                Pawn->ClientInternalEquipWeapon(TrapTool);
                Pawn->OnRep_CurrentWeapon(); // i dont think this is needed but alr
            }
        }	
    }
}

void SpawnDeco(AFortDecoTool* Tool, void* _Params)
{
    if (!_Params)
        return;

	auto Params = (AFortDecoTool_ServerSpawnDeco_Params*)_Params;

	FTransform Transform {};
    Transform.Scale3D = FVector(1, 1, 1);
    Transform.Rotation = RotToQuat(Params->Rotation);
    Transform.Translation = Params->Location;

    // if (Params->AttachedActor->IsA(ABuildingTrap::StaticClass()))
    {
        // auto Trap = (ABuildingTrap*)Params->AttachedActor;

        auto TrapDef = (UFortTrapItemDefinition*)Tool->ItemDefinition;

        if (TrapDef)
        {
            auto Trap = (ABuildingTrap*)SpawnActorTrans(TrapDef->GetBlueprintClass(), Transform);

            if (Trap)
            {
                Trap->TrapData = TrapDef;

                auto Pawn = (APlayerPawn_Athena_C*)Tool->Owner;

                Trap->InitializeKismetSpawnedBuildingActor(Trap, (AFortPlayerController*)Pawn->Controller);

				Trap->AttachedTo = Params->AttachedActor;
                Trap->OnRep_AttachedTo();

				auto TrapAbilitySet = Trap->AbilitySet;

				for (int i = 0; i < TrapAbilitySet->GameplayAbilities.Num(); i++) // this fixes traps crashing the game // don't ask how
                {
                    auto Ability = TrapAbilitySet->GameplayAbilities[i];

                    if (!Ability)
                        continue;

                    GrantGameplayAbility(Pawn, Ability);
                }
            }			
        }	
    }
}

void Ban(const std::wstring& IP, const std::wstring& Reason) // this isn't mcp banning or anything
{
    nlohmann::json json;
    json["IP"] = IP;
    json["Reason"] = Reason;
    auto Json = json.dump(4);

    std::ofstream stream("banned-ips.json", std::ios::app);

    stream << Json << '\n';

    stream.close(); 
}

void Unban(const std::wstring& IP) // TODO
{
}

bool IsBanned(const std::wstring& IP)
{
	/*
    
    {
        "IP": "IP",
        "Reason": ""
    }
    
    */

    std::ifstream input_file("banned-ips.json");
    
    if (!input_file.is_open())
		return false;
	
    std::string line;

    while (std::getline(input_file, line))
    {
        if (std::wstring(line.begin(), line.end()).find(IP) != std::wstring::npos)
        {
            return true;
        }
    }

    return false;
}