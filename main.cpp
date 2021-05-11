#include <QApplication>

#include <QD3D11WINDOW.hpp>
#include <QDebug>



class A:public QD3D11WINDOW<A>
{
public:
    void DrawEvent() override
    {

        D3DIMGUI::draw_text(10,10,"123",false);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(10,10),ImVec2(500,500),ImColor(0, 0, 0, 255));
        ImGui::Begin("123");
        ImGui::Text("123");
        ImGui::End();
    }
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    A* b = A::InitInstance();
    b->show();
    return a.exec();
}
