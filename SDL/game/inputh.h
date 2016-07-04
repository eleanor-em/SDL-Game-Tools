#ifndef INPUTH_H
#define INPUTH_H

#include <SDL2/SDL.h>
#include <cstring>

class InputHandler {
    public:
        InputHandler()
            : enabled(true) {
            auto p = SDL_GetKeyboardState(&numKeys);
            
            prev = new Uint8[numKeys];
            current = new Uint8[numKeys];
            std::memcpy(prev, p, numKeys);
            std::memcpy(current, p, numKeys);
        }
        ~InputHandler() { delete[] prev; delete[] current; }
        
        void update() {
            // can't just use p because SDL might modify it between updates
            std::memcpy(prev, current, numKeys);
            auto p = SDL_GetKeyboardState(nullptr);
            std::memcpy(current, p, numKeys);
        }
        
        bool keyReleased(int key) const { return enabled && current[key] == 0 && prev[key] == 1; }
        
        bool keyPressed(int key) const { return enabled && current[key] == 0 && prev[key] == 1; }
        
        bool keyDown(int key) const { return enabled && current[key]; }
        
        friend class Game;
        
    private:
        Uint8 *prev;
        Uint8 *current;
        bool enabled;
        int numKeys;
        
        void disable() { enabled = false; }
        void enable() { enabled = true; }
        
        // do not allow copy
        InputHandler(const InputHandler &copy) { (void)copy; }
};

#endif
