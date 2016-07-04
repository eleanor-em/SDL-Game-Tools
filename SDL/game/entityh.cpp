#include <algorithm>
#include <utility>
#include <set>
#include "entity.h"
#include "entityh.h"
#include "orientedbb.h"
#include "game.h"

std::pair<Entity *, Entity *> makePair(Entity *a, Entity *b) {
	return std::pair<Entity *, Entity *>( (a < b) ? (a) : (b),
										  (a < b) ? (b) : (a) );
}

EntityHandler::~EntityHandler() {
   	for (auto p : list) {
   		delete p;
   	}
}

void EntityHandler::removeFirst(unsigned int id) {
    // find and flag the pointers matching the id
	remove(*std::find_if(list.begin(), list.end(), [id](Entity *p) { return p->getID() == id; }));
}

void EntityHandler::remove(Entity *entity) {
	deletionStack.push(entity);
}

void EntityHandler::removeAll(unsigned int id) {
	for (auto p : list) {
		if (p->getID() == id) {
			remove(p);
		}
	}
}

void EntityHandler::update(float delta) {
    // update the list
    while (creationStack.size()) {
        list.push_back(creationStack.top());
        creationStack.pop();
    }    
    while (deletionStack.size()) {
        auto it = std::find(list.begin(), list.end(), deletionStack.top());
        delete *it;
        list.erase(it);
        
        deletionStack.pop();
    }
    
    // update the entities
    for (Entity *p : list) {
        p->update(delta);
    }
    
    // we expect a relatively small number of entities
    std::set< std::pair<Entity *, Entity *> > collisions;
    for (Entity *first : list) {
    	for (Entity *second : list) {
    		Manifold m;
    		if (first != second && first->getPosition().intersects(second->getPosition(), m)) {
    			auto p = makePair(first, second);
    			if (collisions.count(p) == 0) {
    				collisions.insert(p);
					first->handleCollision(m, *second);
					second->handleCollision(m, *first);
				}
    		}
    	}
    }
}

void EntityHandler::clear() {
    for (Entity *p : list) {
        if (p->getPersist() == false) {
        	deletionStack.push(p);
		}
    }
}

Entity *EntityHandler::add(unsigned int id) { Entity *p = factory.make(game, id); creationStack.push(p); return p; }

Entity *EntityHandler::findFirst(unsigned int id) const {
    auto p = find_if(list.begin(), list.end(), [=](Entity *p){ return p->getID() == id; });
    if (p == list.end()) {
    	throw std::out_of_range("Entity not found");
    }
	return *p;
}
std::list<Entity *> EntityHandler::findAll(unsigned int id) const {
	std::list<Entity *> ret;
	for (auto p : list) {
		if (p->getID() == id) {
			ret.push_back(p);
		}
	}
	return ret;
}

void EntityHandler::render() { for (Entity *p : list) { p->render(); } }

