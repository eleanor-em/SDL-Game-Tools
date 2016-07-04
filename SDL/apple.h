#ifndef APPLE_H
#define APPLE_H
#include "wolf.h"
#include "game/engine.h"
#include "game/orientedbb.h"
#include <cstdlib>

class Apple: public Entity {
    public:
        Apple(Game &game)
            : Entity(game), tex(game.getWindow()), sprite(tex, 0, 0, 1, 0)
            { tex.load("apple.png"); tex.loadTiles(1, 1); persist = true; }

        OrientedBB getPosition() const override { return OrientedBB(position.x, position.y, 12, 12); }
        virtual void handleCollision(const Manifold &m, const Entity &other) override {
        	(void)m;
        	if (other.getID() == Wolf::ID) {
		    	getState().removeEntity(this);
		    }
        }
        void setPosition(float xx, float yy) { position.x = xx; position.y = yy; }
        static constexpr int ID = 1;
        unsigned int getID() const override { return ID; }
        
        void update(float delta) override { (void)delta; }
                                             
        void render() override { Vector2f pos = game.getScreen().transform(position); sprite.renderCentred((int)pos.x, (int)pos.y); }
        
    private:
        SdlTexture tex;
        SdlSprite sprite;
        Vector2f position;
};

#endif
