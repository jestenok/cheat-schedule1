#include <MinHook.h>
#include "Adresses.h"


void* GetMethodAddress(const char* className, const char* methodName) {
    auto klass = assembly->Get(className);
    if(!klass) {
        printf("Class not found: %s\n", className);
        return nullptr;
    }

    auto method = klass->Get<UnityResolve::Method>(methodName);
    if(!method) {
        printf("Method not found: %s\n", methodName);
        return nullptr;
    }

    return method->GetInvokeAddress();
}

template<typename T>
struct HookWrapper {
    intptr_t offset;
    const char* className;
    const char* methodName;
    T hook;
    void** original;
    void** target;
};

class HookManager {
public:
    std::vector<HookWrapper<void*>> hooks;

    HookManager() {
        hooks.reserve(100);
    }

    template<typename FuncType>
    void Add(
            intptr_t offset,
            const char* methodName,
            FuncType hook,
            FuncType* original,
            void** target
    ) {
        HookWrapper<void*> wrapper{};
        wrapper.offset = offset;
        wrapper.methodName = methodName;
        wrapper.hook = reinterpret_cast<void*>(hook);
        wrapper.original = reinterpret_cast<void**>(original);
        wrapper.target = target;
        hooks.push_back(wrapper);
    }

    virtual void ResolveAll() {
        for(auto& hook : hooks) {
            uintptr_t address = GAME_ASSEMBLY_ADDRESS + hook.offset;
            *hook.target = reinterpret_cast<void*>(address);
        }
    }

    void EnableAll() {
        for(auto& hook : hooks) {
            MH_CreateHook(
                    *hook.target,
                    hook.hook,
                    hook.original);
            printf("Method name %s at address %p\n", hook.methodName, *hook.target);
        }
        MH_EnableHook(MH_ALL_HOOKS);
    }

    void SetupHook()
    {
        ResolveAll();
        EnableAll();
        log("Hook set!");
    }

    void RemoveHook()
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_RemoveHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }
};

class ResolverHookManager : public HookManager {
public:
    template<typename FuncType>
    void Add(
            const char* className,
            const char* methodName,
            FuncType hook,
            FuncType* original,
            void** target
    ) {
        HookWrapper<void*> wrapper{};
        wrapper.className = className;
        wrapper.methodName = methodName;
        wrapper.hook = reinterpret_cast<void*>(hook);
        wrapper.original = reinterpret_cast<void**>(original);
        wrapper.target = target;
        hooks.push_back(wrapper);
    }

    void RemoveHook(void* target)
    {
        for (auto& hook : hooks) {
            if(*hook.target == target) {
                MH_DisableHook(*hook.target);
//                MH_RemoveHook(*hook.target);
                printf("Hook  %s %p removed\n", hook.methodName, *hook.target);
                return;
            }
        }
    }

    void ResolveAll() override {
        for(auto& hook : hooks) {
            auto addr = GetMethodAddress(hook.className, hook.methodName);
            if(!addr) {
                printf("Failed to resolve method %s\n", hook.methodName);
                continue;
            }
            *hook.target = addr;
        }
    }
};

static HookManager hookManager;
static ResolverHookManager resolverHookManager;


#define METHOD_BY_OFFSET(offset, method, ret, conv, ...) \
    typedef ret(conv* t##method)(__VA_ARGS__); \
    static inline uintptr_t a##method = GAME_ASSEMBLY + offset; \
    static inline t##method o##method = (t##method)a##method;


#define HOOK_METHOD_BY_OFFSET(offset, method, ret, conv, ...) \
    typedef ret(conv* t##method)(__VA_ARGS__); \
    static inline t##method o##method = nullptr; \
    static ret conv hk##method(__VA_ARGS__); \
    static inline void* a##method = nullptr; \
    struct HookRegister_##method { \
        HookRegister_##method() { \
            hookManager.Add(offset, #method, &hk##method, &o##method, &a##method); \
        } \
    }; \
    static inline HookRegister_##method HookRegister_##method##_instance; \
    static ret conv hk##method(__VA_ARGS__)


#define HOOK_METHOD(klass, method, ret, conv, ...) \
    typedef ret(conv* t##klass##_##method)(__VA_ARGS__); \
    static inline t##klass##_##method o##klass##_##method = nullptr;\
    static ret conv hk##klass##_##method(__VA_ARGS__); \
    static inline void* a##klass##_##method = nullptr; \
    struct HookRegister_##klass##_##method { \
        HookRegister_##klass##_##method() { \
            resolverHookManager.Add(#klass, #method, &hk##klass##_##method, &o##klass##_##method, &a##klass##_##method); \
        } \
    }; \
    static inline HookRegister_##klass##_##method HookRegister_##klass##_##method##_instance; \
    static ret conv hk##klass##_##method(__VA_ARGS__)

