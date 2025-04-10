#include <windows.h>
#include "Utils.h"
#include "HookManager.h"

typedef void(__attribute__((__fastcall__)) *tBeginBodySearch_Networked)(PoliceOfficer* thisPtr, void* target);


HOOK_METHOD("PlayerMovement", SetCrouched, void, __fastcall, PlayerMovement* thisPtr, bool c)
{
    oSetCrouched(thisPtr, c);
}

HOOK_METHOD("PoliceOfficer", ConductBodySearch, void, __fastcall, PoliceOfficer* thisPtr, Player* p)
{
    printf("ConductBodySearch %p\n", thisPtr);
//    void* method = *(void**)((uintptr_t)thisPtr + 0x88 * 8);
//    void* vtable = thisPtr->vtable;
//    void* method = vtable[0x88];
//    printf("ConductBodySearch method %p\n", method);
    return oConductBodySearch(thisPtr, p);
}

HOOK_METHOD_BY_OFFSET(0x7C90D0, set_headLightsOn, void, __fastcall, void* thisPtr, bool value)
{
//    printf("set_headLightsOn %p\n", thisPtr);
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

HOOK_METHOD("moneyManager", ChangeCashBalance, void, __thiscall, uintptr_t moneyManager, float change, bool visualizeChange, bool playCashSound)
{
    printf("New money %f\n", change);
    printf("moneyManager %p\n", moneyManager);
    return oChangeCashBalance(moneyManager, change, visualizeChange, playCashSound);
}

HOOK_METHOD("LandVehicle", Hovered, void, __fastcall, LandVehicle* thisPtr)
{
    if (myCar != thisPtr) {
        printf("Car set %p\n", thisPtr);
        myCar = thisPtr;
        myCar->OwnVehicle();
    }
    return oHovered(thisPtr);
}

HOOK_METHOD("LandVehicle", RemoveNPCOccupant, void, __thiscall, LandVehicle* thisPtr, void* npc)
{
    printf("RemoveNPCOccupant %p %p\n", thisPtr, npc);
    oRemoveNPCOccupant(thisPtr, npc);
}

HOOK_METHOD("LandVehicle", set_handbrakeApplied, void, __thiscall, LandVehicle* thisPtr, bool value)
{
    oset_handbrakeApplied(thisPtr, value);
}


HOOK_METHOD("LandVehicle", ExitVehicle, void, __thiscall, LandVehicle* thisPtr)
{
    oExitVehicle(thisPtr);
}

HOOK_METHOD("LandVehicle", SetTransform, void, __fastcall, LandVehicle* thisPtr, Vector3 pos, Quaternion rot)
{
    printf("SetTransform %p\n", thisPtr);
    oSetTransform(thisPtr, pos, rot);
}

HOOK_METHOD("VehicleManager", SpawnVehicle, void, __thiscall, void* thisPtr, const char* vehicleCode, Vector3 position, Quaternion rotation, bool playerOwned)
{
    printf("Spawn vehicle %s\n", vehicleCode);
    oSpawnVehicle(thisPtr, vehicleCode, position, rotation, playerOwned);
}

//HOOK_METHOD_BY_OFFSET(0x6C11C0, SetCrouched, void, __fastcall, PlayerMovement* thisPtr, bool c)
//{
////    Vector3 vehicleVector = Vector3{0, 10, 0};
////    Quaternion vehicleQuat = Quaternion{1,0,0,0};
////    hkSetTransform(myCar, vehicleVector, vehicleQuat);
////
////    printf("addr occupant %p\n", &myCar->OccupantNPCs);
////    void* occupant = (void*)0x0000014d1a578540;
////    void* occupant = myCar->OccupantNPCs;
////    hkRemoveNPCOccupant(myCar, occupant);
//    oSetCrouched(thisPtr, c);
////    hkSpawnVehicle(vehicleManager, "Veeper", vehicleVector, vehicleQuat, true);
//}

