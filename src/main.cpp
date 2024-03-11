#include "math/math.h"
#include "utils/helper_macros.h"
#include "window/window_manager.h"

MUSTACHE_API bool create_window(const char *name, bool monitor_size, bool vsync) {
  static WindowManager window;
  bool result = window.create(name, monitor_size, vsync);
  return result;
}