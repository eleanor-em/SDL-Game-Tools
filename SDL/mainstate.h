#ifndef MAINSTATE_H
#define MAINSTATE_H

#include "game/engine.h"    
#include "wolf.h"
#include "apple.h"
#include <cmath>

class MainState: public GameState {
    public:
        MainState(Game &game)
            : GameState(game), map(game, 1000, 1000, 36, 36), houseMap(game, 1000, 1000, 36, 36) {
            map.load("map.tm");
            houseMap.load("house.tm");
			
			map.renderToBuffer();			
			houseMap.renderToBuffer();
                
            solid = map.getPropertyID("solid");
            warp = houseMap.getPropertyID("warp");
        }
        
        static constexpr int ID = 0;
        unsigned int getID() override { return ID; }
        
        static constexpr int DefaultState = 0;
        static constexpr int HouseState = 1;
        
		void loadFile(Loader &loader) override {
			do {
				float x, y;
				if (!loader.getFloatValue(x) || 
				    !loader.nextDatum() ||
					!loader.getFloatValue(y)) {
					throw std::runtime_error("error loading mainstate");
				}
				dynamic_cast<Apple *>(addEntity(Apple::ID))->setPosition(x, y);
			} while (loader.nextDatum());

			float x, y;
			if (loader.nextBlob() &&
				loader.getFloatValue(x) &&
				loader.nextDatum() &&
				loader.getFloatValue(y)) {
					auto w = dynamic_cast<Wolf *>(addEntity(Wolf::ID));
					w->setPosition(x, y);
					game.getScreen().setTarget(w);
			}
			else {
				throw std::runtime_error("error loading mainstate");
			}
		}

		void saveFile(Saver &saver) override {
			auto list = findAll(Apple::ID);
			for (auto p : list) {
				saver.putDatum(p->getPosition().getCentre().x);
				saver.putDatum(p->getPosition().getCentre().y);
			}
			saver.closeBlob();
			saver.openBlob();
			auto p = findFirst(Wolf::ID);
			// wolf may not actually exist currently
			if (p) {
				saver.putDatum(p->getPosition().getCentre().x);
				saver.putDatum(p->getPosition().getCentre().y);
			}
		}

        void loadState(unsigned int scene) override;
        
        void updateState(float delta) override;
                                                  
        void renderState() override { game.getScreen().renderBufferedTileMap(map); game.getScreen().renderBufferedTileMap(houseMap); }
        
        bool solidAt(int x, int y) const { return map.getPropertyAt(solid, x, y) || houseMap.getPropertyAt(solid, x, y); }
		bool solidIn(const OrientedBB &box)  const { return map.maxPropertyIn(solid, box) || houseMap.maxPropertyIn(solid, box); }
        
    private:
        TileMap map;
        TileMap houseMap;
        Property solid;
        Property warp;
};

#endif
