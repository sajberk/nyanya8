#ifndef SDLHANDLER_H
#define SDLHANDLER_H

#include <SDL2/SDL.h>
#include <array>

class SDLHandler
{
public:
    SDLHandler();
    ~SDLHandler();

    bool init();
    std::array<bool, 16> processInput();
    void update(std::array<bool, 64*32> displayPixelValues);
    void render();

    bool shouldQuit;

private:
    SDL_Window *window;
    SDL_Surface *screenSurface;
    SDL_Texture *display;
    SDL_Renderer *renderer;
    std::array<bool, 16> keystates;
};

#endif // SDLHANDLER_H