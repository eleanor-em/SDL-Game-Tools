#include <string>
#include <fstream>
#include "tilemap.h"
#include "sdltexture.h"
#include "core.h"

void Tile::write(std::ofstream &fs) const {
    fs.write((char *)&tex, sizeof(tex));
    fs.write((char *)&tx, sizeof(tx));
    fs.write((char *)&ty, sizeof(ty));
    
    for (auto p : properties) {
        fs.write((char *)&p, sizeof(p));
    }
}

Tile *Tile::read(std::ifstream &fs, int propertyCount) {
    int tex, tx, ty;
    fs.read((char *)&tex, sizeof(tex));
    fs.read((char *)&tx, sizeof(tx));
    fs.read((char *)&ty, sizeof(ty));
    Tile *p = new Tile(tex, 0, tx, ty);
    
    unsigned int data;
    for (int i = 0; i < propertyCount; ++i) {
        fs.read((char *)&data, sizeof(data));
        p->properties.push_back(data);
    }
    
    return p;
}

TileMap::TileMap(const Game &world, int mapWidth, int mapHeight, int tileWidth, int tileHeight)
    : game(world), arraySize((mapWidth / tileWidth + 1) * (mapHeight / tileHeight + 1)),
      width(mapWidth), height(mapHeight), tw(tileWidth), th(tileHeight),
      // + 1 to account for remainders
      tileCount(0), tex(world.getWindow()) {
    // allocate tile array
    tileArray = new TileCoord *[arraySize];
    // not guaranteed to be null
    for (int i = 0; i < arraySize; ++i) {
        tileArray[i] = nullptr;
    }
    // create buffer
    tex.load(mapWidth, mapHeight);
}

TileMap::~TileMap() {
    // free everything
    for (auto p : textures) {
        delete p;
    }
    for (auto p : tiles) {
        delete p;
    }
    for (int i = 0; i < arraySize; ++i) {
        TileCoord *p = tileArray[i];
        if (p != nullptr) {
            delete p;
        }
    }
    delete[] tileArray;
}

Texture TileMap::createTexture(const char *path, unsigned int tilesX, unsigned int tilesY) {
    Texture id = textures.size();
    
    SdlTexture *p = new SdlTexture(game.getWindow());
    p->load(path);
    p->loadTiles(tilesX, tilesY);
    textures.push_back(p);
    return id;
}

TileId TileMap::createTile(Texture _tex, int tx, int ty) {
    TileId id = tiles.size();
    Tile *p = new Tile(_tex, id, tx, ty);
    tiles.push_back(p);
    // set up properties
    for (unsigned int i = 0; i < propertyMap.size(); ++i) {
        p->properties.push_back(0);
    }
    return id;
}

Property TileMap::createProperty(const char *name) {
    Property p = propertyMap.size();
    for (auto t : tiles) {
        t->properties.push_back(0);
    }
    propertyMap.emplace(name, p);
    return p;
}

void TileMap::addTile(TileId id, int x, int y) {
    int tx = x / tw;
    int ty = y / th;
    TileCoord * &p = tileArray[tx * (height / th + 1) + ty];
    if (p != nullptr) {
        delete p;
    }
    p = new TileCoord;
    p->id = id;
    p->x = x;
    p->y = y;
    
    ++tileCount;
}

unsigned int TileMap::getPropertyAt(Property property, int x, int y) const {
    int tx = x / tw;
    int ty = y / th;
    TileCoord *p = getTileAt(tx, ty);
    if (p == nullptr) {
        return 0;
    }
    return tiles[p->id]->getProperty(property);
}

unsigned int TileMap::maxPropertyIn(Property property, const OrientedBB &box) const {
	unsigned int val = 0;
	unsigned int temp = getPropertyAt(property, (int)(box.getCentre().x - box.getHalfWidth().x), (int)(box.getCentre().y));
	if (temp > val) {
		val = temp;
	}
	temp = getPropertyAt(property, (int)(box.getCentre().x + box.getHalfWidth().x), (int)(box.getCentre().y));
	if (temp > val) {
		val = temp;
	}
	temp = getPropertyAt(property, (int)(box.getCentre().x), (int)(box.getCentre().y - box.getHalfWidth().y));
	if (temp > val) {
		val = temp;
	}
	temp = getPropertyAt(property, (int)(box.getCentre().x), (int)(box.getCentre().y + box.getHalfWidth().y));
	if (temp > val) {
		val = temp;
	}
	return val;
}

