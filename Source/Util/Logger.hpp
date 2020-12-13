#ifndef __UTIL_LOGGER
#define __UTIL_LOGGER

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#define LOG_WARN (std::cerr << "[WARN] ")
#define LOG_INFO (std::cout << "[INFO] ")
#define LOG_ERROR (std::cerr << "[ERROR] ")
#endif
