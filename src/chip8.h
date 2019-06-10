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

/*
    OPCODES {
        0x0NNN // Chiama dal RCA 1802 le istruzioni all'indirizzo NNN ! Non utilizzato
        0x00E0 // Pulisce lo schermo
        0x00EE // Ret
        0x1NNN // Jmp a NNN
        0x2NNN // Chiama subroutin a NNN ! *(0xNNN)();
        0x3XNN // Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN)
        0x4XNN // Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN)
        0x5XY0 // Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y])
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
