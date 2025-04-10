#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include "Hook.h"

void PrintObject(UnityResolve::UnityType::GameObject* obj) {}

void CheatLogic() {
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

    officers = Officers::GetInstance();
    if (!officers) {
        printf("Officers not found\n");
        return;
    }
    PoliceOfficer* officer = officers->_items->array[0];

    auto* mainCamera = UnityResolve::UnityType::Camera::GetMain();
    if (!mainCamera) {
        std::cout << "Камера не найдена!" << std::endl;
        return;
    }
    printf("mainCamera address %p\n", mainCamera);

    auto screenX = UnityResolve::UnityType::Screen::get_width() / 2.0f;
    auto screenY = UnityResolve::UnityType::Screen::get_height() / 2.0f;

    UnityResolve::UnityType::Vector2 screenCenter(screenX, screenY);

    auto vtable = officer->klass->vtable;
    printf("Officers vtable %p\n", vtable);
    auto method = vtable->_88_BeginBodySearch_Networked;
    printf("method address %p\n", method);
    printf("method pointer address %p\n", method.methodPtr);

    tBeginBodySearch_Networked oBeginBodySearch_Networked = (tBeginBodySearch_Networked)0x7FFCB642BE90;


//    tBeginBodySearch_Networked oBeginBodySearch_Networked = (tBeginBodySearch_Networked)method.methodPtr;
//        oBeginBodySearch_Networked(officer, player->_networkObjectCache);
    UnityResolve::UnityType::GameObject* gameObject = nullptr;

    while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            playerMovement->SetJumpForce();
            playerMovement->SetMoveSpeedMultiplier();
            playerMovement->gravityMultiplier = 1.4;
        }

        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            playerMovement->SetJumpForce(20);
            playerMovement->SetMoveSpeedMultiplier(5);
            playerMovement->gravityMultiplier = 1.4;
        }

        if (GetAsyncKeyState('E') & 0x8000) {
            playerMovement->gravityMultiplier = 0;
        }
        if (GetAsyncKeyState('Q') & 0x8000) {
            playerMovement->gravityMultiplier = 1.4;
        }
        if (GetAsyncKeyState('O') & 0x8000) {
            myCar->SwitchPoliceLight();
        }
        if (GetAsyncKeyState('R') & 0x8000) {
            auto ray = mainCamera->ScreenPointToRay(screenCenter);
            ray.m_vOrigin = ray.m_vOrigin + ray.m_vDirection * 1.0f;
            UnityResolve::UnityType::RaycastHit hit;
            bool isHit = UnityResolve::UnityType::Physics::Raycast(ray, &hit, 10.0f);
            if (isHit) {
                // Получаем Transform объекта, в который попал луч
                auto hitCollider = hit.get_collider();
                auto hitTransform = hitCollider->GetTransform();

                gameObject = hitTransform->GetGameObject();
                auto name = gameObject->GetName()->ToString();
                printf("GameObject %p %s\n", gameObject, name.c_str());

                for ( int i = 0; i < 10; i++) {
                    auto parent = hitTransform->GetParent();
                    if (!parent) {
                        break;
                    }
                    if (parent == hitTransform) {
                        printf("Parent is self\n");
                        break;
                    } else {
                        hitTransform = parent;
                    }
                    gameObject = parent->GetGameObject();
                    name = gameObject->GetName()->ToString();

                    printf("level %d %p %s\n", i, gameObject, name.c_str());
                }
                printf("\n");
            }
        }
        if (GetAsyncKeyState('T') & 0x8000) {
//            gameObject->Destroy();
        }
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            auto  itemInstance = player->GetItem()->GetItemInstance();
            itemInstance->_quantity = 228;
        }
        Sleep(50);
    }
}