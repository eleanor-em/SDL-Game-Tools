#include <cstring>
#include <fstream>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "sdltexture.h"
#include "sdlwindow.h"
#include "sdlcore.h"
#include "core.h"

void SdlTexture::load(const char *file) {
    loaded = file;
    // load from file
    SDL_Surface *surf = IMG_Load(CWD::makeDir(file));
    if (surf == nullptr) {
        throw SdlException("IMG_Load");
    }
    // load the surface
    load(surf);
    SDL_FreeSurface(surf);
}
void SdlTexture::load(SDL_Surface *surf) {
    // load from surface
    tex = SDL_CreateTextureFromSurface(win.getRenderer(), surf);
    if (tex == nullptr) {
        throw SdlException("SDL_CreateTextureFromSurface");
    }
    
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
}

void SdlTexture::load(int w, int h) {
	tex = SDL_CreateTexture(win.getRenderer(),
							SDL_PIXELFORMAT_ARGB8888,
							SDL_TEXTUREACCESS_TARGET,
							w, h);
	if (tex == nullptr) {
		throw SdlException("SDL_CreateTexture");
	}
	renderable = true;
}

void SdlTexture::loadTiles(int xNum, int yNum) {
    // sets up the texture as a tileset
    tw = width / xNum;
    th = height / yNum;
}

void SdlTexture::write(std::ofstream &fs) const {
    fs.write((char *)&tw, sizeof(tw));
    fs.write((char *)&th, sizeof(th));
    fs.write(loaded, std::strlen(loaded));
    fs.write("\0", 1);
}
void SdlTexture::read(std::ifstream &fs) {
    // read tile information
    int data;
    fs.read((char *)&data, sizeof(data));
    tw = data;
    fs.read((char *)&data, sizeof(data));
    th = data;
    // read the string
    char *file;
    int pos = (int)fs.tellg();
    // scroll to null
    while (fs.get());
    int size = (int)fs.tellg() - pos;
    fs.seekg(pos);
    file = new char[size];
    fs.get(file, size, '\0');
    // discard null
    fs.get();
    load(file);
    delete[] file;
}

void SdlTexture::render(int x, int y, int w, int h, int sx, int sy, int sw, int sh) const {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_Rect src;
    src.x = sx;
    src.y = sy;
    src.w = sw;
    src.h = sh;
    SDL_RenderCopyEx(win.getRenderer(), tex, &src, &dest, transforms.degrees, NULL, transforms.flip);	
}

void SdlTexture::renderTile(int x, int y, int w, int h, int tx, int ty) const {
    if (tw == -1) {
        throw std::runtime_error("Tiles not loaded in renderTile");
    }
    int sx = tx * tw;
    int sy = ty * th;
    render(x, y, w, h, sx, sy, tw, th);
}

void SdlTexture::renderTileCentred(int x, int y, int w, int h, int tx, int ty) const {
    if (tw == -1) {
        throw std::runtime_error("Tiles not loaded in renderTileCentred");
    }
    int sx = tx * tw;
    int sy = ty * th;
    renderCentred(x, y, w, h, sx, sy, tw, th);
}
