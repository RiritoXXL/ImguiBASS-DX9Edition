#pragma once
#include <Windows.h>
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3d.h>
#include <tchar.h>
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = { NULL };
using namespace std;
// Forward declare message handler from imgui_impl_win32.cpp
// Data

LPCWSTR OpenFileDialog(TCHAR szFileName[]) {
    OPENFILENAME ofn;       // common dialog box structure
    HWND hwnd;              // owner window
    HANDLE hf;              // file handle
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = (LPWSTR)szFileName;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = (LPWSTR)L"MP3 File\0*.*\0File of MP3\0*.mp3\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    // Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == TRUE) 
    {
       hf = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
       return (LPCWSTR)szFileName;
    }
    return (LPCWSTR)L"";
}
namespace DX_9
{
    // Helper Functions
    bool CreateDeviceD3D(HWND hWnd)
    {
        g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

        // Create the D3DDevice
        ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
        //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
        if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
            return false;

        return true;
    }
    void CleanupDeviceD3D()
    {
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
        if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
    }
    void ResetDevice() {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
        if (hr == D3DERR_INVALIDCALL)
            IM_ASSERT(0);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
}
bool done = false;
bool active = true;
namespace IMGUIRENDER
{
    const char* window_title = "IMGUI BASS by RiritoXXL";
    ImVec2 window_size{ 400, 400 };
    DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
    void Render()
    {
        ImGui::SetNextWindowSize(window_size);
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::Begin(window_title, &active, window_flags);
        {
            if (ImGui::Button("Play Music")) 
            {
                TCHAR openedFileName[MAX_PATH];
                OpenFileDialog(openedFileName);
            }
        }
        ImGui::End();
    }
}
