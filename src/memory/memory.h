#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <optional>
#include <string_view>


class Memory {

  uintptr_t m_moduleBase;
  uintptr_t m_moduleSize;
  unsigned int m_processId;

public:
  explicit Memory(const std::string_view processName) { this->processId(processName); }

  unsigned int getProcessId() const { return this->m_processId; }

  uintptr_t getModuleBase() const { return this->m_moduleBase; }

  uintptr_t getModuleSize() const { return this->m_moduleSize; }

private:
  std::optional<std::pair<uintptr_t, uintptr_t>> getModuleData(std::string_view moduleName);

  void processId(std::string_view processName);
};
