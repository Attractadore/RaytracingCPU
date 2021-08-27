#include "SDLHandle.hpp"

#include <SDL2/SDL.h>

SDLHandle::SDLHandle() {
    SDL_SetMainReady();
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_Init(SDL_INIT_EVERYTHING);
}

SDLHandle::~SDLHandle() noexcept {
    SDL_Quit();
}
