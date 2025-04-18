#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdint>
#include "Hook.h"


std::vector<UnityResolve::UnityType::GameObject*> objects;

void DestroyObject(int index) {
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


void CheatLogic() {
    InitAddresses();
    auto vtable = officer->klass->vtable;

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
        if (GetAsyncKeyState('F') & 0x8000) {
            auto methodAssignTarget = officer->fields.BodySearchBehaviour->klass->vtable._36_AssignTarget;
            AssignTarget f = (AssignTarget)methodAssignTarget.methodPtr;
            f(officer->fields.BodySearchBehaviour, 0, player->_networkObjectCache, methodAssignTarget.method);

            auto methodEnable = officer->fields.BodySearchBehaviour->klass->vtable._20_Enable;
            Enable f2 = (Enable)methodEnable.methodPtr;
            f2(officer->fields.BodySearchBehaviour, methodEnable.method);
        }
        if (GetAsyncKeyState('O') & 0x8000) {
            if (myCar) {
                myCar->SwitchPoliceLight();
            }
        }
        if (GetAsyncKeyState('R') & 0x8000) {
            objects.clear();

            auto ray = mainCamera->ScreenPointToRay(screenCenter);
            ray.m_vOrigin = ray.m_vOrigin + ray.m_vDirection * 1.0f;
            UnityResolve::UnityType::RaycastHit hit;
            bool isHit = UnityResolve::UnityType::Physics::Raycast(ray, &hit, 10.0f);
            if (isHit) {
                // Получаем Transform объекта, в который попал луч
                auto hitCollider = hit.get_collider();
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
//        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
//            DestroyObject(0);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
//            DestroyObject(1);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
//            DestroyObject(2);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
//            DestroyObject(3);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
//            DestroyObject(4);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
//            DestroyObject(5);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
//            DestroyObject(6);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
//            DestroyObject(7);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
//            DestroyObject(8);
//        }
//        if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
//            DestroyObject(9);
//        }
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            auto  itemInstance = player->GetItem()->GetItemInstance();
            itemInstance->_quantity = 228;
        }
        Sleep(50);
    }
}