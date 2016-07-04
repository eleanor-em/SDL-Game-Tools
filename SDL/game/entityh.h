#ifndef ENTITYH_H
#define ENTITYH_H

#include <algorithm>
#include <vector>
#include <stack>
#include <memory>
#include "game.h"
#include "entity.h"

class Entity;
class Game;

class EntityFactory {
    public:
        virtual Entity *make(Game &game, unsigned int id) = 0;
};

class EntityHandler {
    public:
        EntityHandler(Game &world, EntityFactory &fact)
            : game(world), factory(fact) { }
        ~EntityHandler();
                    
        Entity *add(unsigned int id);
        void removeFirst(unsigned int id);
        void remove(Entity *entity);
		void removeAll(unsigned int id);
        
        Entity *findFirst(unsigned int id) const;
		std::list<Entity *> findAll(unsigned int id) const;
        
        void update(float delta);
        void render();
        
        void clear();
        
    private:
        std::stack<Entity *> creationStack;
        std::stack<Entity *> deletionStack;
        std::vector<Entity *> list;
        
        Game &game;
        EntityFactory &factory;
};
#endif
