#include <string>
#include <locale>
#include <codecvt>
#include "Global.h"


#pragma pack(push, 1)
typedef void(*Il2CppMethodPointer)();

struct MethodInfo;

struct VirtualInvokeData
{
    Il2CppMethodPointer methodPtr;
    const MethodInfo* method;
};

struct Il2CppType
{
    void* data;
    unsigned int bits;
};

struct Il2CppClass;

struct Il2CppObject
{
    Il2CppClass *klass;
    void *monitor;
};

union Il2CppRGCTXData
{
    void* rgctxDataDummy;
    const MethodInfo* method;
    const Il2CppType* type;
    Il2CppClass* klass;
};

struct Il2CppRuntimeInterfaceOffsetPair
{
    Il2CppClass* interfaceType;
    int32_t offset;
};
struct Il2CppClass_1
{
    void* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    Il2CppClass* element_class;
    Il2CppClass* castClass;
    Il2CppClass* declaringType;
    Il2CppClass* parent;
    void *generic_class;
    void* typeMetadataHandle;
    void* interopData;
    Il2CppClass* klass;
    void* fields;
    void* events;
    void* properties;
    void* methods;
    Il2CppClass** nestedTypes;
    Il2CppClass** implementedInterfaces;
    Il2CppRuntimeInterfaceOffsetPair* interfaceOffsets;
};

struct Il2CppClass_2
{
    Il2CppClass** typeHierarchy;
    void *unity_user_data;
    uint32_t initializationExceptionGCHandle;
    uint32_t cctor_started;
    uint32_t cctor_finished;
    size_t cctor_thread;
    void* genericContainerHandle;
    uint32_t instance_size;
    uint32_t actualSize;
    uint32_t element_size;
    int32_t native_size;
    uint32_t static_fields_size;
    uint32_t thread_static_fields_size;
    int32_t thread_static_fields_offset;
    uint32_t flags;
    uint32_t token;
    uint16_t method_count;
    uint16_t property_count;
    uint16_t field_count;
    uint16_t event_count;
    uint16_t nested_type_count;
    uint16_t vtable_count;
    uint16_t interfaces_count;
    uint16_t interface_offsets_count;
    uint8_t typeHierarchyDepth;
    uint8_t genericRecursionDepth;
    uint8_t rank;
    uint8_t minimumAlignment;
    uint8_t naturalAligment;
    uint8_t packingSize;
    uint8_t bitflags1;
    uint8_t bitflags2;
};

struct Il2CppClass
{
    Il2CppClass_1 _1;
    void* static_fields;
    Il2CppRGCTXData* rgctx_data;
    Il2CppClass_2 _2;
    VirtualInvokeData vtable[255];
};

template <typename T = VirtualInvokeData>
struct Il2CppClass_t : Il2CppClass
{
    T vtable[255];
};

