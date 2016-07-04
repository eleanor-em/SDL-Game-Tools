#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "saver.h"
#include "loader.h"
#include "game.h"
#include "entityh.h"

class Game;

class Entity;
class EntityHandler;

class GameState {
    public:
        GameState(Game &world);
        virtual ~GameState() { }
        
        void load(unsigned int scene) { loadState(scene); }
		void unload() { clearEntities(); }
		
		virtual void saveFile(Saver &saver) { (void)saver; }
		virtual void loadFile(Loader &loader) { (void)loader; }
        
        void update(float delta);
        void render() { renderState(); entities.render(); }
        
		std::list<Entity *> findAll(unsigned int id) const;
        Entity *addEntity(unsigned int id);
        void removeFirstEntity(unsigned int id);
        void removeEntity(Entity *entity);
		void removeAll(unsigned int id);
        Entity *findFirst(unsigned int id) const;
        
        virtual unsigned int getID() = 0;
        
        Game &getGame() { return game; }
        
        enum Hook {
            hookNone,   // hooks nothing
            hookInput,  // only hooks input
            hookUpdate, // hooks update and input
            hookClear   // clears all states below it
        };
        // default behaviour is to clear all lower states
        virtual Hook getHook() const { return hookClear; }
        
    protected:
        virtual void loadState(unsigned int scene) { (void)scene; }
		virtual void unloadState() { }
    
        virtual void updateState(float delta) { (void)delta; }
        virtual void renderState() { }
			        
        Game &game;
    
    private:
        EntityHandler entities;
		void clearEntities();
};

#endif
