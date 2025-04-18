#include "DataStructures.h"


LandVehicle* myCar;
VehicleManager*  vehicleManager;
PlayerMovement* playerMovement;
PlayerList* playerList;
Officers* officers;
ScheduleOne_Police_PoliceOfficer_o* officer;
Player* player;
UnityResolve::UnityType::Camera* mainCamera;
UnityResolve::UnityType::Vector2 screenCenter;

float screenX;
float screenY;


void InitAddresses()
{
    vehicleManager = VehicleManager::GetInstance();
    if (!vehicleManager) {
        printf("VM not found\n");
        return;
    }

    playerMovement = PlayerMovement::GetInstance();
    printf("Player movement address %p\n", playerMovement);
    if (!playerMovement) {
        return;
    }
    playerList = PlayerList::GetInstance();
    printf("player list address %p\n", playerList);
    if (!playerList) {
        return;
    }
    player = playerList->GetHost();
    printf("player address %p\n", player);
    printf("player network obj address %p\n", player->_networkObjectCache);

    officers = Officers::GetInstance();
    if (!officers) {
        printf("Officers not found\n");
        return;
    }
    officer = officers->_items->array[0];
    printf("Officer address %p\n", officer);
    char* officerName = reinterpret_cast<char *>(officer->fields.FirstName);
    printf("Officer name %s\n", officerName);

    mainCamera = UnityResolve::UnityType::Camera::GetMain();
    if (!mainCamera) {
        std::cout << "Камера не найдена!" << std::endl;
        return;
    }

    screenX = UnityResolve::UnityType::Screen::get_width() / 2.0f;
    screenY = UnityResolve::UnityType::Screen::get_height() / 2.0f;

    screenCenter.x = screenX;
    screenCenter.y = screenY;

    auto item = player->GetItem(2);
    printf("item address %p\n", item);
}