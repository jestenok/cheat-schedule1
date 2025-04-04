#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include "Hook.h"

#pragma pack(push, 1)

#define BEGIN_STRUCT(name) struct name { \
    constexpr static size_t __BASE_OFFSET = 0x0; \
    constexpr static size_t __CURRENT_OFFSET = __BASE_OFFSET;

#define FIELD(type, name, offset, prev_offset) \
    static_assert(offset >= prev_offset, "Offsets must be in ascending order"); \
    char filler_##__LINE__[(offset) - prev_offset]; \
    type name;

#define END_STRUCT };

BEGIN_STRUCT(Health)
    FIELD(float, CurrentHealth, 0x11C, __CURRENT_OFFSET)
END_STRUCT

BEGIN_STRUCT(Player)
    FIELD(Health*, Health, 0x178, __CURRENT_OFFSET)
END_STRUCT

template<typename T>
BEGIN_STRUCT(List)
    FIELD(T, array[10], 0x20, __CURRENT_OFFSET)
END_STRUCT

BEGIN_STRUCT(PlayerList)
    FIELD(List<Player*>*, _items, 0x10, __CURRENT_OFFSET)
END_STRUCT


//struct PlayerList {
//    char filler_0[0x10];
//    void* _items[10];
//};


#pragma pack(pop)

void CheatLogic() {
//    void* addr = oGetPlayer("Local");
//    printf("GetPlayer %p\n", addr);

    std::vector<uintptr_t> offsets = { 0xB8, 0x0, 0xD0, 0xE8, 0x58, 0x3C0 };
    PlayerList* aPlayerList = reinterpret_cast<PlayerList *>(getWithOffsets(unityPlayer + 0x01CF0AC8, offsets));
//    PlayerList oPlayerList = *aPlayerList;
    printf("PlayerList %p\n", aPlayerList);
    printf("PlayerList items %p\n", aPlayerList->_items);
    printf("PlayerList items %p\n", aPlayerList->_items->array[0]);
    Player* player = aPlayerList->_items->array[0];
    printf("Player %p\n", player);
    printf("Player Health %p\n", player->Health);
    printf("Player Health CurrentHealth %f\n", player->Health->CurrentHealth);
//    player->Health->CurrentHealth = -1.0f;
}