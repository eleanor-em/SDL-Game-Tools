#ifndef HOUSESTATE_H
#define HOUSESTATE_H
#include "game/engine.h"

class HouseState: public GameState {
    public:
        HouseState(Game &game)
            : GameState(game), map(game, 500, 500, 36, 36) {
            map.load("inside.tm");
            solid = map.getPropertyID("solid");
            warp = map.getPropertyID("warp");
			map.renderToBuffer();
        }
            
        static constexpr int ID = 1;
        unsigned int getID() override { return ID; }
        
        void loadState(unsigned int scene) override;
        
        void updateState(float delta) override;
                                                  
        void renderState() override { game.getScreen().renderBufferedTileMap(map); }
        
        bool solidAt(int x, int y) const { return map.getPropertyAt(solid, x, y) == 1; }
		bool solidIn(const OrientedBB &box)  const { return map.maxPropertyIn(solid, box); }
        
    private:
        TileMap map;
        Property solid;
        Property warp;
};
#endif
