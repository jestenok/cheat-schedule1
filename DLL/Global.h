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

auto GAME_OVERLAY_RENDERER_NAME = "GameOverlayRenderer64.dll";
auto GAME_OVERLAY_RENDERER = GetModuleHandleA(GAME_OVERLAY_RENDERER_NAME);
uintptr_t GAME_OVERLAY_ADDRESS = (uintptr_t)GAME_OVERLAY_RENDERER;
auto GAME_OVERLAY_RENDER_PATTERN = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8";
void* GAME_OVERLAY_RENDERER_ADDRESS = nullptr;


auto DX_MODULE_NAME = "d3d11.dll";
auto DX_MODULE = GetModuleHandleA(DX_MODULE_NAME);
uintptr_t DX_MODULE_ADDRESS = (uintptr_t)DX_MODULE;