#ifndef SDLSYS_H
#define SDLSYS_H

#include <SDL2/SDL.h>

class SdlSys {
    public:
        static void init();
        ~SdlSys();
    
    private:
        SdlSys();
};

#endif
