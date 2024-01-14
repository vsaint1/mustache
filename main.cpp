#include "src/math/math.h"
#include "src/memory/memory.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_MODULE(mustache, handle) {

  handle.doc() = "CPython API for memory and process access.";

  // py-math
  handle.def("rad_to_deg", &math::radToDeg, pybind11::arg("value"));
  handle.def("deg_to_rad", &math::degToRad, pybind11::arg("value"));
  handle.def("world_to_screen", &math::worldToScreen, pybind11::arg("pos"), pybind11::arg("out"), pybind11::arg("view_matrix"), pybind11::arg("window_width"), pybind11::arg("window_height"));

  pybind11::class_<math::ViewMatrix>(handle, "FViewMatrix")
      .def(pybind11::init<>())
      .def(
          "__getitem__",
          [](const math::ViewMatrix &vm, int index) {
            if (index >= 0 && index < 4) {
              return vm.matrix[index];
            }
            throw pybind11::index_error();
          },
          pybind11::is_operator());

  pybind11::class_<math::Vector>(handle, "FVector")
      .def(pybind11::init<>())
      .def(pybind11::init<float, float, float>())
      .def(pybind11::init<const math::Vector &>())
      .def("world_to_screen", &math::Vector::worldToScreen, pybind11::arg("view_matrix"), pybind11::arg("window_width"), pybind11::arg("window_height"))
      .def("distance", &math::Vector::distance, pybind11::arg("other"))
      .def("is_zero", &math::Vector::isZero);

  // py-enum
  pybind11::enum_<ACCESS_LEVEL>(handle, "ACCESS_LEVEL").value("READ", ACCESS_LEVEL::READ_ONLY).value("WRITE", ACCESS_LEVEL::READ_WRITE).value("ALL", ACCESS_LEVEL::ALL_ACCESS);

  // py-memory
  pybind11::class_<Memory>(handle, "Memory")
      .def(pybind11::init<const std::string_view, ACCESS_LEVEL>(), pybind11::arg("process_name"), pybind11::arg("access_level") = ACCESS_LEVEL::READ_ONLY)
      .def("find_signature", &Memory::findPattern, pybind11::arg("module_name"), pybind11::arg("pattern"))
      .def("get_process_id", &Memory::getProcessId, pybind11::return_value_policy::reference)
      .def("get_module_info", &Memory::getModuleInfo, pybind11::return_value_policy::reference)
      .def("get_module_base", &Memory::getModuleBase, pybind11::return_value_policy::reference)
      .def("read_view_matrix", &Memory::readv<math::ViewMatrix>)
      .def("read_x86_ptr", &Memory::readv<uint32_t>, pybind11::return_value_policy::reference)
      .def("read_ptr", &Memory::readv<uintptr_t>, pybind11::return_value_policy::reference)
      .def("read_bool", &Memory::readv<bool>)
      .def("read_float", &Memory::readv<float>)
      .def("read_str", &Memory::readString)
      .def("read_int", &Memory::readv<int>, pybind11::return_value_policy::reference);
}