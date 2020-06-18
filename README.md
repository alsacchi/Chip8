# Emulatore CHIP8
## Technically completed, sound not present, crash may occur!
[Architettura CHIP8](https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description)

    Cpu 8 Bit
    4Kib di memoria
    16 registri 8 bit V0....VF, VF registro FLAG per alcune istruzioni
    Address Register I, 16 bit
    Stack a 24 livelli
    Due timer:
        sound_timer e delay timer
    Input con un tastierino da 16 'pulsanti'
    Display:
        Risoluzione 64x32
        Monocromatico
    35 Opcode


### How to Build:
```
git clone https://github.com/alsacchi/Chip8.git
cd chip8/src
make
```
---
### How to use
After you have built the chip8 emulator you can execute it with the binary 'chip8' inside src/ folder
```
cd chip8/src
./chip8 <ROM PATH>
```
---
### Keypad:
#### Keyboard
|KEYS|KEYS|KEYS|KEYS|
|:---:|:---:|:---:|:---:|
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

#### Chip8
|KEYS|KEYS|KEYS|KEYS|
|:---:|:---:|:---:|:---:|
| 1 | 2 | 3 | C |
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |


~SPACE for SPEED BOOST~
----
OPCODES

| OPCODE | Tipo | Descrizione |
| ----- | ----- | ----- |
| 0x0NNN | Call | // Chiama "Machine code routine" all'indirizzo NNN ! Non utilizzato nella maggior parte di ROM |
| 0x00E0 | Display | // Pulisce lo schermo |
| 0x00EE | Flusso | // Ret |
| 0x1NNN | Flusso | // Jmp a NNN |
| 0x2NNN | Flusso | // Chiama subroutine a NNN ! *(0xNNN)(); |
| 0x3XNN | Condizione | // Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN) |
| 0x4XNN | Condizione | // Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN) |
| 0x5XY0 | Condizione | // Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y]) |
| 0x6XNN | Assegnazione | // Assegna il valore NN al registro V[X] |
| 0x7XNN | Assegnazione | // Aggiunge il valore NN al registro V[X] (Carry Flag immutata) |
| 0x8XY0 | Assegnazione | // Assegna il valore V[Y] a V[X] |
| ... | ... | ... |
