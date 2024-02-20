#ifndef MOUSE_H
#define MOUSE_H

#define MOUSE_PRESS 1
#define MOUSE_RELEASE 2
#define MOUSE_MOVE 3
#define MOUSE_CLICK 4

#if WIN32

#include <Windows.h>

#endif

#include <SDL.h>

class Mouse {

    int m_mouseX;
    int m_mouseY;

public:


    explicit Mouse() {
        SDL_GetGlobalMouseState(&m_mouseX, &m_mouseY);
    }

    void move(unsigned long x, unsigned long y) {
#if WIN32

        SDL_MouseMotionEvent(SDL_MOUSEMOTION, 0, 0, 0, 0, x, y, 0, 0);

#endif
        mouse_event(0x0001, x, y, 0, 0);
    }

    void click() {
#if WIN32

        mouse_event(0x0002, 0, 0, 0, 0);
        mouse_event(0x0004, 0, 0, 0, 0);

#endif
        SDL_MouseMotionEvent(SDL_BUTTON_LMASK, 0, 0, 0, 0, 0, 0, 0, 0);
    }

    int getMouseX() const {
        return m_mouseX;
    }

    int getMouseY() const {
        return m_mouseY;
    }


};

#endif