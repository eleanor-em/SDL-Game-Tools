#include "gamestate.h"
#include "game.h"
#include "entityh.h"

// needs to be here so that the types are complete
GameState::GameState(Game &world)
        : game(world), entities(world, world.getFactory()) { }
Entity *GameState::addEntity(unsigned int id) { Entity *p = entities.add(id); p->_state = this; return p; }
void GameState::update(float delta) { entities.update(delta); updateState(delta); }
void GameState::removeFirstEntity(unsigned int id) { entities.removeFirst(id); }
void GameState::removeEntity(Entity *entity) { entities.remove(entity); }
void GameState::removeAll(unsigned int id) { entities.removeAll(id); }
Entity *GameState::findFirst(unsigned int id) const { return entities.findFirst(id); }
std::list<Entity *> GameState::findAll(unsigned int id) const { return entities.findAll(id); }
void GameState::clearEntities() { entities.clear(); }
