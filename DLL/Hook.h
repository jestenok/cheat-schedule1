#include <windows.h>
#include "Utils.h"
#include "HookManager.h"


HOOK_METHOD(0x931F20, ChangeCashBalance, void, __fastcall, void* moneyManager, float change, bool visualizeChange, bool playCashSound)
{
    printf("New money %f\n", change);
    return oChangeCashBalance(moneyManager, change, visualizeChange, playCashSound);
}


HOOK_METHOD(0x833420, ChangeBalance, void, __fastcall, void* thisPtr, float amount)
{
    printf("New balance %f\n", amount);
//    amount = 50000;
    oChangeBalance(thisPtr, amount);
}

//// ScheduleOne.PlayerScripts.Player
//// Token: 0x06002525 RID: 9509 RVA: 0x00002050 File Offset: 0x00000250
//[Token(Token = "0x6002525")]
//[Address(RVA = "0x67F6C0", Offset = "0x67E6C0", VA = "0x18067F6C0")]
//public static Player GetPlayer(string playerCode)
//{
//    return null;
//}

//int i = 0;
//HOOK_METHOD(0x67F6C0, GetPlayer, void*, __fastcall, const char* playerCode)
//{
//    if (i <= 10) {
//        printf("GetPlayer %s\n", playerCode);
//        i++;
//    }
//    return oGetPlayer(playerCode);
//}


HOOK_METHOD(0x6BE7F0, Jump, void*, __fastcall, void* thisPtr)
{
    printf("Jump %p\n", thisPtr);
    return oJump(thisPtr);
}