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


std::string generate_random_name(int len) {
 {

  std::random_device rd;
  std::mt19937 generator(rd());

  const std::string allowed_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789._-";

  std::string filename;
  std::uniform_int_distribution<int> distribution(0, allowed_chars.size() - 1);
  for (int i = 0; i < len; ++i) 
    filename += allowed_chars[distribution(generator)];


  return filename;
}
}