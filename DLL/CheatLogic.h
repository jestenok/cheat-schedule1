#include <windows.h>
#include <d3d11.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "Hook.h"
#include "Interface.h"


std::vector<UnityResolve::UnityType::GameObject*> objects;

void DestroyObject(int index) {
    if ( objects.empty() ) {
        printf("Ошибка: список объектов пуст\n");
        return;
    }

    if (index < 0 || index >= objects.size()) {
        printf("Ошибка: индекс %d вне диапазона (0-%zu)\n", index, objects.size() - 1);
        return;
    }
    auto object = objects[index];
    printf("Объект удален %s\n", object->GetName()->ToString().c_str());
    object->Destroy();
}

//void LookAtMeBitch() {
//    v40 = **(ScheduleOne_PlayerScripts_PlayerCamera_o ***)(v39 + 184);
//    v41 = this->fields._beh_k__BackingField;
//    if ( v41 )
//    {
//        v42 = v41->fields._Npc_k__BackingField;
//        if ( v42 )
//        {
//            Avatar = v42->fields.Avatar;
//            if ( Avatar )
//            {
//                MiddleSpineRB = (UnityEngine_Component_o *)Avatar->fields.MiddleSpineRB;
//                if ( MiddleSpineRB )
//                {
//                    transform = UnityEngine_Component__get_transform(MiddleSpineRB, 0);
//                    if ( v40 )
//                    {
//                        ScheduleOne_PlayerScripts_PlayerCamera__FocusCameraOnTarget(v40, transform, 0);
//                        goto LABEL_45;
//                    }
//                }
//            }
//        }
//    }
//}

struct RaycastResult {
    bool isHit;
    UnityResolve::UnityType::RaycastHit hit;
};

RaycastResult RaycastHit(float distance = 100.0f) {
    auto ray = mainCamera->ScreenPointToRay(screenCenter);
    ray.m_vOrigin = ray.m_vOrigin + ray.m_vDirection * 1.0f;

    RaycastResult result;
    result.isHit = UnityResolve::UnityType::Physics::Raycast(ray, &result.hit, distance);
    return result;
}

void TeleportOneToOther(void* receiver, void* teleported) {
    auto teleportedComponent = reinterpret_cast<UnityResolve::UnityType::Component*>(teleported);
    auto teleportedTransform = teleportedComponent->GetTransform();
    auto teleportedPos = teleportedTransform->GetPosition();

    auto receiverComponent = reinterpret_cast<UnityResolve::UnityType::Component*>(receiver);
    auto receiverTransform = receiverComponent->GetTransform();
    auto receiverPos = receiverTransform->GetPosition();

    float dx = teleportedPos.x - receiverPos.x;
    float dy = teleportedPos.y - receiverPos.y;
    float dz = teleportedPos.z - receiverPos.z;

    float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    
    printf("teleported x %f %f %f\n", teleportedPos.x, teleportedPos.y, teleportedPos.z);
    printf("receiver x %f %f %f\n", receiverPos.x, receiverPos.y, receiverPos.z);
    printf("Distance %f\n", distance);

    Vector3 newPos;
    newPos.x = receiverPos.x + 0.4f;
    newPos.y = receiverPos.y + 0.4f;
    newPos.z = receiverPos.z;
    teleportedTransform->SetPosition(newPos);
}

int qwe = 0;
HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    //__int64 __fastcall sub_18008ED90(__int64 a1, __int64 a2, __int64 a3)
    InitImGui(pSwapChain, SyncInterval, Flags);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddCircle(ImVec2(100, 100), 50.0f, ImColor(255, 0, 0, 255), 12, 1.0f);

    bool isGameFocused = (GetForegroundWindow() == hWnd);
    isGameFocused = true;
    ULONGLONG currentTime = GetTickCount64();
    if (isGameFocused) {
        if (KeyPressed(VK_INSERT, currentTime)) {
            menu = !menu;
            lastToggleTime = currentTime;
        }
        if (menu) {
            ImGui::Begin("Cheat Menu");
            ImGui::Text("Hello, world!");
            ImGui::End();
        }
    }
    ImGui::Render();
    pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

void CheatLogic() {
    InitAddresses();

    while (!(GetAsyncKeyState(VK_END) & 0x8000)) {
        if (playerMovement) {
            if (GetAsyncKeyState(VK_F1) & 0x8000) {
                playerMovement->SetJumpForce();
                playerMovement->SetMoveSpeedMultiplier();
                playerMovement->fields.gravityMultiplier = 1.4;
            }

            if (GetAsyncKeyState(VK_F2) & 0x8000) {
                playerMovement->SetJumpForce(20);
                playerMovement->SetMoveSpeedMultiplier(5);
                playerMovement->fields.gravityMultiplier = 1.4;
            }
            if (GetAsyncKeyState('E') & 0x8000) {
                playerMovement->fields.gravityMultiplier = 0;
            }
            if (GetAsyncKeyState('Q') & 0x8000) {
                playerMovement->fields.gravityMultiplier = 1.4;
            }
        }

        if (GetAsyncKeyState('O') & 0x8000) {
            if (myCar) {
                myCar->SwitchPoliceLight();
            }
        }
        if (GetAsyncKeyState(VK_F4) & 0x8000) {
            auto targetPlayer = playerList->GetByIndex(0);
            if (!targetPlayer) {
                printf("Target player is null\n");
                return;
            }

            auto methodAssignTarget = officer->fields.BodySearchBehaviour->klass->vtable._36_AssignTarget;
            AssignTarget f = (AssignTarget)methodAssignTarget.methodPtr;
            f(officer->fields.BodySearchBehaviour, 0, targetPlayer->_networkObjectCache, methodAssignTarget.method);

            TeleportOneToOther(targetPlayer, officer);

            auto methodEnable = officer->fields.BodySearchBehaviour->klass->vtable._20_Enable;
            Enable f2 = (Enable)methodEnable.methodPtr;
            f2(officer->fields.BodySearchBehaviour, methodEnable.method);
        }
        if (GetAsyncKeyState('R') & 0x8000) {
            if (!player) {
                continue;
            }

            objects.clear();

            auto res = RaycastHit();
            if (res.isHit) {
                // Получаем Transform объекта, в который попал луч
                auto hitCollider = res.hit.get_collider();
                auto hitTransform = hitCollider->GetTransform();

                auto gameObject = hitTransform->GetGameObject();
                auto objectName = gameObject->GetName()->ToString();
                printf("level 0 %p %s\n", gameObject, objectName.c_str());
                objects.push_back(gameObject);

                for ( int i = 1; i < 10; i++) {
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
                    auto parentGameObject = parent->GetGameObject();
                    auto parentName = parentGameObject->GetName()->ToString();
                    objects.push_back(parentGameObject);

                    printf("level %d %p %s\n", i, parentGameObject, parentName.c_str());
                }
                printf("\n");
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
            DestroyObject(0);
        }
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
            DestroyObject(1);
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
            DestroyObject(2);
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
            DestroyObject(3);
        }
        if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
            DestroyObject(4);
        }
        if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
            DestroyObject(5);
        }
        if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
            DestroyObject(6);
        }
        if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
            DestroyObject(7);
        }
        if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
            DestroyObject(8);
        }
        if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
            DestroyObject(9);
        }
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            auto  itemInstance = player->GetItem()->GetItemInstance();
            itemInstance->_quantity = 228;
        }

        Sleep(50);
    }
}