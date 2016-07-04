#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <iostream>
#include "core.h"
#include "entityh.h"
#include "sdlwindow.h"
#include "inputh.h"
#include "sdlsys.h"
#include "component.h"
#include "vector2f.h"

class EntityFactory;
class GameState;
class Component;
class Screen;

class Game {
    public:
        const int WINDOW_W;
        const int WINDOW_H;
        const int WORLD_W;
        const int WORLD_H;
        const int WINDOW_X;
        const int WINDOW_Y;
        const char *WINDOW_CAPTION;

        ~Game();
    
        bool isRunning() const { return running; }
        
        void update(float delta);
        
        void render();
                
        void pushState(unsigned int id, unsigned int scene = 0);
        
        void exit() { running = false; }
        
        void loop();

		void save(const char *filename) const;
		void load(const char *filename);
        
        const SdlWindow &getWindow() const { return win; }
        const InputHandler &getInput() const { return input; }
        EntityFactory &getFactory() { return factory; }
        Screen &getScreen() { return *screen; }
            
        template <class Factory>
        static Game create(const char *title = "Game", Version _version = Version(0, 0, 0), unsigned int _key = 0,
							int windowWidth = 640, int windowHeight = 480,
                            int windowX = 100, int windowY = 100, int worldWidth = 1280, int worldHeight = 960);
        
        template <class State>
        void registerState();
        
        template <class Comp>
        void registerComponent();
        
        GameState &getState(unsigned int id) { return *stateMap.at(id); }
        Component &getComponent(unsigned int id) { return *componentMap.at(id); }
        
        bool stateExists(unsigned int id);
        
    private:
        static bool exists;
        static int __id_count;
    
        SdlWindow win;
        InputHandler input;
        Screen *screen;
        EntityFactory &factory;
        
        unsigned int clearUpTo;
        std::vector<GameState *> newStates;
        std::vector<GameState *> states;
        std::map<unsigned int, GameState *> stateMap;
        
        std::map<unsigned int, Component *> componentMap;
        
        bool running;
		Version version;
		unsigned int key;
        
		Game(EntityFactory &fact, int windowWidth, int windowHeight, int worldWidth,
			int worldHeight, int windowX, int windowY, const char *title, Version _version, unsigned int _key);
        
        bool handleEvents() const;
};

template <class Factory>
Game Game::create(const char *title, Version _version, unsigned int _key,
		int windowWidth, int windowHeight, int windowX, int windowY, int worldWidth, int worldHeight) {
    if (exists) {
        throw std::runtime_error("Game instantiated multiple times");
    }
    // need to have a concrete instance somewhere so why not here
    static Factory f;
    SdlSys::init();
    exists = true;
    return Game(f, windowWidth, windowHeight, worldWidth, worldHeight, windowX, windowY, title, _version, _key);
}

template <class State>
void Game::registerState() {
    State *p = new State(*this);
    // force State to inherit from GameState
    (void)(GameState *)&p;
    // check that this ID isn't used
    if (stateMap.count(p->getID()) != 0) {
        delete p;
        throw std::runtime_error("Multiple states share an ID");
    }
    stateMap.emplace(p->getID(), p);
}

template <class Comp>
void Game::registerComponent() {
    Comp *p = new Comp(*this);
    // check that this ID isn't used
    if (stateMap.count(p->getID()) != 0) {
        delete p;
        throw std::runtime_error("Multiple components share an ID");
    }
    componentMap.emplace(p->getID(), p);
}

#endif
