/*
 *   Copyright (c) 2024 vsantos1
 *   All rights reserved.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../src/window/window_manager.h"

TEST_SUITE("Window Manager") {
  TEST_CASE("should create a window on given target") {
    static WindowManager window;

    INFO("Creating window without target process running");
    CHECK_THROWS_WITH_AS(window.create("unknown-given-process", false, true), "Window not found", std::runtime_error::exception);
  }

  TEST_CASE("should create a valid window") {
    static WindowManager window;

    INFO("Creating window with target process running");
    CHECK(window.create("mustache"));
  }

  TEST_CASE("should not create a window on given target") {
    static WindowManager window;

    INFO("Creating window with target process running");

    CHECK(window.create("AssaultCube", false, true));
  }
}
