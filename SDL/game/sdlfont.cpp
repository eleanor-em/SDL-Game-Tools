#include <SDL2/SDL_ttf.h>
#include "sdlcore.h"
#include "sdlfont.h"
#include "sdlwindow.h"
#include "sdltexture.h"

SdlFont::SdlFont(const SdlWindow& window, const char *file, int size)
    : win(window) {
    font = TTF_OpenFont(file, size);
    if (font == nullptr) {
        throw SdlException("OpenFont");
    }
}
SdlFont::~SdlFont() {
    TTF_CloseFont(font);
}

SdlTexture SdlFont::createTexture(const char *text, SDL_Color colour) {
    // render the texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, colour);
    if (surf == nullptr) {
        throw SdlException("RenderText_Blended");
    }
    // create the texture
    SdlTexture tex(win);
    tex.load(surf);
    SDL_FreeSurface(surf);
    return tex;
}
