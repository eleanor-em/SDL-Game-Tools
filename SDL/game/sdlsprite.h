#ifndef SDLSPRITE_H
#define SDLSPRITE_H

#include <SDL2/SDL.h>
#include "sdltexture.h"

class SdlSprite {
    public:
        SdlSprite(SdlTexture &tileset, int xTile, int yTile, int numTiles, double speed = 1)
            : tex(tileset), tx(xTile), ty(yTile), numFrames(numTiles), animSpeed(speed), frame(0), paused(false) { }
        
        void setAlpha(unsigned char alpha) { transforms.alpha = alpha; }
        void setRotation(double degrees) { transforms.degrees = degrees; }
        void setHorizontalFlip(bool flipped);
        void setVerticalFlip(bool flipped);
        void setFrame(int _frame) { frame = _frame; }
        void setSpeed(double speed) { animSpeed = speed; }
        void pause() { paused = true; }
        void play() { paused = false; }
        
        void update(float delta);
        
        void render(int x, int y) { render(x, y, tex.getTileWidth(), tex.getTileHeight()); }
        void render(int x, int y, int w, int h);
        
        void renderCentred(int x, int y) { renderCentred(x, y, tex.getTileWidth(), tex.getTileHeight()); }
        void renderCentred(int x, int y, int w, int h) { render(x - w / 2, y - h / 2, w, h); }
                      
    private:
        SdlTexture &tex;
        int tx;
        int ty;
        int numFrames;
        double animSpeed;
        double frame;
        bool paused;
        
        Transforms transforms;
};

#endif
