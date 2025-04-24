#include <windows.h>
#include <iostream>
#include "UnityResolve.hpp"
#include "il2cpp.h"
#include "Utils.h"


uintptr_t base = (uintptr_t)GetModuleHandleA(nullptr);

const char* ASSEMBLY_CSHARP_NAME = "Assembly-CSharp.dll";
auto ASSEMBLY_CSHARP = GetModuleHandleA(ASSEMBLY_CSHARP_NAME);
uintptr_t ASSEMBLY_CSHARP_ADDRESS = (uintptr_t)ASSEMBLY_CSHARP;

auto GAME_ASSEMBLY_NAME = "GameAssembly.dll";
auto GAME_ASSEMBLY = GetModuleHandleA(GAME_ASSEMBLY_NAME);
uintptr_t GAME_ASSEMBLY_ADDRESS = (uintptr_t)GAME_ASSEMBLY;
UnityResolve::Assembly *assembly;

auto UNITY_PLAYER_NAME = "UnityPlayer.dll";
uintptr_t UNITY_PLAYER = (uintptr_t)GetModuleHandleA(UNITY_PLAYER_NAME);

auto GAME_OVERLAY_RENDER_NAME = "GameOverlayRender64.dll";
auto GAME_OVERLAY_RENDER = GetModuleHandleA(GAME_OVERLAY_RENDER_NAME);
uintptr_t GAME_OVERLAY_RENDER_ADDRESS = (uintptr_t)GAME_ASSEMBLY;