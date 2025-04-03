#include <MinHook.h>
#include "Adresses.h"


template<typename T>
struct HookWrapper {
    intptr_t offset;
    const char* methodName;
    T original;
    T hook;
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
            uintptr_t *target
    ) {
        HookWrapper<void*> wrapper{};
        wrapper.offset = offset;
        wrapper.methodName = methodName;
        wrapper.hook = reinterpret_cast<void*>(hook);
        wrapper.original = reinterpret_cast<void*>(original);
        wrapper.target = reinterpret_cast<void**>(target);
        hooks.push_back(wrapper);
        printf("Hook added: %s at offset 0x%X\n", methodName, offset);
    }

    void ResolveAll() {
    }

    void EnableAll() {
        MH_Initialize();
        for(auto& hook : hooks) {
            MH_CreateHook(
                    *hook.target,
                    hook.hook,
                    reinterpret_cast<LPVOID*>(hook.original)
            );
            printf("Method name %s at address 0x%X\n", hook.methodName, *hook.target);
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

static HookManager hookManager;


#define METHOD(offset, method, ret, conv, ...) \
    typedef ret(conv* t##method)(__VA_ARGS__); \
    static inline uintptr_t a##method = baseAssembly + offset; \
    static inline t##method o##method = (t##method)a##method;


#define HOOK_METHOD(offset, method, ret, conv, ...) \
    typedef ret(conv* t##method)(__VA_ARGS__); \
    static inline t##method o##method = nullptr; \
    static ret conv hk##method(__VA_ARGS__); \
    static inline uintptr_t a##method = baseAssembly + offset; \
    struct HookRegister_##method { \
        HookRegister_##method() { \
            hookManager.Add(offset, #method, &hk##method, &o##method, &a##method); \
        } \
    }; \
    static inline HookRegister_##method HookRegister_##method##_instance; \
    static ret conv hk##method(__VA_ARGS__)

