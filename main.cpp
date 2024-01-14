#include "src/memory/memory.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(mustache, handle) {

  handle.doc() = "CPython API for memory and process access.";

  pybind11::enum_<ACCESS_LEVEL>(handle,"ACCESS_LEVEL")
  .value("READ", ACCESS_LEVEL::READ_ONLY)
  .value("WRITE", ACCESS_LEVEL::READ_WRITE)
  .value("ALL", ACCESS_LEVEL::ALL_ACCESS);


  pybind11::class_<Memory>(handle, "Memory")
      .def(pybind11::init<const std::string_view, ACCESS_LEVEL>(), pybind11::arg("process_name"), pybind11::arg("access_level") = ACCESS_LEVEL::READ_ONLY)
      .def("find_signature",&Memory::findPattern, pybind11::arg("module_name"), pybind11::arg("pattern"))
      .def("get_process_id", &Memory::getProcessId, pybind11::return_value_policy::reference)
      .def("get_module_info", &Memory::getModuleInfo, pybind11::return_value_policy::reference)
      .def("get_module_base", &Memory::getModuleBase, pybind11::return_value_policy::reference)
      .def("read_ptr", &Memory::readv<uintptr_t>, pybind11::return_value_policy::reference)
      .def("read_bool", &Memory::readv<bool>)
      .def("read_float", &Memory::readv<float>)
      .def("read_str", &Memory::readString)
      .def("read_int", &Memory::readv<int>, pybind11::return_value_policy::reference);
}