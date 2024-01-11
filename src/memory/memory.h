#pragma once
#define WIN32_LEAN_AND_MEAN
#include <TlHelp32.h>
#include <Windows.h>
#include <optional>
#include <string_view>

typedef _Return_type_success_(return >= 0) long NTSTATUS;

typedef NTSTATUS(__stdcall *pNtReadVirtualMemory)(void *ProcessHandle, void *BaseAddress, void *Buffer, unsigned long NumberOfBytesToRead, unsigned long *NumberOfBytesRead);
typedef NTSTATUS(__stdcall *pNtWriteVirtualMemory)(void *Processhandle, void *BaseAddress, void *Buffer, unsigned long NumberOfBytesToWrite, unsigned long *NumberOfBytesWritten);

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

  template <typename T> T readv(uintptr_t address) {
    T buffer;
    readVirtual(m_handle, reinterpret_cast<void *>(address), &buffer, sizeof(T), nullptr);
    return buffer;
  }

  std::string read_str(uintptr_t address) {
    std::string str;
    char c;

    do {
      c = this->readv<char>(address);
      str += c;
      address++;
    } while (c != '\0');

    return str;
  }

private:
  bool attachProcess();
};
