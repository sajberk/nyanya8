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

    std::string romFilename = "IBM Logo.ch8"; 
    chip8.loadRom(romFilename);

    while(!sdlHandler.shouldQuit) 
    {
        chip8.setKeystates(sdlHandler.processInput()); 
        chip8.fetchDecodeExecute();
        sdlHandler.update(chip8.getDisplay());
        sdlHandler.render();    
    }

    return 0;
}

