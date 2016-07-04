#include "game.h"
#include "saver.h"
#include "loader.h"
#include "fps.h"
#include "gamestate.h"
#include "screen.h"
#include "core.h"
#include <algorithm>

CWD CWD::obj;
bool Game::exists = false;

Game::Game(EntityFactory &fact, int windowWidth, int windowHeight, int worldWidth,
		int worldHeight, int windowX, int windowY, const char *title, Version _version, unsigned int _key)
    :   WINDOW_W(windowWidth), WINDOW_H(windowHeight), WORLD_W(worldWidth), WORLD_H(worldHeight), WINDOW_X(windowX), WINDOW_Y(windowY), WINDOW_CAPTION(title),
        win(WINDOW_X, WINDOW_Y, WINDOW_W, WINDOW_H, WINDOW_CAPTION), screen(new Screen()), factory(fact), clearUpTo(0), running(false),
		version(_version), key(_key) {
    screen->width = (float)WINDOW_W;
    screen->height = (float)WINDOW_H;
    screen->maxWidth = (float)WORLD_W;
    screen->maxHeight = (float)WORLD_H;
}
      
Game::~Game() {
    // free memory
    for (auto p : stateMap) {
        delete p.second;
    }
    for (auto p : componentMap) {
        delete p.second;
    }
    delete screen;
}

void Game::update(float delta) {
    // do the housekeeping
    running = handleEvents();
    input.update();
    
    // update components
    for (auto p : componentMap) {
        p.second->update(delta);
    }
    
    // update state list
    for (auto p : newStates) {
        states.push_back(p);
    }
    newStates.clear();
    
    // iterate over states top down, hooking input when necessary
    for (auto it = states.rbegin(); it != states.rend(); it++) {
         (*it)->update(delta);
         // hook input if necessary
         if ((*it)->getHook() == GameState::hookInput) {
            input.disable();
         }
         // break if this state hooks the update
         if ((*it)->getHook() == GameState::hookUpdate) {
            break;
         }
    }
    // unhook input
    input.enable();
    
    // delete if we need to
    if (clearUpTo > 0) {
		// unload each state
		for (auto p = states.begin(); p < states.begin() + clearUpTo; ++p) {
			(*p)->unload();
		}
        states.erase(states.begin(), states.begin() + clearUpTo);
        clearUpTo = 0;
    }
    
    // update screen
    screen->update();
}

void Game::render() {
    win.clear();
    for (auto p : states) {
        p->render();
    }
    win.present();
}

// returns false if the program should quit
bool Game::handleEvents() const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                return false;
        }
    }
    return true;
}

void Game::loop() {
    if (!running) {
        running = true;
        FPS fps;
        try {
		    while (running) {
		        float delta = (float)fps.update();
		        update(delta);
		        render();
		    }
		} catch (InternalException &e) {
			std::cout << "warning: " << e.what() << "\n";
		} catch (...) {
			throw;
		}
    }
}

void Game::pushState(unsigned int id, unsigned int scene) {
    GameState *p = stateMap.at(id);
    // we can't modify the list while iterating over it
    if (p->getHook() == GameState::hookClear) {
        clearUpTo = states.size();
    }
    newStates.push_back(stateMap.at(id));
    p->load(scene);
}

bool Game::stateExists(unsigned int id) {
    return std::find_if(states.begin(), states.end(), [=](GameState *p) { return p->getID() == id; }) != states.end();
}

void Game::save(const char * filename) const {
	Saver saver(filename, version, key);
	for (auto p : stateMap) {
		saver.openBlob();
		saver.putDatum(p.second->getID());
		p.second->saveFile(saver);
		saver.closeBlob();
	}
	for (auto p : componentMap) {
		saver.openBlob();
		saver.putDatum(p.second->getID());
		p.second->saveFile(saver);
		saver.closeBlob();
	}
}

void Game::load(const char *filename) {
	Loader loader(filename, version, key);
	for (auto p : stateMap) {
		// first datum is an ID
		loader.nextDatum();

		p.second->loadFile(loader);
		if (!loader.nextBlob()) {
			return;
		}
	}
	for (auto p : componentMap) {
		// first datum is an ID
		loader.nextDatum();

		p.second->loadFile(loader);
		if (!loader.nextBlob()) {
			return;
		}
	}
}