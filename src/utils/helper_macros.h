#ifndef UTILS_HELPER_MACROS_H_
#define UTILS_HELPER_MACROS_H_

#define WIN32_LEAN_AND_MEAN
#include <random>
#include <string>

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

std::string generate_random_name(int len);

#define EQUALS_IGNORE_CASE(str1, str2) compare(str1,str2)

#define RANDOM_NAME(len) generate_random_name(len)

#define MUSTACHE_API __declspec(dllexport)

#endif // UTILS_HELPER_MACROS_H_
