
#include <string>

#pragma comment (lib, "ws2_32.lib")
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include "awesome.h"
#include "awesome_light.h"
#include "icons.h"
#include "Roboto-Bold.h"
#include "../imgui_internal.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <D3dx9tex.h>
#include <vector>
#pragma comment(lib, "D3dx9")
using namespace std;
using namespace ImGui;


ImFont* logo;
ImFont* close;
ImFont* main_font;
ImFont* profile_tabs;
ImFont* footer;



struct game_data {
    uint8_t id;
    uint8_t status;
    std::string title;
    std::string sub;
    std::string date;
    std::string icon;
    std::string last_upd;


};
class user {
    public:

        std::vector<game_data> games;
        game_data selected_game;
};







// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool inject_btn(const char* label,bool* v, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_MouseButtonLeft);

    ImColor c_hovered_l = ImColor(178, 72, 98);
    ImColor c_hovered_r = ImColor(197, 101, 84);

    ImColor c_held_l = ImColor(147, 56, 78);
    ImColor c_held_r = ImColor(140, 72, 60);

    ImColor c_l = ImColor(160, 62, 86);
    ImColor c_r = ImColor(160, 85, 71);
       
    window->DrawList->InjectButton(ImVec2(bb.Min.x, bb.Min.y + 2), ImVec2(bb.Max.x, bb.Max.y + 2), held ? c_held_l : hovered ? c_hovered_l : c_l, held ? c_held_r : hovered ? c_hovered_r : c_r, held ? c_held_r : hovered ? c_hovered_r : c_r, held ? c_held_l : hovered ? c_hovered_l : c_l, 6, 15);
    ImGui::PushFont(logo);
    window->DrawList->AddText(ImVec2(bb.Min.x + size_arg.x / 2 - ImGui::CalcTextSize(label).x / 2, bb.Min.y + size_arg.y / 2 + 1 - ImGui::CalcTextSize(label).y / 2 + 2), ImColor(238, 230, 236), label);
    ImGui::PopFont();
     
    return pressed;
}



void Rener_Profile(ImTextureID user_texture_id, float radius, ImU32 col, int num_segments)
{
    auto window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    window->DrawList->PathClear();
    ImVec2 pos = window->DC.CursorPos;
    if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2)
        return;


    const bool push_texture_id = window->DrawList->_TextureIdStack.empty() || user_texture_id != window->DrawList->_TextureIdStack.back();
    if (push_texture_id)
        window->DrawList->PushTextureID(user_texture_id);

    int vert_start_idx = window->DrawList->VtxBuffer.Size;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
    window->DrawList->PathArcTo(pos, radius, 0.0f, a_max, num_segments - 1);
    window->DrawList->PathFillConvex(col);
    int vert_end_idx = window->DrawList->VtxBuffer.Size;

    ImGui::ShadeVertsLinearUV(window->DrawList, vert_start_idx, vert_end_idx, ImVec2(pos.x - radius, pos.y - radius), ImVec2(pos.x + radius, pos.y + radius), ImVec2(0, 0), ImVec2(1, 1), true);

    if (push_texture_id)
        window->DrawList->PopTextureID();


}


void render_status(const char* icon, const char* label, int status,const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    ImColor c_bg = ImColor(48, 37, 70);
    ImColor c_badge_bg = ImColor(32, 71, 59);
    ImColor c_badge_txt = ImColor(9, 171, 108);

    ImColor c_badge_bg2 = ImColor(84, 24, 24);
    ImColor c_badge_txt2 = ImColor(161, 49, 49);
    window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), c_bg, 6, 15);
    window->DrawList->AddText(profile_tabs,11,ImVec2(bb.Min.x +6, bb.Min.y + size_arg.y / 2 + 1 - ImGui::CalcTextSize(label).y / 2 + 2),ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9)), icon);
    window->DrawList->AddText(logo, 14, ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x/ 2-5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2-1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9)), label);


    if (status == 0) {
        window->DrawList->AddRectFilled(ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x * 1.7, bb.Min.y + 6), ImVec2(bb.Max.x - 6, bb.Max.y - 6), c_badge_bg, 6, 15);
        window->DrawList->AddText(logo, 17, ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x + ImGui::CalcTextSize("Undetected").x, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize("Undetected").y / 2 - 2), c_badge_txt, "Undetected");

    }
    else {

        window->DrawList->AddRectFilled(ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x * 1.7, bb.Min.y + 6), ImVec2(bb.Max.x - 6, bb.Max.y - 6), c_badge_bg2, 6, 15);
        window->DrawList->AddText(logo, 17, ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x + ImGui::CalcTextSize("Detected").x+20, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize("Detected").y / 2 - 2), c_badge_txt2, "Detected");

    }
  

}


