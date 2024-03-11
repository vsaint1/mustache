/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */
#ifndef WINDOW_WINDOW_MANAGER_H_
#define WINDOW_WINDOW_MANAGER_H_
#include "GLFW/glfw3.h"
#include <iostream>
#include "../utils/helper_macros.h"
#include <stdexcept>
#if _WIN32
#include <windows.h>
#endif

class WindowManager {
  int m_width;
  int m_height;
  GLFWwindow *m_window;
  bool m_transparent = false;

public:
  bool create(const char *name_, bool monitor_size = true, bool vsync = true);

  bool should_close();

  void render();

  void pool_events();

  int get_height() const { return m_height; }

  int get_width() const { return m_width; }

  GLFWwindow *get_window() const { return m_window; }

  void mouse_click_through(bool enable);

private:
 std::pair<float,float> get_target_window_size(const char* window_name);
};

#endif // WINDOW_WINDOW_MANAGER_H_
