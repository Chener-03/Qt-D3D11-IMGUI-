#ifndef QD3D11WINDOW_HPP
#define QD3D11WINDOW_HPP

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <thread>
#include <math.h>
#include <iostream>
#include <D3DX11.h>

enum e_flags
    {
        circle_normal,
        circle_filled,
        circle_3d,

        rect_normal,
        rect_filled,

        triangle_normal,
        triangle_filled,

        text_normal,
        text_with_font
    };

namespace D3DIMGUI
{
void draw_text(float_t x, float_t y, const char* text, bool outlined,
               ImColor color = ImColor(255, 255, 255, 255), e_flags flags = text_normal, ImFont* font = nullptr, ...)
{
    switch (flags)
    {
    case text_normal:

        if (outlined)
        {
            ImGui::GetWindowDrawList()->AddText(ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
            ImGui::GetWindowDrawList()->AddText(ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
        }

        ImGui::GetWindowDrawList()->AddText(ImVec2(x, y), color, text);
        break;
    case text_with_font:

        if (outlined)
        {
            ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
            ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
        }

        ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y), color, text);
        break;
    default:
        break;
    }
}

void draw_line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float_t thickness)
{
    ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void draw_rect(float_t x, float_t y, float_t w, float_t h, ImColor color, e_flags flags, float_t rounding, uintptr_t points, float_t thickness)
{
    switch (flags)
    {
    case rect_normal:
        ImGui::GetWindowDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points, thickness);
        break;
    case rect_filled:
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points);
        break;
    default:
        break;
    }
}

void draw_triangle(float_t x1, float_t y1, float_t x2, float_t y2, float_t x3, float_t y3, ImColor color, e_flags flags, float_t thickness)
{
    switch (flags)
    {
    case rect_normal:
        ImGui::GetWindowDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color, thickness);
        break;
    case rect_filled:
        ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
        break;
    default:
        break;
    }
}

void draw_circle(float_t x, float_t y, float_t radius, ImColor color, e_flags flags, uintptr_t points, float_t thickness)
{
    switch (flags)
    {
    case circle_normal:
        ImGui::GetWindowDrawList()->AddCircle(ImVec2(x, y), radius, color, points, thickness);
        break;
    case circle_filled:
        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, points);
        break;
    case circle_3d:
        break;
    default:
        break;
    }
}

}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

template<class T>
class QD3D11WINDOW
{
public:
    static T* InitInstance()
    {
        if(m_pd3dwindowinstance==NULL)
            m_pd3dwindowinstance=new T();
        return m_pd3dwindowinstance;
    }

    static void destroyWindow()
    {
        if(m_pd3dwindowinstance!=NULL)
        {
            delete m_pd3dwindowinstance;
            m_pd3dwindowinstance=NULL;
        }
    }

    virtual void DrawEvent();

    void show();

    void hide();

    HWND winID(){return m_hwnd;}

    void setWindowTitle(wchar_t* title);

    void DrawImage(wchar_t* lpszFilePath,int x,int y,int w,int h);

    static T* m_pd3dwindowinstance;
private:
    QD3D11WINDOW(const QD3D11WINDOW& s);
    QD3D11WINDOW& operator = (QD3D11WINDOW& s);
protected:
    QD3D11WINDOW();
    ~QD3D11WINDOW();
    static T* pd3d11_window;
    ID3D11ShaderResourceView* DX11LoadTextureImageFromFile(ID3D11Device *pD3dDevice, wchar_t* lpszFilePath);
    bool created3dwindow();
    void maindraw();
    HWND m_hwnd;
    ID3D11Device*            g_pd3dDevice = NULL;
    ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
    IDXGISwapChain*          g_pSwapChain = NULL;
    ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
    bool isDistory=false;
    ImVec4 clear_color = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    std::thread* mainthread;

    bool CreateDeviceD3D(HWND hWnd);
    void CleanupDeviceD3D();
    void CreateRenderTarget();
    void CleanupRenderTarget();

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void DrawMain();
};

template<typename T> T* QD3D11WINDOW<T>::pd3d11_window;
template<typename T> T* QD3D11WINDOW<T>::m_pd3dwindowinstance=NULL;

template<class T>
QD3D11WINDOW<T>::QD3D11WINDOW()
{
    pd3d11_window=(T*)this;
    created3dwindow();
    CreateDeviceD3D(m_hwnd);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    mainthread=new std::thread(DrawMain);

}


