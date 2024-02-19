#pragma once

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"


#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#include <spdlog/spdlog.h>

#endif

bool compare(std::string_view str1, std::string_view str2);

#define EQUALS_IGNORE_CASE(str1, str2) compare(str1,str2)
