#pragma once

#include "../utils/helper_macros.h"
#include <cassert>

namespace hotkey {

#if WIN32

    bool isKeyPressed(int key);

#else

    bool isKeyPressed(int *key);
#endif

};