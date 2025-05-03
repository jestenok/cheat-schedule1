#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>


inline HWND hWnd = nullptr;
inline ID3D11Device* pDevice = nullptr;
inline ID3D11DeviceContext* pContext = nullptr;
inline ID3D11RenderTargetView* pRenderTargetView = nullptr;
inline WNDPROC oWndProc = nullptr;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

typedef HRESULT(__stdcall* tPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
tPresent oPresent = nullptr;

bool ImGuiInit = false;
bool menu = false;

bool InitImGui(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!ImGuiInit) {
        printf("++++++++++++++++++++++++++++++ ImGuiInit ++++++++++++++++++++++++++++++\n");
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            hWnd = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
            ImGui_ImplWin32_Init(hWnd);
            ImGui_ImplDX11_Init(pDevice, pContext);
            ImGuiInit = true;
        }
    }
    return ImGuiInit;
}

static ULONGLONG lastToggleTime = 0;
bool KeyPressed(int key, ULONGLONG currentTime) {
    return ((GetAsyncKeyState(key) & 0x8000) && (currentTime - lastToggleTime > 200));
}

void CleanupD3D() {
    if (ImGuiInit) {
        // Восстанавливаем оригинальный WndProc
        if (hWnd && oWndProc) {
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR) oWndProc);
            oWndProc = nullptr;
        }

        // Освобождаем ресурсы ImGui
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        // Освобождаем DirectX объекты
        if (pRenderTargetView) {
            pRenderTargetView->Release();
            pRenderTargetView = nullptr;
        }
        if (pContext) {
            pContext->Release();
            pContext = nullptr;
        }
        if (pDevice) {
            pDevice->Release();
            pDevice = nullptr;
        }

        ImGuiInit = false;
    }
}