# Emulatore CHIP8
## (WIP)
[Architettura CHIP8](https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description)

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



OPCODES

| OPCODE | Description |
| ----- | ----- |
| 0x0NNN | // Chiama dal RCA 1802 le istruzioni all'indirizzo NNN ! Non utilizzato |
| 0x00E0 | // Pulisce lo schermo |
| 0x00EE | // Ret |
| 0x1NNN | // Jmp a NNN |
| 0x2NNN | // Chiama subroutin a NNN ! *(0xNNN)(); |
| 0x3XNN | // Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN) |
| 0x4XNN | // Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN) |
| 0x5XY0 | // Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y]) |