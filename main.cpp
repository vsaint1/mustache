#include "src/memory/memory.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// python bindings

PYBIND11_MODULE(mustache, handle) {

  handle.doc() = "CPython API for memory and process access.";

  pybind11::class_<Memory>(handle, "Memory")
      .def(pybind11::init<const std::string_view>(), pybind11::arg("process_name"))
      .def("get_process_id", &Memory::getProcessId, pybind11::return_value_policy::reference)
      .def("get_module_data", &Memory::getModuleData, pybind11::return_value_policy::reference)
      .def("get_module_base", &Memory::getModuleBase, pybind11::return_value_policy::reference)
      .def("read_ptr", &Memory::readv<uintptr_t>, pybind11::return_value_policy::reference)
      .def("read_bool", &Memory::readv<bool>)
      .def("read_float", &Memory::readv<float>)
      .def("read_str", &Memory::read_str)
      .def("read_int", &Memory::readv<int>, pybind11::return_value_policy::reference);
}