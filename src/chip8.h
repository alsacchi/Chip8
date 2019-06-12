#include <iostream>
/*
    Architettura CHIP8 {
        Cpu 8 Bit
        4Kib di memoria
        16 registri 8 bit V0....VF, VF registro FLAG per alcune istruzioni
        Address Register I, 16 bit
        Stack a 24 livelli
        Due timer che contano fino a 0:
            sound_timer e delay timer
        Input con un tastierino da 16 'pulsanti'
        Display:
            Risoluzione 64x32
            Monocromatico
        35 Opcode
    }
*/

class chip8 {
    private:
        uint16_t opcode;
        uint8_t memory[4096];
        uint8_t V[16];
        uint16_t I;
        uint16_t pc;
        uint8_t delay_timer;
        uint8_t sound_timer; 
        uint16_t stack[16];
        uint16_t sp;
        static uint8_t chip8_fontset[80];

    public:
        uint8_t key[16];
        uint8_t gfx[64 * 32];
        bool drawFlag; //Flag drawFlag, se 1 aggiorna lo schermo
        void initialize();
        void emulateCycle();
        void loadGame(std::string path);
        void printMemory(int start, int end);
};
