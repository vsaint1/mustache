#ifndef UTILS_HELPER_MACROS_H_
#define UTILS_HELPER_MACROS_H_

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#define BUILD_TYPE "Build Type: [DEBUG], at: " ## __TIMESTAMP__
#include <spdlog/spdlog.h>
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#define BUILD_TYPE "Build Type: [RELEASE], at: " ## __TIMESTAMP__
#include <spdlog/spdlog.h>
#endif

#define GLSL_VERSION "#version 130"

bool compare(std::string_view str1, std::string_view str2);

#define EQUALS_IGNORE_CASE(str1, str2) compare(str1,str2)
#endif // UTILS_HELPER_MACROS_H_
