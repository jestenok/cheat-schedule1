#include <windows.h>
#include <iostream>

uintptr_t base = (uintptr_t)GetModuleHandle(nullptr);

auto baseAssemblyName = "GameAssembly.dll";
uintptr_t baseAssembly = (uintptr_t)GetModuleHandle(baseAssemblyName);

auto unityPlayerName = "UnityPlayer.dll";
uintptr_t unityPlayer = (uintptr_t)GetModuleHandle(unityPlayerName);