#include "housestate.h"
#include "mainstate.h"
#include "wolf.h"

void Wolf::update(float delta) {
    Vector2f prevPos = position;
    if (game.getInput().keyDown(SDL_SCANCODE_RIGHT)) {
        position.x += SPEED * delta;
        changeSprite(right);
    }
    else if (game.getInput().keyDown(SDL_SCANCODE_LEFT)) {
        position.x -= SPEED * delta;
        changeSprite(left);
    }
    else if (game.getInput().keyDown(SDL_SCANCODE_DOWN)) {
        position.y += SPEED * delta;
        changeSprite(down);
    }
    else if (game.getInput().keyDown(SDL_SCANCODE_UP)) {
        position.y -= SPEED * delta;
        changeSprite(up);
    }
    else {
        sprite->pause();
    }
    sprite->update(delta);
    if (game.stateExists(MainState::ID)) {
        MainState &s = (MainState &)game.getState(MainState::ID);
        if (s.solidAt((int)position.x, (int)position.y) || s.solidIn(getPosition()) ) {
            position = prevPos;
        }
    }
    if (game.stateExists(HouseState::ID)) {
        HouseState &s = (HouseState &)game.getState(HouseState::ID);
        if (s.solidAt((int)position.x, (int)position.y) || s.solidIn(getPosition())) {
            position = prevPos;
        }
    }
}
