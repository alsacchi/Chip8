#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"
chip8 myChip8;
SDL_Window *win;
SDL_Renderer *ren;
int setupGraphics();
void draw();
int main() {
    bool exit = false;
    using std::cout;
    using std::endl;
    SDL_Event event;
    if(int result = setupGraphics()) {
        return result;
    }
    myChip8.initialize();
    myChip8.loadGame("../roms/pong.ch8");
    myChip8.printMemory(0, 512 + 0xff);
    while((!exit)) {
        myChip8.emulateCycle();
        if(myChip8.drawFlag) {
            draw();
        }
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    break;
                case SDL_QUIT:
                    exit = true;
                    SDL_DestroyRenderer(ren);
                    SDL_DestroyWindow(win);
                    SDL_Quit(); 
                    break;
                default:
                    break;
            }
        }
        SDL_RenderPresent(ren);
        SDL_Delay(1000);
    }
    return EXIT_SUCCESS;

}

int setupGraphics() {
    using std::cout;
    using std::endl;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL Init ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    SDL_Window *win = SDL_CreateWindow("Chip-8!", 100, 100, 300, 300, SDL_WINDOW_SHOWN);
    if(win == nullptr) {
        cout << "SDL Window ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == nullptr) {
        cout << "SDL Renderer ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);
SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    return 0;
}
void draw() {
    myChip8.drawFlag = false;
}