template<class T>
QD3D11WINDOW<T>::~QD3D11WINDOW()
{
    isDistory=true;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(m_hwnd);
}


template<class T>
void QD3D11WINDOW<T>::DrawEvent()
{
    ImGui::ShowDemoWindow();
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&pd3d11_window->clear_color);

        if (ImGui::Button("Button"))
        {
            counter++;
        }

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }


}


template<class T>
void QD3D11WINDOW<T>::show()
{
    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);
}


template<class T>
void QD3D11WINDOW<T>:: setWindowTitle(wchar_t* title)
{
    ::SetWindowTextW(m_hwnd,title);
}

template<class T>
void QD3D11WINDOW<T>::hide()
{
    ::ShowWindow(m_hwnd,SW_HIDE);
}


template<class T>
bool QD3D11WINDOW<T>::created3dwindow()
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"QTD3D11WINDOW", NULL };
    ::RegisterClassEx(&wc);
    m_hwnd = ::CreateWindow(wc.lpszClassName, L"QT D3D11 WINDOW", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);
    return true;
}


template<class T>
bool QD3D11WINDOW<T>::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}


template<class T>
void QD3D11WINDOW<T>::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}


template<class T>
void QD3D11WINDOW<T>::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}


template<class T>
void QD3D11WINDOW<T>::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }

}

template<class T>
void QD3D11WINDOW<T>::DrawImage(wchar_t* lpszFilePath,int x,int y,int w,int h)
{
    ImVec2 im=ImGui::GetWindowPos();
    ImGui::SetWindowPos(ImVec2(x,y));
    ImGui::Image(DX11LoadTextureImageFromFile(g_pd3dDevice, (wchar_t*)lpszFilePath),
                 ImVec2(w, h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    ImGui::SetWindowPos(im);
}

template<class T>
LRESULT QD3D11WINDOW<T>::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (pd3d11_window->g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            pd3d11_window->CleanupRenderTarget();
            pd3d11_window->g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            pd3d11_window->CreateRenderTarget();
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


template<class T>
void QD3D11WINDOW<T>::DrawMain()
{
    const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
    while(!pd3d11_window->isDistory)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                pd3d11_window->isDistory = true;
        }
        if (pd3d11_window->isDistory)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("##overlay", nullptr, flags);//全屏幕绘图
        ImGui::GetStyle().AntiAliasedFill = true;
        ImGui::GetStyle().AntiAliasedLines = true;
        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

        pd3d11_window-> DrawEvent();

        ImGui::GetOverlayDrawList()->PushClipRectFullScreen();
        ImGui::End();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { pd3d11_window->clear_color.x * pd3d11_window->clear_color.w, pd3d11_window->clear_color.y * pd3d11_window->clear_color.w, pd3d11_window->clear_color.z * pd3d11_window->clear_color.w, pd3d11_window->clear_color.w };
        pd3d11_window->g_pd3dDeviceContext->OMSetRenderTargets(1, &pd3d11_window->g_mainRenderTargetView, NULL);
        pd3d11_window->g_pd3dDeviceContext->ClearRenderTargetView(pd3d11_window->g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        pd3d11_window->g_pSwapChain->Present(1, 0);
    }
}

template<class T>
ID3D11ShaderResourceView* QD3D11WINDOW<T>::DX11LoadTextureImageFromFile(ID3D11Device *pD3dDevice, wchar_t* lpszFilePath)
{

    ID3D11Texture2D * pTexture2D = NULL;
    D3D11_TEXTURE2D_DESC dec;


    HRESULT result;
    D3DX11_IMAGE_LOAD_INFO loadInfo;
    ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
    loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    loadInfo.MipLevels = D3DX11_DEFAULT;
    loadInfo.MipFilter = D3DX11_FILTER_LINEAR;

    result = D3DX11CreateTextureFromFile(pD3dDevice, lpszFilePath, &loadInfo, NULL, (ID3D11Resource**)(&pTexture2D), NULL);
    pTexture2D->GetDesc(&dec);

    if (result != S_OK)
    {
        return NULL;
    }

    ID3D11ShaderResourceView*pFontTextureView = NULL;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = dec.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    pD3dDevice->CreateShaderResourceView(pTexture2D, &srvDesc, &pFontTextureView);
    return pFontTextureView;
}




#endif // QD3D11WINDOW_HPP
