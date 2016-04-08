#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#define S_MEMSIZE 2048

uint16_t mem_org[S_MEMSIZE];
uint16_t mem_chg[S_MEMSIZE];
uint16_t regs[4];
uint16_t PC;

void trace() {
    int i;

    printf("Reg A: %04x\n", regs[0]);
    printf("Reg B: %04x\n", regs[1]);
    printf("Reg C: %04x\n", regs[2]);
    printf("Reg D: %04x\n", regs[3]);
    printf("PC: %04x\n", PC);

    printf("Different memory locations:\n");

    for (i = 0; i < S_MEMSIZE; i++) {
        if (mem_org[i] != mem_chg[i]) {
            printf("Addr: %03x Old Val: %04x New Val: %04x\n", i, mem_org[i], mem_chg[i]);
        }
    }
}

int main (int argc, char** argv) {
    int mem_fd;
    uint16_t singleword;
    int16_t a, b;
    char word[5];
    char beep;

    int i;

    int readval;

    uint16_t inst = 0;
    uint16_t opcode = 0;
    uint16_t r1 = 0;
    uint16_t r2 = 0;
    uint16_t imm = 0;
    uint16_t control = 0;
    uint16_t leave = 0;

    for (i = 0; i < S_MEMSIZE; i++) {
        mem_org[i] = 0;
        mem_chg[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        regs[i] = 0;
    }

    mem_fd = open("memfile", O_RDONLY);

    if (mem_fd < 0) {
        printf("Error opening memfile!\n");
        exit(2);
    }
    /* Read from file into memory */
    readval = 1;
    i = 0;
    do {
        readval = read(mem_fd, word, 4);
        readval = read(mem_fd, word, 4);
        readval = read(mem_fd, &beep, 1);
        word[4] = 0;
        singleword = (uint16_t)strtol(word, NULL, 16);
        mem_org[i] = singleword;
        mem_chg[i] = singleword;
        i++;

        readval = read(mem_fd, &beep, 1);
        lseek(mem_fd, -1, SEEK_CUR);
    } while(readval > 0);    
  
    close(mem_fd);

    PC = 0;
    inst = 0;
    opcode = 0;
    r1 = 0;
    r2 = 0;
    imm = 0;
    leave = 0;

    while (leave == 0) {
        inst = mem_chg[PC];
        opcode = (inst >> 13) & 0x7;
        r1 = (inst >> 11) & 0x3;
        r2 = (inst >> 9) & 0x3;
        imm = (inst >> 1) & 0xff;
        control = inst & 0x1;
        PC++;

        /*
        printf("inst  =0x%08x\n", inst);
        printf("opcode=%d\n", opcode);
        printf("control=%d\n", control);
        printf("r1    =%d\n", r1);
        printf("r2    =%d\n", r2);
        printf("imm  =0x%08x\n", imm);
        printf("PC    =%d\n", PC);
        printf("regs[A] = %d\n", regs[0]);
        printf("regs[B] = %d\n", regs[1]);
        printf("regs[C] = %d\n", regs[2]);
        printf("regs[D] = %d\n\n", regs[3]);
        */

        switch(opcode) {
            case 0: // ADD SUB
                if (control == 0) 
                    regs[r1] = regs[r1] + regs[r2] + imm;
                else
                    regs[r1] = regs[r1] - regs[r2] - imm;
                break;
            case 1: // AND NAND
                if (control == 0)
                    regs[r1] = regs[r1] & regs[r2];
                else
                    regs[r1] = ~(regs[r1] & regs[r2]);
                break;
            case 2: // OR NOR
                if (control == 0)
                    regs[r1] = regs[r1] | regs[r2];
                else
                    regs[r1] = ~(regs[r1] | regs[r2]);
                break;
            case 3: // SLT SGT
                if (control == 0)
                    regs[r1] = regs[r1] < regs[r2];
                else
                    regs[r1] = regs[r1] > regs[r2];
                break;
            case 4: // BEQ BNE
                if (imm & 128)
                    imm |= 0xff00;
                if (control == 0) {
                    if (regs[r1] == regs[r2]) {
                        PC = PC + imm;
                    }
                }
                else {
                    if (regs[r1] != regs[r2]) {
                        PC = PC + imm;
                    }
                }
                break;
            case 5: // LOAD STORE
                if (imm & 128)
                    imm |= 0xff00;
                imm = regs[r2] + imm;
                if (control == 0) {
                    if (imm >= S_MEMSIZE) {
                        printf("Memory access out of bounds (load).\n");
                        trace();
                        exit(1);
                    }
                    regs[r1] = mem_chg[imm]; 
                }
                else {
                    if (imm >= S_MEMSIZE) {
                        printf("Memory access out of bounds (store).\n");
                        trace();
                        exit(1);
                    }
                    mem_chg[imm] = regs[r1]; 
                }
                break;
            case 6: // CALL RETURN
                if (control == 0) {
                    regs[r1] = PC;
                    PC = imm;
                }
                else {
                    if (r2 == 3) {
                        leave = 1;
                    }
                    else if (r2 == 1) {
                        trace();
                    }
                    else {
                        PC = regs[r1];
                    }
                }
                break;
            case 7: // SLL SRL
                imm &= 0xf;
                if (control == 0) {
                    regs[r1] = regs[r2] << imm;
                }
                else {
                    regs[r1] = regs[r2] >> imm;
                }
                break;
        }
    }    
    trace();
}
