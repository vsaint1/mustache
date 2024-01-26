#include "key_state.h"

bool hotkey::isKeyPressed(unsigned long key) {
  using GetAsyncKeyStateFn = SHORT(__stdcall *)(int);
  HMODULE user32 = LoadLibrary("User32.dll");

  assert(user32 != nullptr);

  auto pGetAsyncKeyState = reinterpret_cast<GetAsyncKeyStateFn>(GetProcAddress(user32, "GetAsyncKeyState"));
  if (pGetAsyncKeyState(key) & 0x8000)
    return true;

  FreeLibrary(user32);
  return false;
}