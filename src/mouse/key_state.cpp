#include "key_state.h"
#include <SDL.h>

#if WIN32

bool hotkey::isKeyPressed(int key) {
    using GetAsyncKeyStateFn = SHORT(__stdcall *)(int);
    HMODULE user32 = LoadLibrary("User32.dll");

    assert(user32 != nullptr);

    auto pGetAsyncKeyState = reinterpret_cast<GetAsyncKeyStateFn>(GetProcAddress(user32, "GetAsyncKeyState"));
    if (pGetAsyncKeyState(key) & 0x8000)
        return true;

    FreeLibrary(user32);
    return false;

}

#else

bool hotkey::isKeyPressed(int *key) {
    if (SDL_GetKeyboardState(key))
        return true;

    return false;
}
#endif

