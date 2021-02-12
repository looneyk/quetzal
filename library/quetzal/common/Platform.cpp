//------------------------------------------------------------------------------
// Platform.cpp
//
//------------------------------------------------------------------------------

#include "Platform.hpp"
#include "ComException.hpp"
#include <objbase.h>

//------------------------------------------------------------------------------
quetzal::Platform::Platform()
{
    COM_CHECK(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}

//------------------------------------------------------------------------------
quetzal::Platform::~Platform()
{
    CoUninitialize();
}
