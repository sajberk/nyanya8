#include "SDLHandler.h"
#include <stdio.h>

SDLHandler::SDLHandler() : window(nullptr), screenSurface(nullptr), display(nullptr), renderer(nullptr), shouldQuit(false) 
{
    keystates.fill(false);
}

SDLHandler::~SDLHandler() 
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool SDLHandler::init() 
{
    int SCREEN_HEIGHT = 320;
    int SCREEN_WIDTH = 640;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}
    
    window = SDL_CreateWindow( "nyanya8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }

    screenSurface = SDL_GetWindowSurface( window );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    return true;

}

std::array<bool, 16> SDLHandler::processInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            shouldQuit = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_1: keystates[0x1] = true; break;
                case SDLK_2: keystates[0x2] = true; break;
                case SDLK_3: keystates[0x3] = true; break;
                case SDLK_4: keystates[0xC] = true; break;

                
                case SDLK_q: keystates[0x4] = true; break;
                case SDLK_w: keystates[0x5] = true; break;
                case SDLK_e: keystates[0x6] = true; break;
                case SDLK_r: keystates[0xD] = true; break;
                
                case SDLK_a: keystates[0x7] = true; break;
                case SDLK_s: keystates[0x8] = true; break;
                case SDLK_d: keystates[0x9] = true; break;
                case SDLK_f: keystates[0xE] = true; break;

                case SDLK_z: keystates[0xA] = true; break;
                case SDLK_x: keystates[0x0] = true; break;
                case SDLK_c: keystates[0xB] = true; break;
                case SDLK_v: keystates[0xF] = true; break;
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_1: keystates[0x1] = false; break;
                case SDLK_2: keystates[0x2] = false; break;
                case SDLK_3: keystates[0x3] = false; break;
                case SDLK_4: keystates[0xC] = false; break;
                
                case SDLK_q: keystates[0x4] = false; break;
                case SDLK_w: keystates[0x5] = false; break;
                case SDLK_e: keystates[0x6] = false; break;
                case SDLK_r: keystates[0xD] = false; break;
                
                case SDLK_a: keystates[0x7] = false; break;
                case SDLK_s: keystates[0x8] = false; break;
                case SDLK_d: keystates[0x9] = false; break;
                case SDLK_f: keystates[0xE] = false; break;

                case SDLK_z: keystates[0xA] = false; break;
                case SDLK_x: keystates[0x0] = false; break;
                case SDLK_c: keystates[0xB] = false; break;
                case SDLK_v: keystates[0xF] = false; break;
            }
        }
    }

    return keystates; 
}

void SDLHandler::update(std::array<bool, 64*32> displayPixelValues)
{
    uint8_t *pixels;
    int pitch = 64 * 32 * 4;

    SDL_LockTexture(display, NULL, (void **)&pixels, &pitch);

    for (int i = 0; i < 64 * 32 * 4; i++)
    {
        if (displayPixelValues[i/4])
        {
            *(pixels + i) = 255;
        }
        else
        {
            *(pixels + i) = 0;
        }
    }

    SDL_UnlockTexture(display);


    
    
    return;
}

void SDLHandler::render()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, display, NULL, NULL);
    SDL_RenderPresent(renderer);
    return;
}
