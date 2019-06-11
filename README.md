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

| OPCODE | Tipo | Descrizione |
| ----- | ----- | ----- |
| 0x0NNN | Call | // Chiama dal RCA 1802 le istruzioni all'indirizzo NNN ! Non utilizzato |
| 0x00E0 | Display | // Pulisce lo schermo |
| 0x00EE | Flusso | // Ret |
| 0x1NNN | Flusso | // Jmp a NNN |
| 0x2NNN | Flusso | // Chiama subroutin a NNN ! *(0xNNN)(); |
| 0x3XNN | Condizione | // Salta l'istruzione successiva se il registro VX è uguale a NN ! if(V[X] == NN) |
| 0x4XNN | Condizione | // Salta l'istruzione successiva se il registro VX non è uguale a NN ! if(V[X] != NN) |
| 0x5XY0 | Condizione | // Salta l'istruzione successiva se il registro VX è uguale all' registro VY ! if(V[X] == V[Y]) |
| 0x6XNN | Condizione | // Assegna il valore NN al registro V[X] |
| 0x7XNN | Condizione | // Aggiunge il valore NN al registro V[X] (Carry Flag immutata) |
| 0x8XY0 | Assegnazione | // Assegna il valore V[Y] a V[X] |
| ... | ... | ... |