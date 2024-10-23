#include "SDLHandler.h"
#include "Chip8.h"
#include <iostream>
#include <iomanip>
#include <fstream>

int main() {
    SDLHandler sdlHandler;
    if(!sdlHandler.init())
    {
        return 1;
    }

    Chip8 chip8;

    std::string romFilename = "roms/test_opcode.ch8";
    chip8.loadRom(romFilename);

    while(!sdlHandler.shouldQuit) 
    {
        Uint64 frameStart = SDL_GetTicks64();

        chip8.setKeystates(sdlHandler.processInput()); 
        for (int i = 0; i < 20; i++)
        {
            chip8.fetchDecodeExecute();
        }
        chip8.decreaseTimers();
        if (chip8.displayChanged)
        {
            sdlHandler.update(chip8.getDisplay());
            sdlHandler.render();   
            chip8.displayChanged = false;
        }
         

        Uint64 frameTime = SDL_GetTicks64() - frameStart;
        SDL_Delay(16.67 - frameTime);
    }
    return 0;
}

