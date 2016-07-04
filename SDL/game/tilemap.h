#ifndef TILEMAP_H
#define TILEMAP_H

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "game.h"
#include "sdltexture.h"

typedef unsigned int Property;
typedef unsigned int TileId;
typedef unsigned int Texture;

class Tile {
    public:
        void setProperty(Property property, unsigned int value) { properties[property] = value; }            
        unsigned int getProperty(Property property) const { return properties.at(property); }
        
        TileId getID() const { return id; }
        
        friend class TileMap;
            
    private:
        Texture tex;
        TileId id;
        int tx;
        int ty;
        std::vector<unsigned int> properties;
                
        Tile(Texture _tex, TileId _id, int _tx, int _ty)
            : tex(_tex), id(_id), tx(_tx), ty(_ty) { }    
            
        void write(std::ofstream &fs) const;
        static Tile *read(std::ifstream &fs, int propertyCount);
};

class TileMap {
    public:
        TileMap(const Game &world, int mapWidth, int mapHeight, int tileWidth, int tileHeight);
        ~TileMap();
        
        Property getPropertyID(const char *name) const { return propertyMap.at(name); }
        unsigned int getPropertyAt(Property property, int x, int y) const;
		unsigned int maxPropertyIn(Property property, const OrientedBB &box) const;
        Tile &getTile(TileId id) const { return *tiles[id]; }
        int getTileWidth() const { return tw; }
        int getTileHeight() const { return th; }
        
        Texture createTexture(const char *path, unsigned int tilesX, unsigned int tilesY);
        TileId createTile(Texture tex, int tx, int ty); 
        Property createProperty(const char *name);
        
        void addTile(Tile &tile, int x, int y) { addTile(tile.id, x, y); }
        void addTile(TileId id, int x, int y);
        
        void load(const char *src);
        void save(const char *dest) const;
        
        void renderToBuffer();
		void clearBuffer() const { tex.clear(); }
		void renderFromBuffer(float x, float y, float xoff, float yoff);
        void render() { render(0, 0); }
        void render(float x, float y) { render(x, y, 0, 0); }
        void render(float x, float y, float tx, float ty) { render(x, y, tx, ty, width - tx * tw, height - ty * th); }
        void render(float x, float y, float tx, float ty, float width, float height);
    
    private:
        struct TileCoord {
            TileId id;
            int x;
            int y;
        };
                        
        TileMap(const TileMap &copy)
            : game(copy.game), arraySize(-1), tex(game.getWindow()) { }
        void operator =(const TileMap &rhs) { (void)rhs; }
        
        TileCoord *getTileAt(int x, int y) const {  return tileArray[x * (height / th + 1) + y]; }
        
        const Game &game;
        const int arraySize;
        TileCoord **tileArray;
        
        int width;
        int height;
        int tw;
        int th;
        
        int tileCount;
        
        SdlTexture tex;
        
        std::map<std::string, Property> propertyMap;
        std::vector<SdlTexture *> textures;
        std::vector<Tile *> tiles;
                       
        static constexpr int version = 3;
};

#endif
