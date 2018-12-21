#include "./imgui/imgui.h"
#include "cimgui.h"

#include "./imgui/imgui_internal.h"

struct CimguiStorage
{
    void(*Platform_GetWindowPos)(ImGuiViewport* vp, ImVec2* out_pos);
    void(*Platform_GetWindowSize)(ImGuiViewport* vp, ImVec2* out_pos);
};

CimguiStorage& GetCimguiStorage()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.BackendLanguageUserData == nullptr)
    {
        io.BackendLanguageUserData = new CimguiStorage();
    }

    return *(CimguiStorage*)io.BackendLanguageUserData;
}

CIMGUI_API void ImGuiPlatformIO_Set_Platform_GetWindowPos(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_pos))
{
    auto storage = GetCimguiStorage();
    storage.Platform_GetWindowPos = user_callback;

    platform_io->Platform_GetWindowPos = [](ImGuiViewport* vp)
    {
        ImVec2 pos;
        GetCimguiStorage().Platform_GetWindowPos(vp, &pos);
        return pos;
    };
}

CIMGUI_API void ImGuiPlatformIO_Set_Platform_GetWindowSize(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_size))
{
    auto storage = GetCimguiStorage();
    storage.Platform_GetWindowSize = user_callback;

    platform_io->Platform_GetWindowSize = [](ImGuiViewport* vp)
    {
        ImVec2 size;
        GetCimguiStorage().Platform_GetWindowSize(vp, &size);
        return size;
    };
}