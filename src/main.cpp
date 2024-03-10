#include "math/math.h"
#include <stdio.h>

#if PYTHON_BINDING

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

void hello() {
  
  printf("Hello, World!\n");

}

PYBIND11_MODULE(mustache, handle) {

    handle.doc() = "OS library for memory access, Window management and overlay drawing.";

  handle.def("say_hello", &hello, "Hello, World!");
}

#endif

#if JAVA_BINDING
#include "memory/memory.h"
#include <jni.h>

extern "C" {

JNIEXPORT long JNICALL Java_com_vsantos1_getProcessId(JNIEnv *p_env, jobject jobj) {

  // TODO: just dev purpose
  long pid = 12345;
  SPDLOG_INFO("Process id: {}", pid);
  return static_cast<jlong>(pid);
}
}

#endif

#if MUSTACHE_SHARED
#include "memory/memory.h"

__declspec(dllexport) unsigned int getProcessId() {
  // TODO: just dev purpose
  long pid = 12345;
  SPDLOG_INFO("Process id: {}", pid);
  return pid;
}

#endif