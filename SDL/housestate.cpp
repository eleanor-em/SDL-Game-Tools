#include "game/engine.h"
#include "housestate.h"
#include "mainstate.h"

void HouseState::loadState(unsigned int scene) {
	(void) scene;
	
    Wolf *w = (Wolf *)addEntity(Wolf::ID);
    game.getScreen().setTarget(w);
    w->setPosition(36 * 10.5f, 36 * 11);
}

void HouseState::updateState(float delta) {
	(void) delta;

    if (game.getInput().keyDown(SDL_SCANCODE_ESCAPE)) { game.exit(); }
    auto b = findFirst(Wolf::ID)->getPosition();
    auto res = map.getPropertyAt(warp, (int)b.getCentre().x, (int)b.getCentre().y);
    switch (res) {
        case 1:
            game.pushState(MainState::ID, MainState::HouseState);
            break;
    }
}
