#ifndef ENTITY_H
#define ENTITY_H

#include "orientedbb.h"
#include "game.h"
#include "vector2f.h"

class GameState;
class Game;

class Entity {
    public:
        Entity(Game &world)
            : game(world), persist(false) { }
        virtual ~Entity() { };

        virtual void update(float delta) { (void)delta; }
        virtual void handleCollision(const Manifold &m, const Entity &otherId) { (void)m; (void)otherId; }
        virtual void render() { }
        virtual OrientedBB getPosition() const = 0;
        virtual unsigned int getID() const = 0;
        
        bool getPersist() const { return persist; }
        
        friend class GameState;
        
    protected:
        Game &game;
        GameState &getState() { return *_state; }
        bool persist;
        
    private:
    	GameState *_state;
};

#endif
