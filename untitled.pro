QT += core gui widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        imgui/imgui.cpp \
        imgui/imgui_demo.cpp \
        imgui/imgui_draw.cpp \
        imgui/imgui_impl_dx11.cpp \
        imgui/imgui_impl_win32.cpp \
        imgui/imgui_tables.cpp \
        imgui/imgui_widgets.cpp \
        main.cpp

INCLUDEPATH += ./imgui/    \
               ./direct/

CONFIG += win_32

CONFIG(win_32)
{
    LIBS +=E:/DX_SDK/Lib/x86/d3d11.lib
    LIBS +=E:/DX_SDK/Lib/x86/d3dcompiler.lib
    LIBS +=E:/DX_SDK/Lib/x86/dxgi.lib
    LIBS +=E:/DX_SDK/Lib/x86/d3dx11.lib
    LIBS += -limm32 -lgdi32 -ldwmapi
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QD3D11WINDOW.hpp
