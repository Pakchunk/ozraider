#pragma once

#include "inireader.h"

static void Error(std::string error, bool bExit = false)
{
    MessageBoxA(nullptr, error.c_str(), "Error", MB_OK | MB_ICONERROR);

    if (bExit)
        exit(0);
}

#define CheckNullFatal(x, y) \
    if (!x)                  \
    {                        \
        Error(y, true);      \
    }

template <typename T>
auto Merge(T v)
{
    return v;
}

template <typename T, typename... Types>
auto Merge(T var1, Types... vars)
{
    return reinterpret_cast<T*>(__int64(var1) + __int64(Merge(vars...)));
}

// Util macros
#define EXECUTE_ONE_TIME if (([] { 		    \
    static bool is_first_time = true;	    \
    auto was_first_time = is_first_time;    \
    is_first_time = false; 					\
    return was_first_time; }()))

// Hooking macros
#define DETOUR_START          \
    DetourTransactionBegin(); \
    DetourUpdateThread(GetCurrentThread());

#define DetourAttachE(Target, Detour) DetourAttach(&(void*&)Target, Detour);

#define DetourDetachE(Target, Detour) DetourDetach(reinterpret_cast<void**>(&Target), Detour);

#define DETOUR_END DetourTransactionCommit();

#define AddressToFunction(a, f) f = reinterpret_cast<decltype(f)>(a)

inline void SetupConsole()
{
    AllocConsole();

    FILE* pFile;
    freopen_s(&pFile, "CONOUT$", "w", stdout);
}

namespace Utils
{
    static __forceinline uintptr_t FindPattern(const char* signature, bool bRelative = false, uint32_t offset = 0)
    {
        uintptr_t base_address = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
        static auto patternToByte = [](const char* pattern)
        {
            auto bytes = std::vector<int> {};
            const auto start = const_cast<char*>(pattern);
            const auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current)
            {
                if (*current == '?')
                {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else
                {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        const auto dosHeader = (PIMAGE_DOS_HEADER)base_address;
        const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)base_address + dosHeader->e_lfanew);

        const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = patternToByte(signature);
        const auto scanBytes = reinterpret_cast<std::uint8_t*>(base_address);

        const auto s = patternBytes.size();
        const auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i)
        {
            bool found = true;
            for (auto j = 0ul; j < s; ++j)
            {
                if (scanBytes[i + j] != d[j] && d[j] != -1)
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                uintptr_t address = reinterpret_cast<uintptr_t>(&scanBytes[i]);
                if (bRelative)
                {
                    address = ((address + offset + 4) + *(int32_t*)(address + offset));
                    return address;
                }
                return address;
            }
        }
        return NULL;
    }

    static float FRand()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    template <typename T>
    static T Max(T f, T s)
    {
        return f > s ? f : s;
    }

    template <typename T>
    static T* FindObjectFast(std::string ObjectName, UClass* Class = UObject::StaticClass())
    {
        auto OrigInName = std::wstring(ObjectName.begin(), ObjectName.end()).c_str();

        static auto StaticFindObjectAddr = FindPattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC 60 80 3D ? ? ? ? ? 45 0F B6 F1");
        auto StaticFindObject = (T * (*)(UClass*, UObject * Package, const wchar_t* OrigInName, bool ExactClass))(StaticFindObjectAddr);
        return StaticFindObject(Class, nullptr, OrigInName, false);
    }

    inline auto Ini()
    {
        INIReader reader("RaiderConfig.ini");
        return reader;
    }

    static auto DelimiterParse(const std::string& str, const std::string& delimiter)
    {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = str.find(delimiter, pos_start)) != -1)
        {
            token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(str.substr(pos_start));
        return res;
    }
};
