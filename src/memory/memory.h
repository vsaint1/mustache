#pragma once
#include <optional>
#include <string_view>

typedef unsigned long ULONG;
typedef unsigned long* PULONG;
typedef _Return_type_success_(return >= 0) long NTSTATUS;

typedef NTSTATUS(__stdcall *pNtReadVirtualMemory)(void* ProcessHandle, void* BaseAddress,  void* Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(__stdcall *pNtWriteVirtualMemory)( void* Processhandle,  void* BaseAddress,  void* Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);

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
    NTSTATUS(readVirtual(m_handle, reinterpret_cast<void *>(address), &buffer, sizeof(T), nullptr));
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
  }

private:
  bool attachProcess();
};
