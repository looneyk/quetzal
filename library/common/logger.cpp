//------------------------------------------------------------------------------
// logger.cpp
//------------------------------------------------------------------------------

#include "logger.hpp"

using namespace std;

quetzal::Log<ofstream> quetzal::log(LogLevel::All, log_path + "\\q.txt");
