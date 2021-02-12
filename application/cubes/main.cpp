//------------------------------------------------------------------------------
// Cubes
// main.cpp
//------------------------------------------------------------------------------

#include "Application.hpp"
#include "quetzal/common/string_util.hpp"

#include "quetzal/common/logger.hpp"

using namespace std;

//------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    hInstance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    quetzal::com_ptr<ID3D11Debug> pDebug;

    try
    {
        quetzal::log.info() << "Application" << endl;
        quetzal::cubes::Application application;
        pDebug = application.device_debug();
        quetzal::log.info() << "run" << endl;
        application.run();
        quetzal::log.info() << "end" << endl;
    }
    catch (exception& e)
    {
        MessageBox(nullptr, quetzal::to_type<wstring>(e.what()).c_str(), L"Exception", MB_OK);
    }

    OutputDebugString(L"main\n");
    pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); // ...
    OutputDebugString(L"system\n");

    quetzal::log.info() << "return" << endl;
    return 0;
}
