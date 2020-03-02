
#include "./imgui/imgui.h"
#include "./imgui/imgui_internal.h"
#include "cimgui.h"



#include "auto_funcs.cpp"


/////////////////////////////manual written functions
CIMGUI_API void igLogText(CONST char *fmt, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, 256, fmt, args);
    va_end(args);

    ImGui::LogText("%s", buffer);
}
CIMGUI_API void ImGuiTextBuffer_appendf(struct ImGuiTextBuffer *buffer, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    buffer->appendfv(fmt, args);
    va_end(args);
}

CIMGUI_API float igGET_FLT_MAX()
{
    return FLT_MAX;
}
CIMGUI_API void igColorConvertRGBtoHSV(float r,float g,float b,float *out_h,float *out_s,float *out_v)
{
    ImGui::ColorConvertRGBtoHSV(r,g,b,*out_h,*out_s,*out_v);
}
CIMGUI_API void igColorConvertHSVtoRGB(float h,float s,float v,float *out_r,float *out_g,float *out_b)
{
    ImGui::ColorConvertHSVtoRGB(h,s,v,*out_r,*out_g,*out_b);
}

CIMGUI_API ImVector_ImWchar* ImVector_ImWchar_create()
{
	return IM_NEW(ImVector<ImWchar>) ();
}

CIMGUI_API void ImVector_ImWchar_destroy(ImVector_ImWchar* self)
{
    IM_DELETE(self);
}

CIMGUI_API void ImVector_ImWchar_Init(ImVector_ImWchar* p)
{
	IM_PLACEMENT_NEW(p) ImVector<ImWchar>();
}
CIMGUI_API void ImVector_ImWchar_UnInit(ImVector_ImWchar* p)
{
	p->~ImVector<ImWchar>();
}

#ifdef IMGUI_HAS_DOCK

// NOTE: Some function pointers in the ImGuiPlatformIO structure are not C-compatible because of their
// use of a complex return type. To work around this, we store a custom CimguiStorage object inside
// ImGuiIO::BackendLanguageUserData, which contains C-compatible function pointer variants for these
// functions. When a user function pointer is provided, we hook up the underlying ImGuiPlatformIO
// function pointer to a thunk which accesses the user function pointer through CimguiStorage.

struct CimguiStorage
{
    void(*Platform_GetWindowPos)(ImGuiViewport* vp, ImVec2* out_pos);
    void(*Platform_GetWindowSize)(ImGuiViewport* vp, ImVec2* out_pos);
};

// Gets a reference to the CimguiStorage object stored in the current ImGui context's BackendLanguageUserData.
CimguiStorage& GetCimguiStorage()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.BackendLanguageUserData == NULL)
    {
        io.BackendLanguageUserData = new CimguiStorage();
    }

    return *(CimguiStorage*)io.BackendLanguageUserData;
}

// Thunk satisfying the signature of ImGuiPlatformIO::Platform_GetWindowPos.
ImVec2 Platform_GetWindowPos_hook(ImGuiViewport* vp)
{
    ImVec2 pos;
    GetCimguiStorage().Platform_GetWindowPos(vp, &pos);
    return pos;
};

// Fully C-compatible function pointer setter for ImGuiPlatformIO::Platform_GetWindowPos.
CIMGUI_API void ImGuiPlatformIO_Set_Platform_GetWindowPos(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_pos))
{
    CimguiStorage& storage = GetCimguiStorage();
    storage.Platform_GetWindowPos = user_callback;
    platform_io->Platform_GetWindowPos = &Platform_GetWindowPos_hook;
}

// Thunk satisfying the signature of ImGuiPlatformIO::Platform_GetWindowSize.
ImVec2 Platform_GetWindowSize_hook(ImGuiViewport* vp)
{
    ImVec2 size;
    GetCimguiStorage().Platform_GetWindowSize(vp, &size);
    return size;
};

// Fully C-compatible function pointer setter for ImGuiPlatformIO::Platform_GetWindowSize.
CIMGUI_API void ImGuiPlatformIO_Set_Platform_GetWindowSize(ImGuiPlatformIO* platform_io, void(*user_callback)(ImGuiViewport* vp, ImVec2* out_size))
{
    CimguiStorage& storage = GetCimguiStorage();
    storage.Platform_GetWindowSize = user_callback;
    platform_io->Platform_GetWindowSize = &Platform_GetWindowSize_hook;
}

#endif
