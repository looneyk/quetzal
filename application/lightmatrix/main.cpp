//------------------------------------------------------------------------------
// Light Matrix
// main.cpp
//------------------------------------------------------------------------------

#include "Application.hpp"
#include "quetzal/common/string_util.hpp"

using namespace std;

//------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    hInstance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    try
    {
        quetzal::lightmatrix::Application application;
        application.run();
    }
    catch (exception& e)
    {
        MessageBox(nullptr, quetzal::to_type<wstring>(e.what()).c_str(), L"Exception", MB_OK);
    }

    return 0;
}
