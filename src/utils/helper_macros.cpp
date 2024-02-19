/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */
#include "helper_macros.h"

bool compare(std::string_view str1, std::string_view str2) {
    return (str1.size() == str2.size()) &&
           std::equal(str1.begin(), str1.end(), str2.begin(), [](const char a, const char b) {
               return std::tolower(a) == std::tolower(b);
           });
}