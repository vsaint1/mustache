#include <pybind11/pybind11.h>

float python_sum(float arg1, float arg2) {
  return arg1 + arg2;
}

PYBIND11_MODULE(mustache,handle) {

  handle.doc() = "pybind11 example plugin";
  handle.def("test_sum",&python_sum, "A function which adds two numbers");
}