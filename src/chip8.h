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
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char V[16];
        unsigned short I;
        unsigned short pc;
        unsigned char gfx[64 * 32];
        unsigned char delay_timer;
        unsigned char sound_timer; 
        unsigned short stack[16];
        unsigned short sp;
        unsigned char key[16];
        static unsigned char chip8_fontset[80];

    public:
        //Flag drawFlag, se 1 aggiorna lo schermo
        bool drawFlag = false;
        void initialize();
        void emulateCycle();
        void loadGame(std::string path);
        void printMemory(int start, int end);
};
