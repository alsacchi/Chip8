#include "chip8.h"
#include <cstdlib>
#include <ctime>
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
    drawFlag = true;
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    //Inizializzo la memoria, i registri e lo stack a 0x00
    for(int i = 0, j = 0; i < 4096; i++) {
        memory[i] = '\0';
        if(i%256 == 0) {
            V[j] = 0x00;
            stack[j] = 0x00;
            j++;
        }
    }
    for(int i = 0; i < 64 * 32; i++) {
        gfx[i] = 0;
    }
    //0x50 = 80;
    //Carico in memoria nei primi 0x50 bytes il fontset
    for(int i = 0; i < 80; i++) {
        memory[i] = chip8::chip8_fontset[i];
    }
    //Inizilizzo i due timer a 0
    delay_timer = 0;
    sound_timer = 0;
    cout << "Initialized" << endl;
}
//Ciclo operativo:
//Fetch OPCODE
//Decode OPCODE
//Execute OPCODE
void chip8::emulateCycle() {
    //Formo l'opcode spostando di 8 bit il primo byte a sinistra
    //e faccio l'or con il secondo byte per unirlo al primo
    opcode = memory[pc] << 8 | memory[pc + 1];
    cout << "["<< std::hex << pc << "]" << "Executing opcode: " << std::hex << opcode << endl;
    /*
    OPCODES {
        0x0NNN // Chiama dal RCA 1802 le istruzioni all'indirizzo NNN ! Non utilizzato
        0x00E0 // Pulisce lo schermo
        0x00EE // Ritorna da una subroutine
        0x1NNN // Salta a NNN
        0x2NNN // Chiama subroutin a NNN ! *(0xNNN)();
        0x3XNN // Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN)
        0x4XNN // Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN)
        0x5XY0 // Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y])
        0x6XNN // Assegna il valore NN al registro V[X]
        0x7XNN // Aggiunge il valore NN al registro V[X] (Carry Flag immutata)
        0x8XY0 // Assegna il valore V[Y] a V[X]
    }
    */
    switch(opcode & 0xF000) { // Prendo solo il primo byte dell'opcode
        case 0x0000: // Visto che il primo byte non mi permette di distinguere i due opcode che iniziano con 0x00
            switch(opcode & 0x000F) { // Prendo gli opcode che inziano con 0x00 e controllo l'ultima parte per distinguerli
                case 0x0000: // 0x00E0 Pulisce lo schermo
                break;
                case 0x000E: // 0x00EE Ritorna da una subroutine
                    pc = stack[sp - 1];
                break;
            }
        break;
        case 0x1000: // 0x1NNN Salta a NNN
            pc = opcode & 0x0FFF;
            pc += 2;
        break;
        case 0x2000: // 0x2NNN Chiama subroutin a NNN ! *(0xNNN)();
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
        break;
        case 0x3000: // 0x3XNN Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN)
            if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                pc+=4;
            }
            pc += 2;
        break;
        case 0x4000: // 0x4XNN Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN)
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                pc+=4;
            }
            pc += 2;
        break;
        case 0x5000: // 0x5XY0 Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y])
            if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]) {
                pc+=4;
            }
            pc += 2;
        break;
        case 0x6000: // 0x6XNN Assegna il valore NN al registro V[X]
            V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            pc += 2;
        break;
        case 0x7000: // 0x7XNN Aggiunge il valore NN al registro V[X] (Carry Flag immutata)
            V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            pc += 2;
        break;
        case 0x8000:
            switch(opcode & 0x000F) {
                case 0x0000: // 0x8XY0 Assegna il valore di V[Y] a V[X]
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0001: // 0x8XY1 Assegna il valore di V[X] OR(|) V[Y] a V[X]
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0002: // 0x8XY2 Assegna il valore di V[X] AND(&) V[Y] a V[X]
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0003: // 0x8XY3 Assegna il valore di V[X] XOR(^) V[Y] a V[X]
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0004: // 0x8XY4 Aggiunge il valore di V[Y] a V[X]. In caso di carry V[0xF] impostato a 1, altrimenti 0
                    if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0005: // 0x8XY5 Sottrae il valore di V[Y] a V[X]. In caso di prestito V[0xF] impostato a 0, altrimenti 1
                    if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) {
                        V[0xF] = 0;
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                case 0x0006: // 0x8XY6 Immagazina il bit meno siginificativo di V[X] in V[0xF] per poi fare lo shift di 1 a destra di V[X]
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                break;  
                case 0x0007: // 0x8XY7 Assegna a V[X] la differenza fra V[Y] - V[X]. In caso di prestito V[0xF] impostato a 0, altrimenti 1
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) {
                        V[0xF] = 0;
                    } else {
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;
                case 0x000E: // 0x8XYE Immagazina il bit più significativo di V[X] in V[0xF] per poi fare lo shift di 1 a sinistra di V[X]
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x80;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                break;
                default:
                break;
            }
        case 0x9000: // 0x9XY0 Salta l'istruzione successiva se V[X] è diverso da V[Y]
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) {
                pc += 4;
            } else {
                pc += 2;
            }
        break;
        case 0xA000: // 0xANNN Imposta I a NNN
            I = opcode & 0x0FFF;
            pc += 2;
        break;
        case 0xB000: // 0xBNNN Salta all'indirizzo NNN + V[0]
            pc = (opcode & 0x0FFF) + V[0];
        break;
        case 0xC000: // 0xCXNN Imposta V[X] all'risultato dell'operazione logica AND(&) tra un numero casuale(0-255) e NN
            srand(time(0));
            V[(opcode & 0x0F00) >> 8] = (rand() % 255) & (opcode & 0x00FF);
            pc += 2;
        break;
        case 0xD000: // 0xDXYN Disegna uno sprite alle coordinate V[X] e V[Y] largo 8 pixel e alto N pixel,
                     // Ogni riga di 8 pixel è letta bit per bit dalla memoria partendo dall' indirizzo I
                     // il valore di I non cambia. VF è impostato a 1 se un qualsiasi pixel viene capovolto
                     // invece 0 se non succede
            //draw(V[(opcode & 0x0F00) >> 8], V[(opcode & 0x00F0) >> 4], opcode & 0x000F);
            pc += 2;
            break;
        default:
            cout << "["<< std::hex << pc << "]" << "Unknown opcode: " << std::hex << opcode << endl;
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
        file.read((char *)(memory + 0x200), length);
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
            cout << std::hex << static_cast<int>(memory[i]) << " ";
            j++;
        }
}
