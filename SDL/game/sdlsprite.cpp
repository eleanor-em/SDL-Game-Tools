#include "sdlsprite.h"

void SdlSprite::update(float delta) {
    if (!paused) {
        frame += animSpeed * delta;
    }
    if (frame >= numFrames) {
        frame = 0;
    }
}

void SdlSprite::render(int x, int y, int w, int h) {
    // find the x and y that correspond to the current frame starting from 0
    int xx = tx;
    int yy = ty;
    // wrap around as many times as we need to (tile coordinates)
    for (int c = (int)frame; c > 0; --c) {
        xx++;
        if (xx * tex.getTileWidth() >= tex.getWidth()) {
            xx = 0;
            yy++;
        }
    }
    tex.setTransforms(transforms);
    // draw the tile
    tex.renderTile(x, y, w, h, xx, yy);
}
void SdlSprite::setHorizontalFlip(bool flipped) {
    SDL_RendererFlip state = SDL_FLIP_HORIZONTAL, flip = SDL_FLIP_NONE;
    if (flipped) {
        // hacky but it's the only way
        flip = (SDL_RendererFlip) ((int)flip | (int)state);
    }
    else {
        // unset the flag
        flip = (SDL_RendererFlip) ((int)flip & ~(int)state);
    }
    transforms.flip = flip;
}
void SdlSprite::setVerticalFlip(bool flipped) {
    SDL_RendererFlip state = SDL_FLIP_VERTICAL, flip = SDL_FLIP_NONE;
    if (flipped) {
        // hacky but it's the only way
        flip = (SDL_RendererFlip) ((int)flip | (int)state);
    }
    else {
        // unset the flag
        flip = (SDL_RendererFlip) ((int)flip & ~(int)state);
    }
    transforms.flip = flip;
}
