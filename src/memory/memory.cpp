#include "memory.h"

bool Memory::runningElevated() {
  bool elevated = false;
  void *token = 0;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
    TOKEN_ELEVATION token_elevation;
    DWORD c_size = sizeof(TOKEN_ELEVATION);
    if (GetTokenInformation(token, TokenElevation, &token_elevation, sizeof(token_elevation), &c_size)) {
      elevated = token_elevation.TokenIsElevated;
    }
  }

  if (token)
    CloseHandle(token);

  return elevated;
}

Memory::Memory(const std::string_view processName, ACCESS_LEVEL accessLevel) {

  this->readVirtual = reinterpret_cast<pNtReadVirtualMemory>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory"));
  this->writeVirtual = reinterpret_cast<pNtWriteVirtualMemory>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory"));

  if (!this->runningElevated())
    SPDLOG_WARN("Running without ROOT privileges");

  this->processId(processName);

  if (!this->attachProcess(accessLevel)) {
    SPDLOG_ERROR("Failed while trying to attach on process");
    // TODO: change to SDL SDL_ShowSimpleMessageBox
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Failed to launch, the PROCESS_NAME[%s] isnt running.", processName);
    MessageBoxA(0, buffer, "Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
  }

  SPDLOG_INFO("Attached to process");
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

std::string Memory::readString(uintptr_t address) {
  std::string str;
  char c;

  do {
    c = this->readv<char>(address);
    str += c;
    address++;
  } while (c != '\0');

  return str;
}

std::optional<uintptr_t> Memory::findPattern(const std::string_view &moduleName, const std::string_view &pattern) {
  {
    constexpr auto pattern_to_bytes = [](const std::string_view &pattern) {
      std::vector<int32_t> bytes;

      for (auto i{0}; i < pattern.size(); ++i) {
        switch (pattern[i]) {
        case '?':
          bytes.push_back(-1);
          break;

        case ' ':
          break;

        default: {
          if (i + 1 < pattern.size()) {
            auto value{0};

            if (const auto [ptr, ec] = std::from_chars(pattern.data() + i, pattern.data() + i + 2, value, 16); ec == std::errc()) {
              bytes.push_back(value);
              ++i;
            }
          }

          break;
        }
        }
      }

      return bytes;
    };

    const auto [module_base, module_size] = this->getModuleInfo(moduleName);

    if (!module_base.has_value() || !module_size.has_value())
      return {};

    const auto module_data = std::make_unique<uint8_t[]>(module_size.value());

    if (!this->read_raw(module_base.value(), module_data.get(), module_size.value()))
      return {};

    const auto pattern_bytes = pattern_to_bytes(pattern);
    for (auto i{0}; i < module_size.value() - pattern.size(); ++i) {
      auto found{true};

      for (auto j{0}; j < pattern_bytes.size(); ++j) {
        if (module_data[i + j] != pattern_bytes[j] && pattern_bytes[j] != -1) {
          found = false;
          break;
        }
      }

      if (found)
        return module_base.value() + i;
    }

    return {};
  }
}
bool Memory::read_raw(const uintptr_t address, void *buffer, uintptr_t size) {
  this->readVirtual(this->m_handle, reinterpret_cast<void *>(address), buffer, size, nullptr);
  return true;
}

bool Memory::attachProcess(ACCESS_LEVEL accessLevel) {

  if (!this->m_processId)
    return false;

  switch (accessLevel) {
  case ACCESS_LEVEL::READ_ONLY:
    this->m_accessLevel = ACCESS_LEVEL::READ_ONLY;
  case ACCESS_LEVEL::READ_WRITE:
    this->m_accessLevel = ACCESS_LEVEL::READ_WRITE;
  case ACCESS_LEVEL::ALL_ACCESS:
    this->m_accessLevel = ACCESS_LEVEL::ALL_ACCESS;
  default:
    this->m_accessLevel = ACCESS_LEVEL::READ_ONLY;
  }

  void *handle = OpenProcess(this->m_accessLevel, FALSE, this->m_processId);

  if (!handle)
    return false;

  this->m_handle = handle;

  SPDLOG_INFO("Attached on pid {}, handle {} with accessLevel {} ", this->m_processId, this->m_handle, this->m_accessLevel);

  return true;
}

std::pair<std::optional<uintptr_t>, std::optional<uintptr_t>> Memory::getModuleInfo(std::string_view moduleName) {
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
