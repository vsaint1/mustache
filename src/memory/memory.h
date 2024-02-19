#pragma once
#include "../utils/helper_macros.h"
#include <TlHelp32.h>
#include <charconv>
#include <memory>
#include <optional>
#include <string>
#include <vector>

typedef _Return_type_success_(return >= 0) long NTSTATUS;

typedef NTSTATUS(__stdcall *pNtReadVirtualMemory)(void *ProcessHandle, void *BaseAddress, void *Buffer, unsigned long NumberOfBytesToRead, unsigned long *NumberOfBytesRead);
typedef NTSTATUS(__stdcall *pNtWriteVirtualMemory)(void *Processhandle, void *BaseAddress, void *Buffer, unsigned long NumberOfBytesToWrite, unsigned long *NumberOfBytesWritten);

enum ACCESS_LEVEL {
  READ_ONLY = PROCESS_VM_READ,
  READ_WRITE = PROCESS_VM_READ  | PROCESS_VM_WRITE,
  ALL_ACCESS =  PROCESS_ALL_ACCESS

};

class Memory {
  pNtReadVirtualMemory readVirtual;
  pNtWriteVirtualMemory writeVirtual;
  void *m_handle;
  uintptr_t m_moduleBase;
  uintptr_t m_moduleSize;
  unsigned int m_processId;
  ACCESS_LEVEL  m_accessLevel;

public:
  explicit Memory(std::string_view processName,ACCESS_LEVEL accessLevel = READ_ONLY);

  unsigned int getProcessId() const { return this->m_processId; }

  uintptr_t getModuleBase() const { return this->m_moduleBase; }

  uintptr_t getModuleSize() const { return this->m_moduleSize; }

  std::pair<std::optional<uintptr_t>, std::optional<uintptr_t>> getModuleInfo(std::string_view moduleName);

  void processId(std::string_view processName);

  template <typename T> T readv(uintptr_t address) {
    T buffer;
    readVirtual(m_handle, reinterpret_cast<void *>(address), &buffer, sizeof(T), nullptr);
    return buffer;
  }

  template <typename T>
  T writev(uintptr_t address, T value){
    writeVirtual(m_handle, (void*)address, &value, sizeof(T), NULL);
    return value;
  }
  std::string readString(uintptr_t address);

  std::optional<uintptr_t> findPattern(const std::string_view &moduleName, const std::string_view &pattern);

private:
  bool read_raw(const uintptr_t address, void *buffer, uintptr_t size);
  bool runningElevated();
  bool attachProcess(ACCESS_LEVEL accessLevel);
};
