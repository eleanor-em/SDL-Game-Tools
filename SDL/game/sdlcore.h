#ifndef SDLCORE_H
#define SDLCORE_H

#include <exception>
#include <string>

class SdlException: public std::exception {
    public:
        SdlException(const char *fname) noexcept {
            f = "Exception in function ";
            f += fname;
            f += ": ";
            f += SDL_GetError();
        }
        const char *what() const noexcept override {
            return f.c_str();
        }
    private:
        std::string f;
};

#endif
