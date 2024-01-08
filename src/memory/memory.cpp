#include "memory.h"

#include <stdexcept>

Memory::Memory(const std::string_view processName) {

  this->readVirtual = reinterpret_cast<pNtReadVirtualMemory>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory"));
  this->writeVirtual = reinterpret_cast<pNtWriteVirtualMemory>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory"));

  this->processId(processName);

  if (!this->attachProcess())
    throw std::runtime_error("Failed to attach to process");

}

void Memory::processId(std::string_view processName) {
  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);
  void *hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, this->m_processId);

  if (hSnapshot != INVALID_HANDLE_VALUE) {
    while (Process32Next(hSnapshot, &pe32)) {

      if (processName.compare(pe32.szExeFile) == 0) {
        CloseHandle(hSnapshot);
        this->m_processId = pe32.th32ProcessID;
      }
    }
  }

  if (hSnapshot)
    CloseHandle(hSnapshot);
}

bool Memory::attachProcess() {

  if (!this->m_processId)
    return false;

  void *handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->m_processId);

  if (!handle)
    return false;

  this->m_handle = handle;

  return true;
}

std::optional<std::pair<uintptr_t, uintptr_t>> Memory::getModuleData(std::string_view moduleName) {
  MODULEENTRY32 me32;
  me32.dwSize = sizeof(MODULEENTRY32);

  void *hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->m_processId);

  if (hSnapshot != INVALID_HANDLE_VALUE) {
    while (Module32Next(hSnapshot, &me32)) {
      if (moduleName.compare(me32.szModule) == 0) {
        this->m_moduleBase = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
        this->m_moduleSize = me32.modBaseSize;
        CloseHandle(hSnapshot);
        return std::make_pair(this->m_moduleBase, this->m_moduleSize);
      }
    }
  }

  if (hSnapshot)
    CloseHandle(hSnapshot);

  return {};
}
