#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.h"
chip8 myChip8;
int setupGraphics(SDL_Window* &win, SDL_Renderer* &ren, SDL_Texture* &tex);
void draw(SDL_Window* &win, SDL_Renderer* &ren, SDL_Texture* &tex);

unsigned char keymap[16] = {
    SDLK_x, SDLK_1, SDLK_2,
    SDLK_3, SDLK_q, SDLK_w,
    SDLK_e, SDLK_a, SDLK_s, 
    SDLK_d, SDLK_z, SDLK_c, 
    SDLK_4, SDLK_r, SDLK_f, 
    SDLK_v,
};

int main(int argc, char **argv) {
    using std::cout;
    using std::endl;
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Texture *tex;
    bool exit = false;
    SDL_Event event;
    int delay = 5;
    if(argc < 2) {
        cout << "Usage ./chip8 <ROM PATH>" << endl;
        return EXIT_FAILURE;
    }
    if(int result = setupGraphics(win, ren, tex)) {
        return result;
    }
    myChip8.initialize();
    myChip8.loadGame(argv[1]);
    myChip8.printMemory(0, 512 + 0xff);
    while((!exit)) {
        myChip8.emulateCycle();
        if(myChip8.drawFlag) {
            draw(win, ren, tex);
        }
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_F1) {
                        myChip8.initialize();
                        myChip8.loadGame(argv[1]);
                    }
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        delay = 1;
                    }
                    for(int i = 0; i < 16; i++) {
                        if(event.key.keysym.sym == keymap[i]) {
                            myChip8.key[i] = 1;
                        }
                    }
                break;
                case SDL_KEYUP:
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        delay = 5;
                    }
                    for(int i = 0; i < 16; i++) {
                        if(event.key.keysym.sym == keymap[i]) {
                            myChip8.key[i] = 0;
                        }
                    }
                break;
                case SDL_QUIT:
                    exit = true;
                    SDL_DestroyTexture(tex);
                    SDL_DestroyRenderer(ren);
                    SDL_DestroyWindow(win);
                    SDL_Quit(); 
                break;
                default:
                break;
            }
        }
        SDL_Delay(delay);
    }
    return EXIT_SUCCESS;

}

int setupGraphics(SDL_Window* &win, SDL_Renderer* &ren, SDL_Texture* &tex) {
    using std::cout;
    using std::endl;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL Init ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    win = SDL_CreateWindow("Chip-8!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    if(win == nullptr) {
        cout << "SDL Window ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if(ren == nullptr) {
        cout << "SDL Renderer ERROR: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }
    tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    return 0;
}
void draw(SDL_Window* &win, SDL_Renderer* &ren, SDL_Texture* &tex) {
    myChip8.drawFlag = false;
    uint32_t pixels[2048];
    for(int i = 0; i < 64 * 32; i++) {
        uint8_t pixel = myChip8.gfx[i];
        pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
    }
    SDL_UpdateTexture(tex, NULL, pixels, 64 * sizeof(Uint32));
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}