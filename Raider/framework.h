#pragma once
#include "gui.h"
#include "ue4.h"
#include "game.h"


std::vector<std::string> AthenaConsumables = {
	"/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields",
	"/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall",
	"/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff",
	"/Game/Athena/Items/Consumables/SuperMedkit/Athena_SuperMedkit.Athena_SuperMedkit",
	"/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade",
	"/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade",
	"/Game/Athena/Items/Consumables/SmokeGrenade/Athena_SmokeGrenade.Athena_SmokeGrenade",
	"/Game/Athena/Items/Consumables/StickyGrenade/Athena_StickyGrenade.Athena_StickyGrenade",
	"/Game/Athena/Items/Consumables/Bush/Athena_Bush.Athena_Bush",
	"/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit",
	"/Game/Athena/Items/Consumables/DanceGrenade/Athena_DanceGrenade.Athena_DanceGrenade",
	"/Game/Athena/Items/Consumables/SmokeGrenade/Athena_SmokeGrenade.Athena_SmokeGrenade"
};

std::vector<std::string> AthenaLootPool = {
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
	"/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03"
};

std::vector<std::string> TrapsPool = {
	"/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena",
	"/Game/Athena/Items/Traps/TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03"
};

std::vector<std::string> ResourcePool = {
	"/Game/Items/ResourcePickups/WoodItemData.WoodItemData",
	"/Game/Items/ResourcePickups/StoneItemData.StoneItemData",
	"/Game/Items/ResourcePickups/MetalItemData.MetalItemData"
};

std::vector<std::string> AmmoPool = {
	"/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets",
	"/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells",
	"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium",
	"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight",
	"/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"
};

std::vector<std::string> PickaxePool = {
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Anchor_Athena.WID_Harvest_Pickaxe_Anchor_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_ArtDeco.WID_Harvest_Pickaxe_ArtDeco",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Assassin.WID_Harvest_Pickaxe_Assassin",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_BoltOn_Athena_C_T01.WID_Harvest_Pickaxe_BoltOn_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Brite.WID_Harvest_Pickaxe_Brite",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Carrot.WID_Harvest_Pickaxe_Carrot",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_CircuitBreaker.WID_Harvest_Pickaxe_CircuitBreaker",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_CuChulainn.WID_Harvest_Pickaxe_CuChulainn",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01.WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Disco_Athena.WID_Harvest_Pickaxe_Disco_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_District_Athena.WID_Harvest_Pickaxe_District_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Dragon.WID_Harvest_Pickaxe_Dragon",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Flamingo_Athena_C_T01.WID_Harvest_Pickaxe_Flamingo_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Heart_Athena.WID_Harvest_Pickaxe_Heart_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_HolidayCandyCane_Athena.WID_Harvest_Pickaxe_HolidayCandyCane_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_HolidayGiftWrap_Athena.WID_Harvest_Pickaxe_HolidayGiftWrap_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_IcePick_Athena_C_T01.WID_Harvest_Pickaxe_IcePick_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Keg_Athena.WID_Harvest_Pickaxe_Keg_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01.WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Medieval_Athena.WID_Harvest_Pickaxe_Medieval_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Megalodon_Athena.WID_Harvest_Pickaxe_Megalodon_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_PajamaParty.WID_Harvest_Pickaxe_PajamaParty",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Pizza.WID_Harvest_Pickaxe_Pizza",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Plunger.WID_Harvest_Pickaxe_Plunger",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_PotOfGold.WID_Harvest_Pickaxe_PotOfGold",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Prismatic.WID_Harvest_Pickaxe_Prismatic",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_RockerPunk.WID_Harvest_Pickaxe_RockerPunk",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Scavenger.WID_Harvest_Pickaxe_Scavenger",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Shark_Athena.WID_Harvest_Pickaxe_Shark_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_SickleBat_Athena_C_T01.WID_Harvest_Pickaxe_SickleBat_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_SkiBoot.WID_Harvest_Pickaxe_SkiBoot",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Smiley_Athena_C_T01.WID_Harvest_Pickaxe_Smiley_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Space.WID_Harvest_Pickaxe_Space",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Spikey_Athena_C_T01.WID_Harvest_Pickaxe_Spikey_Athena_C_T01",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Squeak.WID_Harvest_Pickaxe_Squeak",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Stealth.WID_Harvest_Pickaxe_Stealth",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Tactical.WID_Harvest_Pickaxe_Tactical",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_TacticalBlack.WID_Harvest_Pickaxe_TacticalBlack",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_TacticalUrban.WID_Harvest_Pickaxe_TacticalUrban",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Teslacoil_Athena.WID_Harvest_Pickaxe_Teslacoil_Athena",
	"/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_WinterCamo_Athena.WID_Harvest_Pickaxe_WinterCamo_Athena"

};