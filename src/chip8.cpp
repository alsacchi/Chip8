#include "chip8.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;
//Fontset di 16 caratteri 4x5
unsigned char chip8::chip8_fontset[80] = { 
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

//Inizializzo il PC, la memoria, l'instruction register e lo stack pointer
void chip8::initialize() { 
    chip8::pc = 0x200;
    chip8::opcode = 0;
    chip8::I = 0;
    chip8::sp = 0;
    //Inizializzo la memoria, i registri e lo stack a 0x00
    for(int i = 0, j = 0; i < 4096; i++) {
        chip8::memory[i] = '\0';
        if(i%256 == 0) {
            chip8::V[j] = 0x00;
            chip8::stack[j] = 0x00;
            j++;
        }
    }
    //0x50 = 80;
    //Carico in memoria nei primi 0x50 bytes il fontset
    for(int i = 0; i < 80; i++) {
       chip8:: memory[i] = chip8::chip8_fontset[i];
    }
    //Inizilizzo i due timer a 0
    chip8::delay_timer = 0;
    chip8::sound_timer = 0;
    cout << "Initialized" << endl;
}
//Ciclo operativo:
//Fetch OPCODE
//Decode OPCODE
//Execute OPCODE
void chip8::emulateCycle() {
    //Formo l'opcode spostando di 8 bit il primo byte a sinistra
    //e faccio l'or con il secondo byte per unirlo al primo
    chip8::opcode = chip8::memory[pc] << 8 | chip8::memory[pc + 1];
    switch(opcode & 0xF000) {
        case 0xA000:
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        default:
            cout << "Unknown opcode: " << std::hex << opcode << endl;
            break;
    }
    if(delay_timer > 0) {
        --delay_timer;
    }
    if(sound_timer > 0) {
        if(sound_timer == 1) {
            cout << "BEEP!" << endl;
        }
        --sound_timer;
    }
}
//Questa funzione carica la rom nella memoria!
void chip8::loadGame(std::string path) {
    //Carico il file
    std::ifstream file (path, std::ifstream::binary);
    if(file) {
        //Trovo la lunghezza del file e poi scrivo nella memoria all'offset 0x200 il contenuto
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);
        cout << "Reading " << length << " bytes" << endl;
        file.read((char *)(chip8::memory + 0x200), length);
        if(file) {
            cout << "Success" << endl;
        } else {
            cout << "Error: only  " << file.gcount() << " bytes could be read" << endl;
        }
        file.close();
    }
} 

//Stampo determinati bytes della memoria
void chip8::printMemory(int start, int end) {
    for(int i = start, j = 16; i < end; i++) {
            if(j > 15) {
                cout << "\n" << std::hex << i << ": ";
                j=0;
            }
            cout << std::hex << static_cast<int>(chip8::memory[i]) << " ";
            j++;
        }
}