void render_lastup(const char* icon, const char* label, const char* lastupd, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return;

    ImColor c_bg = ImColor(48, 37, 70);


    window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), c_bg, 6, 15);
    window->DrawList->AddText(profile_tabs, 11, ImVec2(bb.Min.x + 6, bb.Min.y + size_arg.y / 2 + 1 - ImGui::CalcTextSize(label).y / 2 + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9)), icon);
    window->DrawList->AddText(logo, 14, ImVec2(bb.Min.x + ImGui::CalcTextSize(label).x / 2 - 5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(label).y / 2 - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9)), label);

    window->DrawList->AddText(logo, 14, ImVec2(bb.Max.x- ImGui::CalcTextSize(lastupd).x -10, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(lastupd).y / 2 - 2), ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9)), lastupd);


}



bool render_cheat(const char* icon, const char* label, const char* title, const char* sub, const char* date, bool v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size_arg = ImVec2(window->Size.x, 51);

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;;

    ImColor c_bg = ImColor(0,0,0,0);
    ImColor c_hovered_bg = ImColor(47, 36, 67, 150);
    ImColor c_pressed_bg = ImColor(30, 23, 43, 190);
    ImColor c_txt = ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.55, 0.65, 0.9));
    ImColor c_txt_active = ImColor(190, 75, 85);




    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_MouseButtonLeft);



    window->DrawList->AddRectFilled(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Max.x, bb.Max.y), held ? c_pressed_bg : hovered ? c_hovered_bg : v? c_bg : c_bg, 0, 0);
    window->DrawList->AddText(profile_tabs, 16, ImVec2(bb.Min.x + 10, bb.Min.y + size_arg.y / 2 + 1 - ImGui::CalcTextSize(label).y / 2 + 2), v ? c_txt_active: c_txt, icon);
    window->DrawList->AddText(logo, 16, ImVec2(bb.Min.x + 10+ ImGui::CalcTextSize(icon).x*5, bb.Min.y + size_arg.y / 2 - ImGui::CalcTextSize(title).y / 2 - 11), v ? c_txt_active : c_txt, title);

    window->DrawList->AddText(profile_tabs, 7, ImVec2(bb.Min.x + 50, bb.Min.y +35), v ? c_txt_active : c_txt, ICON_FA_HOURGLASS);
    window->DrawList->AddText(logo, 12, ImVec2(bb.Min.x + 63, bb.Min.y + 30), v ? c_txt_active : c_txt, sub);


    window->DrawList->AddText(profile_tabs, 7, ImVec2(bb.Min.x + 120, bb.Min.y + 35), v ? c_txt_active : c_txt, ICON_FA_CALENDAR_ALT);
    window->DrawList->AddText(logo, 12, ImVec2(bb.Min.x + 133, bb.Min.y + 30), v ? c_txt_active : c_txt, date);
  
    return pressed;

}


