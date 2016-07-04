#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sdlcore.h"
#include "sdlsys.h"

SdlSys::SdlSys() {
    // initialise everything
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw SdlException("SDL_Init");
    }
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Quit();
        throw SdlException("IMG_Init");
    }
    if (TTF_Init() != 0) {
        SDL_Quit();
        IMG_Quit();
        throw SdlException("TTF_Init");
    }
}

SdlSys::~SdlSys() {
    // clean up everything
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

void SdlSys::init() {
    // make sure an instance exists
    static SdlSys sys;
}
