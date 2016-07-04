#include "game/game.h"
#include <iostream>
#include "game/saver.h"
#include "game/loader.h"
#include "wolf.h"
#include "apple.h"
#include "housestate.h"
#include "mainstate.h"

class Factory: public EntityFactory {
    public:
        Entity *make(Game &game, unsigned int id) override {
            switch (id) {
                case Wolf::ID:
                    return new Wolf(game);
                case Apple::ID:
                	return new Apple(game);
                default:
                    return nullptr;
            }
        }
};

int main(int argc, char ** argv) {
	if (argc > 0) {
		CWD::setCWD(argv[0]);
	}
	Game game = Game::create<Factory>("Game", Version(), 0);
    game.registerState<MainState>();
    game.registerState<HouseState>();
    game.pushState(MainState::ID);
    game.loop();
    return 0;
}

