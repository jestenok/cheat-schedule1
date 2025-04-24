#include "DataStructures.h"


LandVehicle* myCar;
PlayerMovement* playerMovement;
Officers* officers;
ScheduleOne_Police_PoliceOfficer_o* officer;
PlayerList* playerList;
Player* player;
UnityResolve::UnityType::Camera* mainCamera;
UnityResolve::UnityType::Vector2 screenCenter;

float screenX;
float screenY;


void InitAddresses() {
    playerList = PlayerList::GetInstance();
    printf("player list address %p\n", playerList);
    if (playerList) {
        player = playerList->GetHost();
        if (player) {
            printf("player address %p\n", player);
            printf("player network obj address %p\n", player->_networkObjectCache);

            auto item = player->GetItem(2);
            printf("item address %p\n", item);
        }
    }

    officers = Officers::GetInstance();
    if (officers) {
        officer = officers->_items->array[0];
        printf("Officer address %p\n", officer);
        char* officerName = reinterpret_cast<char *>(officer->fields.FirstName);
        printf("Officer name %s\n", officerName);
    }

    mainCamera = UnityResolve::UnityType::Camera::GetMain();

    screenX = UnityResolve::UnityType::Screen::get_width() / 2.0f;
    screenY = UnityResolve::UnityType::Screen::get_height() / 2.0f;

    screenCenter.x = screenX;
    screenCenter.y = screenY;
}