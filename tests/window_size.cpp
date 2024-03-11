/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */



#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../src/window/window_manager.h"

TEST_CASE("should not have null window sizes") {

    static WindowManager window;

    window.create("test");
    
    CHECK(window.get_width() != 0);
    CHECK(window.get_height() != 0);

}
