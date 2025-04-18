#include <string>
#include <locale>
#include <codecvt>
#include "Global.h"


#pragma pack(push, 1)

struct Vector3 : UnityResolve::UnityType::Vector3{};

struct Quaternion : UnityResolve::UnityType::Quaternion{};


template<typename T, size_t N = 10>
struct _Items {
    char filler[0x20];
//    int32_t _count;
    T array[N];
};

template<typename T, size_t N = 10>
struct List {
    char filler[0x10];
    _Items<T, N>* _items;
};

struct Health {
    char filler[0x11C];
    float CurrentHealth;
};

struct ItemInstance {
    char filler[0x20];
    uint32_t _quantity;
};

struct Item {
    char filler[0x10];
    ItemInstance* _itemInstance;

    ItemInstance* GetItemInstance() {
        if (!this->_itemInstance) {
            printf("Item instance is null\n");
            return nullptr;
        }
        return this->_itemInstance;
    }
};

struct Player {
    char filler0[0x30];
    FishNet_Object_NetworkObject_o* _networkObjectCache;
    char filler1[0x178 - 0x30 - sizeof(void*)];
    Health* Health;
    char filler2[0x2a0 - 0x178 - sizeof(void*)];
    _Items<Item*, 10>* Inventory;

    Item* GetItem(int index = 0) {
        if (!this->Inventory) {
            printf("Player Inventory is null\n");
            return nullptr;
        }
        Item* item = this->Inventory->array[index];
        return item;
    }
};

struct PlayerList : List<Player*, 10> {
    static PlayerList* GetInstance() {
        return reinterpret_cast<PlayerList*>(getWithOffsets(
                UNITY_PLAYER + 0x01CF0AC8, {0xB8, 0x0, 0xD0, 0xE8, 0x58, 0x3C0 }));
    }

    Player* GetHost() {
        Player* player = this->_items->array[0];
        if (!player) {
            printf("Player is null\n");
            return nullptr;
        }
        return player;
    }
};

struct PlayerMovement {
    char filler[0x20];
    Player* Player;
    void* Controller;
    float sensitivity;
    float dead;
    bool canMove;
    bool canJump;
    bool SprintingRequiresStamina;
    bool _filler2;
    float MoveSpeedMultiplier;
    float SlipperyMovementMultiplier;
    float jumpForce;
    float gravityMultiplier; // 1.4
    void* groundDetectionMask;
    float slopeForce;
    float slopeForceRayLength;
    float crouchSpeedMultipler;
    float Crouched_VigIntensity;
    float Crouched_VigSmoothness;
    void* visibilityPointsToScale;
    void* originalVisibilityPointOffsets;
    float playerHeight_k__BackingField;
    UnityResolve::UnityType::Vector3 movement;

    static PlayerMovement* GetInstance() {
        return reinterpret_cast<PlayerMovement*>(getWithOffsets(
                GAME_ASSEMBLY_ADDRESS + 0x0380B9E0, {0xB8, 0x0, 0x0 }));
    }

    void SetJumpForce(float value = 5.25){
        jumpForce = value;
    }
    void SetMoveSpeedMultiplier(float value = 1){
        MoveSpeedMultiplier = value;
    }
};

struct Name {
    char filler[0x10];
    int _strLen;

    std::wstring GetName() {
        const wchar_t* str = reinterpret_cast<const wchar_t *>((char*)this + 0x14);
        return std::wstring(str, _strLen); // безопасный вариант
    }
};

struct Siren {
    char filler[0x24];
    float Volume;
};

struct PoliceLight {
    char filler[0x20];
    bool IsOn;
    char filler2[0x48 - 0x20 - sizeof(bool)];
    Siren* siren;
    char filler3[0x50 - 0x48 - sizeof(Siren*)];
    float CycleDuration;
    char filler4[0x88 - 0x50 - sizeof(float)];
    float LightBrightness;
};

struct syncVar___currentSteerAngle {
    char filler[0xA0];
    PoliceLight policeLight;
};

struct LandVehicle {
    char filler[0x120];
    Name* vehicleName;
    Name* vehicleCode;
    float vehiclePrice;
    bool isPlayerOwned;
    char filler2[0x190 - 0x120 - sizeof(Name*) - sizeof(Name*) - sizeof(float) - sizeof(bool)];
    _Items<void*, 10>* seats;
    char filler3[0x278 - 0x190 - sizeof(void*)];
    void* OccupantNPCs;
    char filler4[0x328 - 0x278 - sizeof(void*)];
    syncVar___currentSteerAngle* _syncVar___currentSteerAngle;

    void SetPoliceLight(bool value = 0) {
        if (!this->_syncVar___currentSteerAngle) {
            printf("syncVar___currentSteerAngle is null\n");
            return;
        }
        this->_syncVar___currentSteerAngle->policeLight.IsOn = value;
    }

    void SwitchPoliceLight() {
        if (!this->_syncVar___currentSteerAngle) {
            printf("syncVar___currentSteerAngle is null\n");
            return;
        }
        this->_syncVar___currentSteerAngle->policeLight.IsOn = !this->_syncVar___currentSteerAngle->policeLight.IsOn;
    }

    void OwnVehicle() {
        this->isPlayerOwned = true;
    }
};

struct AllVehicles : List<LandVehicle*, 10> {
    void PrintAllVehicles() {
        for (int i = 0; i < 60; i++) {
            LandVehicle* vehicle = this->_items->array[i];
            if (!vehicle) {
                printf("Vehicle is null\n");
                continue;
            }
            Name* name = vehicle->vehicleName;
            if (!name) {
                printf("Vehicle name is null\n");
                continue;
            }
            std::wstring vehicleName = name->GetName();
            wprintf(L"Vehicle name %ls\n", vehicleName.c_str());
        }
    }
};

struct VehicleManager {
    char filler[0x120];
    AllVehicles* AllVehicles;

    static VehicleManager* GetInstance() {
        return reinterpret_cast<VehicleManager*>(getWithOffsets(
                GAME_ASSEMBLY_ADDRESS + 0x038CFEA0, {0xB8, 0x0, 0x0 }));
    }
};

struct Officers : List<ScheduleOne_Police_PoliceOfficer_o*, 10> {
    static Officers* GetInstance() {
        return reinterpret_cast<Officers*>(getWithOffsets(
                GAME_ASSEMBLY_ADDRESS + 0x00379B8C0, {0xB8, 0x8, 0x0 }));
    }
};

#pragma pack(pop)

