#include <exception>
#include <SDL2/SDL.h>
#include "sdlwindow.h"
#include "sdlcore.h"

SdlWindow::SdlWindow(const int x, const int y, const int width, const int height, const char *caption)
    : w(width), h(height) {
    win = SDL_CreateWindow(caption, x, y, width, height, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        throw SdlException("CreateWindow");
    }
    createRenderer();
}
SdlWindow::~SdlWindow() {
	SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}
void SdlWindow::createRenderer() {
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        throw SdlException("CreateRenderer");
    }
}

