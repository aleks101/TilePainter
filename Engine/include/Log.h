#pragma once

#include <iostream>

#ifdef _DEBUG
#define LOG(msg) std::cout << msg
#define LOGln(msg) std::cout << msg << std::endl
#else
#define LOG(msg)
#define LOGln(msg)
#endif // !DEBUG