int main(int, char**)
{

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX9 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);


   


    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;

    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 14.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    logo = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 17.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    close = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(awesome_light_compressed_data, awesome_light_compressed_size, 24.0f, &icons_config, icons_ranges);
    footer = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Roboto_compressed_data, Roboto_compressed_size, 13.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    profile_tabs = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Awesome, Awesome_size, 16.f, &icons_config, icons_ranges);
    ImGui::StyleColorsDark();
 
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
   
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2{ 559, 353 });


        ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

        ImGuiStyle* Style = &ImGui::GetStyle();
        Style->WindowBorderSize = 1.5;
        Style->FrameBorderSize = 0;
        Style->ChildBorderSize = 1.5;
        Style->WindowRounding = 0;
        Style->ChildRounding = 0;
        Style->FrameRounding = 0;
        Style->ScrollbarSize = 0;
        Style->WindowPadding = ImVec2(0, 0);
        Style->FramePadding = ImVec2(0, 0);
        Style->ItemSpacing = ImVec2(0, 0);
        Style->ItemInnerSpacing = ImVec2(0, 0);
        Style->IndentSpacing = 0;
        Style->DisplayWindowPadding = ImVec2(0, 0);
        Style->Colors[ImGuiCol_Border] = ImColor(66, 54, 89);
        Style->Colors[ImGuiCol_WindowBg] = ImColor(48, 37, 70);
        Style->Colors[ImGuiCol_ChildBg] = ImColor(40, 31, 56);
        Style->Colors[ImGuiCol_Text] = ImColor(212, 209, 218);

        //
        Style->Colors[ImGuiCol_FrameBg] = ImColor(33, 33, 33);
        Style->Colors[ImGuiCol_ScrollbarBg] = ImColor(33, 37, 41);
        Style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(235, 63, 58);
        Style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(244, 77, 72);
        Style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(184, 47, 43);
        Style->Colors[ImGuiCol_FrameBg] = ImColor(33, 37, 41);
        Style->Colors[ImGuiCol_FrameBgHovered] = ImColor(33, 37, 41);
        Style->Colors[ImGuiCol_FrameBgActive] = ImColor(47, 52, 56);
        

        //For games ex.;
     
        static bool checkbox = false;

        user current_user;

        current_user.games.emplace_back(game_data{ 0, 1, "Counter-Strike: Global Offensive", "1 week", "18 Feb, 2021",ICON_FA_GHOST,"2 days ago" });
        current_user.games.emplace_back(game_data{ 1, 0, "Counter-Strike: Global Offensive", "3 week", "18 Feb, 2022",ICON_FA_SKULL ,"2 weeks ago"});

        if (ImGui::Begin("Test", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar)) {
            ImGuiWindow* window = ImGui::GetCurrentWindow();

            ImGui::SetCursorPos(ImVec2(0,0));
            if(ImGui::BeginChild("top",ImVec2(559, 39),true, ImGuiWindowFlags_NoDecoration)){
                ImGui::SetCursorPos(ImVec2(12, 10));
                ImGui::PushFont(logo);
                ImGui::Text("INTERWEBZ");
                ImGui::PopFont();

                ImGui::SetCursorPos(ImVec2(533,12));
                ImGui::PushFont(close);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.59, 0.55, 0.65,1));
                if (ImGui::Button(u8"\uf00d",ImVec2(20,20))) {

               }
                ImGui::PopStyleColor(4);
                ImGui::PopFont();


            }
            ImGui::EndChild();
            
            ImGui::SetCursorPos(ImVec2(15,54));

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3);

            if (ImGui::BeginChild("Cheat List", ImVec2(256, 224), true, ImGuiWindowFlags_NoDecoration)) {

                static int selected = 0;
                for (int i = 0; i < current_user.games.size(); i++) {
                    auto& game = current_user.games[i];
                    if (render_cheat(game.icon.c_str(),to_string(i).c_str(), game.title.c_str(), game.sub.c_str(), game.date.c_str(), i == selected)) {
                        selected = i;
                    }


                };
                auto game = current_user.games[selected];
                current_user.selected_game = game;

            }
            ImGui::EndChild();
           
            ImGui::SetCursorPos(ImVec2(287, 54));
            if (ImGui::BeginChild("Profile", ImVec2(256, 224), true, ImGuiWindowFlags_NoDecoration)) {     

                ImGui::SetCursorPos(ImVec2(45, 45));
                Rener_Profile(nullptr, 25, ImColor(196, 196, 196), 23);

                ImGui::SetCursorPos(ImVec2(78, 30));
                ImGui::Text("Media51");

                ImGui::SetCursorPos(ImVec2(78, 45));
                ImGui::TextColored(ImVec4(0.59, 0.55, 0.65, 0.9), "18 Feb, 2021");


                ImGui::SetCursorPos(ImVec2(15, 80));
                render_status(ICON_FA_INFO_CIRCLE, "Cheat Status", current_user.selected_game.status, ImVec2(223, 36));

                ImGui::SetCursorPos(ImVec2(15, 124));
                render_lastup(ICON_FA_HISTORY, "Last Updated", current_user.selected_game.last_upd.c_str(), ImVec2(223, 36));



                ImGui::SetCursorPos(ImVec2(18, 205));        
                ImGui::PushFont(footer);
                ImGui::TextColored(ImVec4(0.59, 0.55, 0.65, 0.9), "@ 2021 INTERWEBZ. All Rights Reserved.");
                ImGui::PopFont();

            }
            ImGui::EndChild();

            ImGui::SetCursorPos(ImVec2(15, 294));

            inject_btn("Load Cheat", &checkbox,ImVec2(528, 42));

            ImGui::PopStyleVar();
   
        } ImGui::End();



        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*255.0f), (int)(clear_color.y*255.0f), (int)(clear_color.z*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
