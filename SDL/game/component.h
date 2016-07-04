#ifndef COMPONENT_H
#define COMPONENT_H

#include "saver.h"
#include "loader.h"
#include "game.h"

class Game;

class Component {
    public:
        Component(Game &world)
            : game(world) { }
        virtual ~Component() { }
        
        virtual void update(float delta) { (void)delta; }
        
        virtual unsigned int getID() const = 0;

		virtual void saveFile(Saver &saver) { (void)saver; }
		virtual void loadFile(Loader &loader) { (void)loader; }
        
    protected:
        Game &game;
};

#endif