template <typename T = VirtualInvokeData>
struct Il2CppObject_t : Il2CppObject
{
    Il2CppClass_t<T> *klass;
};

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
    void* _networkObjectCache;
    char filler1[0x178 - 0x30 - sizeof(void*)];
    Health* Health;
    char filler2[0x2a0 - 0x178 - sizeof(void*)];
    _Items<Item*, 10>* Inventory;

    Item* GetItem() {
        if (!this->Inventory) {
            printf("Player Inventory is null\n");
            return nullptr;
        }
        Item* item = this->Inventory->array[0];
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


struct ScheduleOne_Police_PoliceOfficer_VTable {
    VirtualInvokeData _0_Equals;
    VirtualInvokeData _1_SetGUID;
    VirtualInvokeData _2_GetHashCode;
    VirtualInvokeData _3_ToString;
    VirtualInvokeData _4_OnStartNetwork;
    VirtualInvokeData _5_OnStopNetwork;
    VirtualInvokeData _6_OnStartServer;
    VirtualInvokeData _7_OnStopServer;
    VirtualInvokeData _8_OnOwnershipServer;
    VirtualInvokeData _9_OnSpawnServer;
    VirtualInvokeData _10_OnDespawnServer;
    VirtualInvokeData _11_OnStartClient;
    VirtualInvokeData _12_Save;
    VirtualInvokeData _13_WriteBaseData;
    VirtualInvokeData _14_GetLocalPath;
    VirtualInvokeData _15_CompleteSave;
    VirtualInvokeData _16_OnValidate;
    VirtualInvokeData _17_DeleteUnapprovedFiles;
    VirtualInvokeData _18_GetContainerFolder;
    VirtualInvokeData _19_WriteSubfile;
    VirtualInvokeData _20_WriteFolder;
    VirtualInvokeData _21_TryLoadFile;
    VirtualInvokeData _22_TryLoadFile;
    VirtualInvokeData _23_get_SaveFileName;
    VirtualInvokeData _24_get_Loader;
    VirtualInvokeData _25_get_ShouldSaveUnderFolder;
    VirtualInvokeData _26_get_LocalExtraFiles;
    VirtualInvokeData _27_set_LocalExtraFiles;
    VirtualInvokeData _28_get_LocalExtraFolders;
    VirtualInvokeData _29_set_LocalExtraFolders;
    VirtualInvokeData _30_get_HasChanged;
    VirtualInvokeData _31_set_HasChanged;
    VirtualInvokeData _32_unknown;
    VirtualInvokeData _33_unknown;
    VirtualInvokeData _34_unknown;
    VirtualInvokeData _35_unknown;
    VirtualInvokeData _36_unknown;
    VirtualInvokeData _37_unknown;
    VirtualInvokeData _38_unknown;
    VirtualInvokeData _39_unknown;
    VirtualInvokeData _40_unknown;
    VirtualInvokeData _41_unknown;
    VirtualInvokeData _42_unknown;
    VirtualInvokeData _43_unknown;
    VirtualInvokeData _44_unknown;
    VirtualInvokeData _45_unknown;
    VirtualInvokeData _46_unknown;
    VirtualInvokeData _47_Awake;
    VirtualInvokeData _48_InitializeSaveable;
    VirtualInvokeData _49_CreateMessageConversation;
    VirtualInvokeData _50_Start;
    VirtualInvokeData _51_OnDestroy;
    VirtualInvokeData _52_MinPass;
    VirtualInvokeData _53_Update;
    VirtualInvokeData _54_SetVisible;
    VirtualInvokeData _55_ApplyScale;
    VirtualInvokeData _56_AimedAtByPlayer;
    VirtualInvokeData _57_SendImpact;
    VirtualInvokeData _58_ReceiveImpact;
    VirtualInvokeData _59_ProcessImpactForce;
    VirtualInvokeData _60_EnterVehicle;
    VirtualInvokeData _61_ExitVehicle;
    VirtualInvokeData _62_Hovered;
    VirtualInvokeData _63_Interacted;
    VirtualInvokeData _64_ShouldNoticeGeneralCrime;
    VirtualInvokeData _65_SetUnsettled_30s;
    VirtualInvokeData _66_GetNameAddress;
    VirtualInvokeData _67_ShowOutline;
    VirtualInvokeData _68_HideOutline;
    VirtualInvokeData _69_ShouldSave;
    VirtualInvokeData _70_ShouldSaveRelationshipData;
    VirtualInvokeData _71_ShouldSaveMessages;
    VirtualInvokeData _72_ShouldSaveInventory;
    VirtualInvokeData _73_ShouldSaveHealth;
    VirtualInvokeData _74_GetSaveString;
    VirtualInvokeData _75_WriteData;
    VirtualInvokeData _76_Load;
    VirtualInvokeData _77_NetworkInitialize___Early;
    VirtualInvokeData _78_NetworkInitialize__Late;
    VirtualInvokeData _79_RpcLogic___AimedAtByPlayer_3323014238;
    VirtualInvokeData _80_RpcLogic___SendImpact_427288424;
    VirtualInvokeData _81_RpcLogic___ReceiveImpact_427288424;
    VirtualInvokeData _82_RpcLogic___EnterVehicle_3321926803;
    VirtualInvokeData _83_RpcLogic___ExitVehicle_2166136261;
    VirtualInvokeData _84_ReadSyncVar___ScheduleOne_NPCs_NPC;
    VirtualInvokeData _85_Awake_UserLogic_ScheduleOne_NPCs_NPC_Assembly_CSharp_dll;
    VirtualInvokeData _86_BeginFootPursuit_Networked;
    VirtualInvokeData _87_BeginVehiclePursuit_Networked;
    VirtualInvokeData _88_BeginBodySearch_Networked;
    VirtualInvokeData _89_AssignToCheckpoint;
    VirtualInvokeData _90_AssignToSentryLocation;
    VirtualInvokeData _91_UpdateBodySearch;
    VirtualInvokeData _92_RpcLogic___BeginFootPursuit_Networked_419679943;
    VirtualInvokeData _93_RpcLogic___BeginVehiclePursuit_Networked_2261819652;
    VirtualInvokeData _94_RpcLogic___BeginBodySearch_Networked_3323014238;
    VirtualInvokeData _95_RpcLogic___AssignToCheckpoint_4087078542;
    VirtualInvokeData _96_ReadSyncVar___ScheduleOne_Police_PoliceOfficer;
    VirtualInvokeData _97_Awake_UserLogic_ScheduleOne_Police_PoliceOfficer_Assembly_CSharp_dll;
};

struct PoliceOfficer : Il2CppObject_t<ScheduleOne_Police_PoliceOfficer_VTable> {
};

struct Officers : List<PoliceOfficer*, 10> {
    static Officers* GetInstance() {
        return reinterpret_cast<Officers*>(getWithOffsets(
                GAME_ASSEMBLY_ADDRESS + 0x00379B8C0, {0xB8, 0x8, 0x0 }));
    }
};

#pragma pack(pop)

