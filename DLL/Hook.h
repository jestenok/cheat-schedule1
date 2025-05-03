#include <windows.h>
#include "HookManager.h"

typedef void(__attribute__((__fastcall__)) *AssignTarget)(ScheduleOne_NPCs_Behaviour_BodySearchBehaviour_o*, void*, FishNet_Object_NetworkObject_o*, const MethodInfo*);
typedef void(__attribute__((__fastcall__)) *Enable)(ScheduleOne_NPCs_Behaviour_BodySearchBehaviour_o*, const MethodInfo*);
typedef void(__attribute__((__fastcall__)) *tRpcLogic___BeginBodySearch_Networked_3323014238)(ScheduleOne_Police_PoliceOfficer_o* thisPtr, FishNet_Object_NetworkObject_o* target);
typedef void(__attribute__((__fastcall__)) *RpcWriter___Server_BeginFootPursuit_Networked_419679943)(ScheduleOne_Police_PoliceOfficer_o* thisPtr, FishNet_Object_NetworkObject_o* target, bool includeColleagues);


HOOK_METHOD(PoliceOfficer, StartBodySearchInvestigation, void, __fastcall, ScheduleOne_Police_PoliceOfficer_o* thisPtr, Player* p)
{
    printf("StartBodySearchInvestigation %p\n", thisPtr);
    return oPoliceOfficer_StartBodySearchInvestigation(thisPtr, p);
}

HOOK_METHOD(PoliceOfficer, Update, void, __fastcall, ScheduleOne_Police_PoliceOfficer_o* thisPtr)
{
    if (!officer) {
        officer = thisPtr;
        printf("Officer set %p\n", thisPtr);
        resolverHookManager.RemoveHook(aPoliceOfficer_Update);
    }
    printf("Officer Update %p\n", thisPtr);
    return oPoliceOfficer_Update(thisPtr);
}

HOOK_METHOD(PlayerMovement, Update, void, __fastcall, PlayerMovement* thisPtr)
{
    if (!playerMovement) {
        playerMovement = thisPtr;
        printf("PlayerMovement set %p\n", thisPtr);
        resolverHookManager.RemoveHook(aPlayerMovement_Update);
    }
    printf("PlayerMovement Update %p\n", thisPtr);
    return oPlayerMovement_Update(thisPtr);
}

HOOK_METHOD(PlayerCrimeData, SetPursuitLevel, void, __fastcall, ScheduleOne_PlayerScripts_PlayerCrimeData_o* thisPtr, int32_t lvl, MethodInfo* method)
{
    printf("SetPursuitLevel %p %d\n", thisPtr, lvl);
    return oPlayerCrimeData_SetPursuitLevel(thisPtr, 0, method);
}

HOOK_METHOD(BodySearchBehaviour, UpdateSearch, void, __fastcall, ScheduleOne_NPCs_Behaviour_BodySearchBehaviour_o* thisPtr, MethodInfo* method)
{
    return oBodySearchBehaviour_UpdateSearch(thisPtr, method);
}

HOOK_METHOD(PlayerMovement, Teleport, void, __fastcall, PlayerMovement* thisPtr, Vector3 pos)
{
    oPlayerMovement_Teleport(thisPtr, pos);
}

HOOK_METHOD(PlayerMovement, SetCrouched, void, __fastcall, PlayerMovement* thisPtr, bool c)
{
    oPlayerMovement_SetCrouched(thisPtr, c);
}

HOOK_METHOD(PoliceOfficer, ConductBodySearch, void, __fastcall, ScheduleOne_Police_PoliceOfficer_o* thisPtr, Player* p)
{
    return oPoliceOfficer_ConductBodySearch(thisPtr, p);
}

HOOK_METHOD_BY_OFFSET(0x7C90D0, set_headLightsOn, void, __fastcall, void* thisPtr, bool value)
{
    return oset_headLightsOn(thisPtr, 0);
}

HOOK_METHOD_BY_OFFSET(0x733330, StaticDoorHovered, void, __thiscall, void* thisPtr)
{
    printf("CanPlayerAccess1 %p\n", thisPtr);
    return oStaticDoorHovered(thisPtr);
}


HOOK_METHOD_BY_OFFSET(0x732780, c1, void, __thiscall, void* thisPtr)
{
    printf("CanPlayerAccess2 %p\n", thisPtr);
    return oc1(thisPtr);
}

HOOK_METHOD_BY_OFFSET(0x732990, c, void, __thiscall, void* thisPtr)
{
    printf("CanPlayerAccess3 %p\n", thisPtr);
    return oc(thisPtr);
}

HOOK_METHOD_BY_OFFSET(0x732BB0, q, void, __thiscall, void* thisPtr)
{
    printf("CanPlayerAccess4 %p\n", thisPtr);
    return oq(thisPtr);
}

HOOK_METHOD(MoneyManager, ChangeCashBalance, void, __thiscall, uintptr_t moneyManager, float change, bool visualizeChange, bool playCashSound)
{
    printf("New money %f\n", change);
    printf("moneyManager %p\n", moneyManager);
    return oMoneyManager_ChangeCashBalance(moneyManager, change, visualizeChange, playCashSound);
}

HOOK_METHOD(LandVehicle, Hovered, void, __fastcall, LandVehicle* thisPtr)
{
    if (myCar != thisPtr) {
        printf("Car set %p\n", thisPtr);
        myCar = thisPtr;
        myCar->OwnVehicle();
    }
    return oLandVehicle_Hovered(thisPtr);
}

HOOK_METHOD(LandVehicle, RemoveNPCOccupant, void, __thiscall, LandVehicle* thisPtr, void* npc)
{
    printf("RemoveNPCOccupant %p %p\n", thisPtr, npc);
    oLandVehicle_RemoveNPCOccupant(thisPtr, npc);
}

HOOK_METHOD(LandVehicle, set_handbrakeApplied, void, __thiscall, LandVehicle* thisPtr, bool value)
{
    oLandVehicle_set_handbrakeApplied(thisPtr, value);
}


HOOK_METHOD(LandVehicle, ExitVehicle, void, __thiscall, LandVehicle* thisPtr)
{
    oLandVehicle_ExitVehicle(thisPtr);
}

HOOK_METHOD(LandVehicle, SetTransform, void, __fastcall, LandVehicle* thisPtr, Vector3 pos, Quaternion rot)
{
    printf("SetTransform %p\n", thisPtr);
    oLandVehicle_SetTransform(thisPtr, pos, rot);
}

HOOK_METHOD(VehicleManager, SpawnVehicle, void, __thiscall, void* thisPtr, const char* vehicleCode, Vector3 position, Quaternion rotation, bool playerOwned)
{
    printf("Spawn vehicle %s\n", vehicleCode);
    oVehicleManager_SpawnVehicle(thisPtr, vehicleCode, position, rotation, playerOwned);
}
