#ifndef SDLFONT_H
#define SDLFONT_H

#include <SDL2/SDL_ttf.h>
#include "sdlwindow.h"
#include "sdltexture.h"

class SdlFont {
    public:
        SdlFont(const SdlWindow& window, const char *file, int size);
        ~SdlFont();
        SdlTexture createTexture(const char *text, SDL_Color colour);
    private:
        TTF_Font *font;
        const SdlWindow& win;
};

#endif
