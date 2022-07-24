#pragma once
#include "gui.h"
#include "ue4.h"
#include "game.h"
#include "logger.h"

    static std::vector<std::string> AthenaConsumablesNames = {
    "/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields",
    "/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall",
    "/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff",
    "/Game/Athena/Items/Consumables/SuperMedkit/Athena_SuperMedkit.Athena_SuperMedkit",
    "/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade",
    "/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade",
    "/Game/Athena/Items/Consumables/Bush/Athena_Bush.Athena_Bush",
    "/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit",
    "/Game/Athena/Items/Consumables/DanceGrenade/Athena_DanceGrenade.Athena_DanceGrenade",
};

static std::vector<std::string> AssaultPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"
};

static std::vector<std::string> ShotgunPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_VR.WID_Shotgun_SlugFire_Athena_VR",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_SR.WID_Shotgun_SlugFire_Athena_SR"
};

static std::vector<std::string> SniperPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03"
};

static std::vector<std::string> SMGPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_C_Ore_T02.WID_Pistol_AutoHeavy_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_UC_Ore_T03.WID_Pistol_AutoHeavy_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_R_Ore_T03.WID_Pistol_AutoHeavy_Athena_R_Ore_T03"
};

static std::vector<std::string> PistolPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_VR_Ore_T03.WID_Pistol_SemiAuto_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_C_Ore_T02.WID_Pistol_SixShooter_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_UC_Ore_T03.WID_Pistol_SixShooter_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03"
};

static std::vector<std::string> RocketPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03"
};

static std::vector<std::string> AthenaLootPoolNames = {
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_VR.WID_Shotgun_SlugFire_Athena_VR",
    "/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_SR.WID_Shotgun_SlugFire_Athena_SR",
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_VR_Ore_T03.WID_Assault_LMG_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_SR_Ore_T03.WID_Assault_LMG_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_VR_Ore_T03.WID_Pistol_SemiAuto_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_C_Ore_T02.WID_Pistol_SixShooter_Athena_C_Ore_T02",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_UC_Ore_T03.WID_Pistol_SixShooter_Athena_UC_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03",
    "/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03"
};

static std::vector<std::string> TrapsPoolNames = {
    "/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena",
    "/Game/Athena/Items/Traps/TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03"
};

static std::vector<std::string> ResourcePoolNames = {
    "/Game/Items/ResourcePickups/WoodItemData.WoodItemData",
    "/Game/Items/ResourcePickups/StoneItemData.StoneItemData",
    "/Game/Items/ResourcePickups/MetalItemData.MetalItemData"
};

static std::vector<std::string> AmmoPoolNames = {
    "/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight",
    "/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"
};

namespace Looting
{
    std::vector<UFortWeaponRangedItemDefinition*> AthenaConsumables;
    std::vector<UFortWeaponRangedItemDefinition*> AssaultPool;
    std::vector<UFortWeaponRangedItemDefinition*> ShotgunPool;
    std::vector<UFortWeaponRangedItemDefinition*> SniperPool;
    std::vector<UFortWeaponRangedItemDefinition*> SMGPool;
    std::vector<UFortWeaponRangedItemDefinition*> PistolPool;
    std::vector<UFortWeaponRangedItemDefinition*> RocketPool; 
    std::vector<UFortWeaponRangedItemDefinition*> AthenaLootPool;
    std::vector<UFortTrapItemDefinition*> TrapsPool;
    std::vector<UFortResourceItemDefinition*> ResourcesPool;
    std::vector<UFortAmmoItemDefinition*> AmmoPool;

    bool bInitialized = false;

    inline auto GetMathLibrary()
    {
        static auto MathLib = (UKismetMathLibrary*)UKismetMathLibrary::StaticClass();
        return MathLib;
    }

    void Init()
    {
        for (int i = 0; i < AthenaConsumablesNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(AthenaConsumablesNames[i]);
            if (Item)
                AthenaConsumables.push_back(Item);
        }
        for (int i = 0; i < AssaultPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(AssaultPoolNames[i]);
            if (Item)
                AssaultPool.push_back(Item);
        }
        for (int i = 0; i < ShotgunPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(ShotgunPoolNames[i]);
            if (Item)
                ShotgunPool.push_back(Item);
        }
        for (int i = 0; i < SniperPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(SniperPoolNames[i]);
            if (Item)
                SniperPool.push_back(Item);
        }
        for (int i = 0; i < SMGPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(SMGPoolNames[i]);
            if (Item)
                SMGPool.push_back(Item);
        }
        for (int i = 0; i < PistolPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(PistolPoolNames[i]);
            if (Item)
                PistolPool.push_back(Item);
        }
        for (int i = 0; i < RocketPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(RocketPoolNames[i]);
            if (Item)
                RocketPool.push_back(Item);
        }
        for (int i = 0; i < AthenaLootPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortWeaponRangedItemDefinition>(AthenaLootPoolNames[i]);
            if (Item)
                AthenaLootPool.push_back(Item);
        }
        for (int i = 0; i < TrapsPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortTrapItemDefinition>(TrapsPoolNames[i]);
            if (Item)
                TrapsPool.push_back(Item);
        }
        for (int i = 0; i < ResourcePoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortResourceItemDefinition>(ResourcePoolNames[i]);
            if (Item)
                ResourcesPool.push_back(Item);
        }
        for (int i = 0; i < AmmoPoolNames.size(); i++)
        {
            auto Item = Utils::FindObjectFast<UFortAmmoItemDefinition>(AmmoPoolNames[i]);
            if (Item)
                AmmoPool.push_back(Item);
        }

        bInitialized = true;
        LOG_INFO("[Looting::Init] Initialized all loot pools.");
    }

