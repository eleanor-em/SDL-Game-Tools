#ifndef SCREEN_H
#define SCREEN_H
#include "game.h"
#include "tilemap.h"
#include "orientedbb.h"
#include "entity.h"

class Entity;

class Screen {
    public:
        static constexpr int TOLERANCE = 50;
        
        void setTarget(Entity *target) { entity = target; }
        void setPos(float _x, float _y) { x = _x; y = _y; }
        
        float getLeft() const { return x; }
        float getRight() const { return x + width; }
        float getTop() const { return y; }
        float getBottom() const { return y + height; }
        
        void update() { if (entity != nullptr) {
        	auto b = entity->getPosition(); x = b.getCentre().x - width / 2; y = b.getCentre().y - height / 2; }
            if (x < 0) { x = 0; }
            if (y < 0) { y = 0; }
            if (x + width > maxWidth) { x = maxWidth - width; }
            if (y + height > maxHeight) { y = maxHeight - height; }
        }
        
        void renderTileMap(TileMap &tm) { int w = tm.getTileWidth(), h = tm.getTileHeight(); tm.render(0, 0, x / w, y / h); }
        void renderBufferedTileMap(TileMap &tm) { tm.renderFromBuffer(0, 0, x, y); }
        
        bool isOnScreen(Vector2f p) {
            return (p.x > x - TOLERANCE
                    && p.y > y - TOLERANCE
                    && p.x < x + TOLERANCE + width
                    && p.y < y + TOLERANCE + height);
        }
        
        Vector2f transform(Vector2f pos) { return Vector2f(pos.x - x, pos.y - y); }
        
        friend class Game;
        
    private:
        Screen (): entity(nullptr) { }
        
        float x;
        float y;
        float width;
        float maxWidth;
        float height;
        float maxHeight;
        
        Entity *entity;
};
#endif
