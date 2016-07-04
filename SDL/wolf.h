#ifndef WOLF_H
#define WOLF_H
#include "game/engine.h"
#include "game/orientedbb.h"

class Wolf: public Entity {
    public:
        Wolf(Game &game)
            : Entity(game), tex(game.getWindow()), up(tex, 0, 0, 4, 0.05f), right(tex, 0, 1, 4, 0.05f), down(tex, 0, 2, 4, 0.05f),
              left(tex, 0, 3, 4, 0.05f), sprite(&up), position(36 * 7, 36 * 8)
            { tex.load("ts.png"); tex.loadTiles(4, 4); }
        
        OrientedBB getPosition() const override { return OrientedBB(position.x, position.y, 15, 36); }
        void setPosition(float xx, float yy) { position.x = xx; position.y = yy; }
        void faceDown() { sprite = &down; }
        static constexpr int ID = 0;      
        unsigned int getID() const override { return ID; }
        
        void update(float delta) override;
                                             
        void render() override { Vector2f pos = game.getScreen().transform(position); sprite->renderCentred((int)pos.x, (int)pos.y, 48, 75); }
        
    private:
        SdlTexture tex;
        SdlSprite up, right, down, left;
        SdlSprite *sprite;
        Vector2f position;
        
        static constexpr float SPEED = 4;
        
        void changeSprite(SdlSprite &spr) { sprite = &spr; sprite->play(); }
};

#endif
