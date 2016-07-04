#ifndef SDLWINDOW_H
#define SDLWINDOW_H
#include <SDL2/SDL.h>

class SdlWindow {
    public:
        SdlWindow(const int x, const int y, const int width, const int height, const char *caption);
        SdlWindow(const SdlWindow &copy)
            : win(copy.win), ren(copy.ren), w(copy.w), h(copy.h) { }
        SdlWindow operator =(const SdlWindow &rhs) { return SdlWindow(rhs); }
        ~SdlWindow();
        
        SDL_Renderer *getRenderer() const { return ren; }
        int getWidth() const { return w; }
        int getHeight() const { return h; }
        
        void clear() const { SDL_RenderClear(ren); }
        void present() const { SDL_RenderPresent(ren); }
        
    private:
        SDL_Window *win;
        SDL_Renderer *ren;
        const int w;
        const int h;
        
        void createRenderer();
};

#endif
