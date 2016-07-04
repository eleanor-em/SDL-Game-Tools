#include "housestate.h"
#include "mainstate.h"
#include "wolf.h"
#include "apple.h"

void MainState::loadState(unsigned int scene) {
	if (!fexists("save")) {
		Wolf *w = (Wolf *)addEntity(Wolf::ID);
		game.getScreen().setTarget(w);
		for (int i = 0; i < 20; ++i) {
			dynamic_cast<Apple *>(addEntity(Apple::ID))->setPosition((float)(36 * (rand() % 20 + 6)), (float)(36 * (rand() % 7 + 7)));
		}
	}
	else if (scene == 0) {
		game.load("save");
	}
	if (scene == 1) {
		Wolf *w = (Wolf *)addEntity(Wolf::ID);
		game.getScreen().setTarget(w);
		w->setPosition(36 * 22 + 18, 36 * 8);
		w->faceDown();
	}
}

void MainState::updateState(float delta) {
	(void) delta;
    if (game.getInput().keyDown(SDL_SCANCODE_ESCAPE)) { game.exit(); }
	if (game.getInput().keyDown(SDL_SCANCODE_S)) { game.save("save"); }
    // a wolf definitely exists
    auto b = findFirst(Wolf::ID)->getPosition();
	auto res = houseMap.maxPropertyIn(warp, b);
    switch (res) {
        case 1:
            game.pushState(HouseState::ID, 0);
            break;
    }
}
