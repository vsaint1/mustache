#include "src/memory/memory.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

float python_sum(float arg1, float arg2) { return arg1 + arg2; }

PYBIND11_MODULE(mustache, handle) {

  handle.doc() = "pybind11 example plugin";
  handle.def("test_sum", &python_sum, "A function which adds two numbers");

  pybind11::class_<Memory>(handle,"Memory")
      .def(pybind11::init<const std::string_view>(), pybind11::arg("process_name"))
      .def("get_process_id", &Memory::getProcessId,pybind11::return_value_policy::reference);

}