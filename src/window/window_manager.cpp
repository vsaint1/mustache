/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */
#include "window_manager.h"

bool WindowManager::create(const char *name_, bool monitor_size, bool vsync) {

  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
  glfwWindowHint(GLFW_FLOATING, 1);
  glfwWindowHint(GLFW_MAXIMIZED, 1);
  glfwWindowHint(GLFW_RESIZABLE, false);

  if (monitor_size) {
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();

    const auto mode = glfwGetVideoMode(monitor);
    this->m_width = mode->width;
    this->m_height = mode->height;
  } else {
    const auto window_size = get_target_window_size(name_);

    if(window_size.first == 0 || window_size.second == 0) 
      return false;
    
    this->m_width = window_size.first;
    this->m_height = window_size.second;
  }

  if (monitor_size) {
    this->m_window = glfwCreateWindow(this->m_width, this->m_height, name_, NULL, NULL);
  } else {
    this->m_window = glfwCreateWindow(this->m_width, this->m_height, RANDOM_NAME(16).c_str(), NULL, NULL);
  }

  SPDLOG_INFO("Window created, sizes: {}x{}", this->m_width, this->m_height);

  if (!this->m_window)
    return false;

  glfwMakeContextCurrent(this->m_window);
  glfwSwapInterval(vsync ? GLFW_TRUE : GLFW_FALSE);

  return true;
}

bool WindowManager::should_close() { return glfwWindowShouldClose(this->m_window); }

void WindowManager::render() {
  glfwSwapBuffers(this->m_window);
  static int display_w, display_h;

  glfwGetFramebufferSize(this->m_window, &display_w, &display_h);

  glViewport(0, 0, display_w, display_h);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(this->m_window);
}

void WindowManager::pool_events() { glfwPollEvents(); }

void WindowManager::mouse_click_through(bool enable) { glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, enable ? GLFW_TRUE : GLFW_FALSE); }

std::pair<float, float> WindowManager::get_target_window_size(const char *window_name) {

#if _WIN32
  HWND hwnd = FindWindowA(NULL, window_name);
  if (hwnd == NULL) {
    throw std::runtime_error::exception("Window not found");
    return {};
  }

  RECT rect;
  GetWindowRect(hwnd, &rect);

  SPDLOG_INFO("Window found, sizes: {}x{}", rect.right - rect.left, rect.bottom - rect.top);

  return {rect.right - rect.left, rect.bottom - rect.top};
#else
  // TODO: Implement
  return {};

#endif
}
