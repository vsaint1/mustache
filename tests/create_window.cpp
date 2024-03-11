/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */



#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../src/window/window_manager.h"



TEST_CASE("should create a window") {

    static WindowManager window;

    CHECK(window.create("hello") == true);
    
}