    template <typename vec>
    void ShuffleVector(vec Vector) // doesnt work for some reason?
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(Vector.begin(), Vector.end(), std::default_random_engine(seed));
    }

    void ShuffleItems()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(AthenaConsumables.begin(), AthenaConsumables.end(), std::default_random_engine(seed));
        std::shuffle(AssaultPool.begin(), AssaultPool.end(), std::default_random_engine(seed));
        std::shuffle(ShotgunPool.begin(), ShotgunPool.end(), std::default_random_engine(seed));
        std::shuffle(SniperPool.begin(), SniperPool.end(), std::default_random_engine(seed));
        std::shuffle(SMGPool.begin(), SMGPool.end(), std::default_random_engine(seed));
        std::shuffle(PistolPool.begin(), PistolPool.end(), std::default_random_engine(seed));
        std::shuffle(RocketPool.begin(), RocketPool.end(), std::default_random_engine(seed));
        std::shuffle(AthenaLootPool.begin(), AthenaLootPool.end(), std::default_random_engine(seed));
        std::shuffle(TrapsPool.begin(), TrapsPool.end(), std::default_random_engine(seed));
        std::shuffle(ResourcesPool.begin(), ResourcesPool.end(), std::default_random_engine(seed));
        std::shuffle(AmmoPool.begin(), AmmoPool.end(), std::default_random_engine(seed));
    }

    auto GetRandomAmmoDefinition()
    {
        ShuffleItems();
        return AmmoPool[GetMathLibrary()->STATIC_RandomInteger(AmmoPool.size())];
    }

    auto GetRandomResource()
    {
        ShuffleItems();
        return ResourcesPool[GetMathLibrary()->STATIC_RandomInteger(ResourcesPool.size())];
    }

    auto GetRandomConsumable()
    {
        ShuffleItems();
        return AthenaConsumables[GetMathLibrary()->STATIC_RandomInteger(AthenaConsumables.size())];
    }

    auto GetLootType()
    {
        if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.43))
            return AssaultPool;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.22))
            return ShotgunPool;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.14))
            return SMGPool;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.11))
            return SniperPool;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.10))
            return PistolPool;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.07))
            return RocketPool;
        else
            return GetLootType();
    }

    auto GetWeapon()
    {
        ShuffleItems();
        auto WeaponType = GetLootType();
        EFortRarity RarityDeduced = EFortRarity::Handmade;

        if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.39))
            RarityDeduced = EFortRarity::Ordinary;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.35))
            RarityDeduced = EFortRarity::Sturdy;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.22))
            RarityDeduced = EFortRarity::Quality;
        else if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.15))
            RarityDeduced = EFortRarity::Fine;

        UFortWeaponRangedItemDefinition* Def = nullptr;

        for (int i = 0; i < WeaponType.size(); i++)
        {
            auto Weapon = WeaponType[GetMathLibrary()->STATIC_RandomInteger(WeaponType.size())];

            if (Weapon && Weapon->Rarity == RarityDeduced)
                Def = Weapon;
            else
                continue;
        }

        if (Def)
            return Def;
        else
            return GetWeapon();
    }

    auto GetHighWeapon()
    {
        ShuffleItems();
        auto WeaponType = GetLootType();
        EFortRarity RarityDeduced = EFortRarity::Fine;

        if (GetMathLibrary()->STATIC_RandomBoolWithWeight(0.50f)) // lazy 50/50 chance
            RarityDeduced = EFortRarity::Quality;

        UFortWeaponRangedItemDefinition* Def = nullptr;
        for (int i = 0; i < WeaponType.size(); i++)
        {
            auto Weapon = WeaponType[GetMathLibrary()->STATIC_RandomInteger(WeaponType.size())];

            if (Weapon && Weapon->Rarity == RarityDeduced)
                Def = Weapon;
            else
                continue;
        }

        if (Def)
            return Def;
        else
            return GetHighWeapon();
    }

    auto GetAmmoBoxLoot()
    {
        TArray<UFortAmmoItemDefinition*> AmmoArray;
        AmmoArray.Add(GetRandomAmmoDefinition());
        AmmoArray.Add(GetRandomAmmoDefinition());
        return AmmoArray;
    }

    auto GetChestLoot()
    {
        TArray<UFortWorldItemDefinition*> ItemsArray;
        auto Weapon = GetWeapon();
        ItemsArray.Add(Weapon);
        ItemsArray.Add(Weapon->GetAmmoWorldItemDefinition_BP());
        ItemsArray.Add(GetRandomResource());
        ItemsArray.Add(GetRandomConsumable());
        return ItemsArray;
    }

    auto GetSupplyDropLoot()
    {
        TArray<UFortWorldItemDefinition*> ItemsArray;
        auto Weapon = GetHighWeapon();
        ItemsArray.Add(Weapon);
        ItemsArray.Add(Weapon->GetAmmoWorldItemDefinition_BP());
        ItemsArray.Add(ResourcesPool[0]);
        ItemsArray.Add(ResourcesPool[1]); // i think i mistaked this for llama loot
        ItemsArray.Add(ResourcesPool[2]);
        ItemsArray.Add(GetRandomConsumable());
        ItemsArray.Add(GetRandomConsumable());
        return ItemsArray;
    }
}