void TileMap::renderToBuffer() {
	tex.clear();
	tex.setRender();
	render();
	tex.resetRender();
}

void TileMap::renderFromBuffer(float x, float y, float xoff, float yoff) {
	tex.render((int)x, (int)y, (int)(width - xoff), (int)(height - yoff), (int)xoff, (int)yoff, (int)(width - xoff), (int)(height - yoff));
}

void TileMap::render(float x, float y, float tx, float ty, float w, float h) {
    for (int xx = (int)tx; xx < (int)(tx + w) / tw; ++xx) {
        for (int yy = (int)ty; yy < (int)(ty + h) / th; ++yy) {
            auto p = getTileAt(xx, yy);
            if (p != nullptr) {
                auto q = tiles[p->id];
                textures[q->tex]->renderTile((int)(p->x - tx * tw + x), (int)(p->y - ty * th + y), q->tx, q->ty);
            }
        }
    }
}


void TileMap::save(const char *dest) const {
    std::ofstream file(dest, std::ios::out | std::ios::binary);
    // magic number
    file.write("TM", 2);
    int ver = version;
    file.write((char *)&ver, sizeof(ver));
    file.write((char *)&width, sizeof(width));
    file.write((char *)&height, sizeof(height));
    file.write((char *)&tw, sizeof(tw));
    file.write((char *)&th, sizeof(th));
    
    int propertyCount = propertyMap.size();
    file.write((char *)&propertyCount, sizeof(propertyCount));
    
    // save textures
    int count = textures.size();
    file.write((char *)&count, sizeof(count));
    for (auto p : textures) {
        p->write(file);
    }
    
    // save tile prototypes
    count = tiles.size();
    file.write((char *)&count, sizeof(count));
    for (auto p : tiles) {
        p->write(file);
    }
    
    // save properties
    for (auto p : propertyMap) {
        file.write(p.first.c_str(), p.first.length());
        file.write("\0", 1);
    }
    
    // save tiles
    file.write((char *)&tileCount, sizeof(tileCount));
    for (int i = 0; i < arraySize; ++i) {
        auto p = tileArray[i];
        if (p != nullptr) {
            file.write((char *)p, sizeof(*p));
        }
    }
    file.close();
}

void TileMap::load(const char *src) {
    std::ifstream file(CWD::makeDir(src), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error((std::string("Error opening file ") + src).c_str());
	}
    // check some things
    int data;
	if (file.get() != 'T' || file.get() != 'M') {
        throw std::runtime_error("Incorrect file format");
    }
    if ((file.read((char *)&data, sizeof(data)), data != version)) {
        throw std::runtime_error("Incorrect format version");
    }
    if ((file.read((char *)&data, sizeof(data)), data != width)
        || (file.read((char *)&data, sizeof(data)), data != height)
        || (file.read((char *)&data, sizeof(data)), data != tw)
        || (file.read((char *)&data, sizeof(data)), data != th)) {
        throw std::runtime_error("Map size does not match the saved values");
    }
    
    int propertyCount;
    file.read((char *)&propertyCount, sizeof(propertyCount));
    
    // load textures
    file.read((char *)&data, sizeof(data));
    for (int i = 0; i < data; ++i) {
        SdlTexture *p = new SdlTexture(game.getWindow());
        p->read(file);
        textures.push_back(p);
    }
    
    // load tile prototypes
    file.read((char *)&data, sizeof(data));
    for (int i = 0; i < data; ++i) {
        auto p = Tile::read(file, propertyCount);
        p->id = i;
        tiles.push_back(p);
    }
    
    // load properties
    for (int i = 0; i < propertyCount; ++i) {
        // read the string
        char *str;
        int pos = (int)file.tellg();
        // scroll to null
        while (file.get());
        int size = (int)file.tellg() - pos;
        file.seekg(pos);
        str = new char[size];
        file.get(str, size, '\0');
        // discard null
        file.get();
        propertyMap.emplace(str, i);
        delete[] str;
    }
    
    // load tiles
    file.read((char *)&data, sizeof(data));
    TileCoord *p = new TileCoord;
    for (int i = 0; i < data; ++i) {
        file.read((char *)p, sizeof(*p));
        addTile(p->id, p->x, p->y);
    }
    delete p;
    
    file.close();
}
