#ifndef SDLTEXTURE_H
#define SDLTEXTURE_H

#include <fstream>
#include <SDL2/SDL.h>
#include "sdlwindow.h"
#include "sdlcore.h"

struct Transforms {
    Transforms()
        : alpha(255), degrees(0), flip(SDL_FLIP_NONE) { }
    unsigned char alpha;
    double degrees;
    SDL_RendererFlip flip;
};
        
class SdlTexture {
    public:
        SdlTexture(const SdlWindow& window)
            : win(window), tw(-1), th(-1), renderable(false) { }
        ~SdlTexture()  {
            SDL_DestroyTexture(tex);
        }
                
        void setTransforms(Transforms transform) { transforms = transform; SDL_SetTextureAlphaMod(tex, transform.alpha); }
        
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getTileWidth() const { return tw; }
        int getTileHeight() const { return th; }
            
        void load(const char *file);
        void load(SDL_Surface *surf);
        void load(int w, int h);
        
        void loadTiles(int xNum, int yNum);
        
        void write(std::ofstream &fs) const;
        void read(std::ifstream &fs);
        
        void setRender() const { if (renderable && SDL_SetRenderTarget(win.getRenderer(), tex) != 0) { throw SdlException("SDL_SetRenderTarget"); } }
		void clear() const { setRender();
							 SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
							 SDL_SetRenderDrawColor(win.getRenderer(), 0, 0, 0, 0);
							 SDL_RenderClear(win.getRenderer());
							 resetRender(); }
        void resetRender() const { if (SDL_SetRenderTarget(win.getRenderer(), NULL) != 0) { throw SdlException("SDL_SetRenderTarget"); } }
        
        void render(int x, int y) const { render(x, y, width, height); }
        void render(int x, int y, int w, int h) const { render(x, y, w, h, 0, 0, width, height); }
        void render(int x, int y, int sx, int sy, int sw, int sh) const { render(x, y, sw, sh, sx, sy, sw, sh); }
        void render(int x, int y, int w, int h, int sx, int sy, int sw, int sh) const;
        void renderCentred(int x, int y) const { renderCentred(x, y, width, height); }
        void renderCentred(int x, int y, int w, int h) const { renderCentred(x, y, w, h, 0, 0, width, height); }
        void renderCentred(int x, int y, int sx, int sy, int sw, int sh) const { renderCentred(x, y, sw, sh, sx, sy, sw, sh); }
        void renderCentred(int x, int y, int w, int h, int sx, int sy, int sw, int sh) const {
            render(x - w / 2, y - h / 2, sw, sh, sx, sy, sw, sh);
        }
        
        void renderTile(int x, int y, int tx, int ty) const { renderTile(x, y, tw, th, tx, ty); }
        void renderTile(int x, int y, int w, int h, int tx, int ty) const;
        
        void renderTileCentred(int x, int y, int tx, int ty) const { renderTileCentred(x, y, tw, th, tx, ty); }
        void renderTileCentred(int x, int y, int w, int h, int tx, int ty) const;
                
    private:
        const SdlWindow& win;
        SDL_Texture *tex;
        int width;
        int height;
        int tw;
        int th;
        
        bool renderable;
        
        const char *loaded;
        
        Transforms transforms;
};

#endif
