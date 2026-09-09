#include <stdio.h>

unsigned char Data_memory[64000]; 
typedef struct {
    unsigned int pc; 
    unsigned int regs[16]; 
} CPU;

void iniciar_cpu(CPU *c) {
    c->pc = 0; 
    for(int i = 0; i < 16; i++){
        c->regs[i] = 0;
    }
}

void procesador_run(CPU *c) {
    int prendido = 1;
    while(prendido && c->pc < 16384) {
        unsigned int instruccion = program_memory[c->pc];
        c->pc++;
        unsigned int paquete1_op = (instruccion >> 16) & 0xF;
        unsigned int paquete2_rd = (instruccion >> 12) & 0xF;
        unsigned int paquete3_rs = (instruccion >> 8)  & 0xF;
        unsigned int paquete4_rt = (instruccion >> 4)  & 0xF;
        unsigned int los_otros_8_bits = instruccion & 0xFF;

        switch (paquete1_op) {
            case 0:
                c->regs[paquete2_rd] = c->regs[paquete3_rs] + c->regs[paquete4_rt];
                break;
            case 2:
                c->regs[paquete2_rd] = Data_memory[c->regs[paquete3_rs] + los_otros_8_bits];
                break;

            case 3:
                c->regs[paquete2_rd] = Data_memory[c->regs[paquete3_rs] + los_otros_8_bits];
                break;

            case 4:
                if (c->regs[paquete2_rd] == c->regs[paquete3_rs]) {
                    c->pc = los_otros_8_bits; // Salta a la dirección
                }
                break;
            case 14:
                c->pc = instruccion & 0xFFFF; // Usa los últimos 16 bits para el salto
                break;
                
            case 15:
                prendido = 0;
                break;
            default:
                break;
        }
    }
}

int main() {
    CPU micpu;
    iniciar_cpu(&micpu);

    program_memory[0] = (0 << 16) | (1 << 12) | (2 << 8) | (3 << 4) | 0;
    
    program_memory[1] = (15 << 16); 

    procesador_run(&micpu);

    return 0;
}
