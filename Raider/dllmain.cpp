#pragma once

#include "SDK.hpp"
#include "game.h"
#include "hooks.h"
#include "ufunctionhooks.h"
#include "logger.h"

DWORD WINAPI Main(LPVOID lpParam)
{

    //SetupConsole();
    AllocConsole();

    raider::utils::Logger::Initialize();

    LOG_INFO("[LogRaider] Welcome to Raider!");

    Native::InitializeAll();

    LOG_INFO("[Native::InitializeAll] Initialized hooks.");

    UFunctionHooks::Initialize();

    DETOUR_START
    DetourAttachE(Native::NetDriver::TickFlush, Hooks::TickFlush);
    DetourAttachE(Native::LocalPlayer::SpawnPlayActor, Hooks::LocalPlayerSpawnPlayActor);

    auto Address = Utils::FindPattern(Patterns::NetDebug);
    CheckNullFatal(Address, "Failed to find NetDebug");
    void* (*NetDebug)(void* _this) = nullptr;
    AddressToFunction(Address, NetDebug);
    
    DetourAttachE(NetDebug, Hooks::NetDebug);
    DetourAttachE(ProcessEvent, Hooks::ProcessEventHook);
    DETOUR_END

    LOG_INFO("[LogRaider] Hooked ProcessEvent");

    LOG_INFO("[LogRaider] Base Address: {:X}", Offsets::Imagebase);

    CreateConsole();
	
    return 1;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, Main, hModule, 0, 0);

    return true;
}
