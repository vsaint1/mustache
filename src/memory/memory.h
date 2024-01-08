#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <optional>
#include <string_view>

typedef NTSTATUS(WINAPI *pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(WINAPI *pNtWriteVirtualMemory)(HANDLE Processhandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);

class Memory {
  pNtReadVirtualMemory readVirtual;
  pNtWriteVirtualMemory writeVirtual;
  void *m_handle;
  uintptr_t m_moduleBase;
  uintptr_t m_moduleSize;
  unsigned int m_processId;

public:
  explicit Memory(std::string_view processName);

  unsigned int getProcessId() const { return this->m_processId; }

  uintptr_t getModuleBase() const { return this->m_moduleBase; }

  uintptr_t getModuleSize() const { return this->m_moduleSize; }

  std::optional<std::pair<uintptr_t, uintptr_t>> getModuleData(std::string_view moduleName);

  void processId(std::string_view processName);

  template <typename T> T readv(const uintptr_t address) {
    T buffer;
    NTSTATUS(readVirtual(m_handle, reinterpret_cast<void *>(address), &buffer, sizeof(T), nullptr));
    return buffer;
  }

private:
  bool attachProcess();
};
