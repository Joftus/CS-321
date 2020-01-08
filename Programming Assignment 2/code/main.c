#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "parse.h"
#include "structs.h"

#define MEMORY_SIZE 4096
#define STACK_SIZE 512
#define NUM_REGISTERS 37
#define NUM_BRANCHES 16

struct EMU emu;

char* toBinary(uint32_t a)
{
    uint32_t i;
    char* t = malloc(32);
    int count = 0;

    for(i=0x80000000;i!=0;i>>=1) {
        t[count] = (a&i)?'1':'0';
        count++;
    }
    return t;
}



void stats() {
	printf("\n\n\n-------| PROGRAM STATISTICS |-------");
	printf("\n  Cycles (unpipelined): %d\n", emu.lines_run);
	// + 4 because a 5-stage pipeline adds 4 trailing clock cycles
	printf("  Cycles (5-stage, no bypassing): %d\n", emu.lines_run + 4 + emu.no_bypass_bubbles);
	printf("  Cycles (5-stage, bypassing): %d\n", emu.lines_run + 4 + emu.bypass_bubbles);
	printf("  Data Hazards (5-stage): %d\n", emu.data_hazards + emu.unused_data_hazards);
	printf("  Control Hazards (5-stage): %d\n\n\n", emu.control_hazards);
	exit(1);
	return;
}



int main( int argc, char *argv[] ) {
    uint32_t* buffer;
    FILE *ptr;
	emu.memory = malloc(MEMORY_SIZE * 8);
	emu.memory_size = MEMORY_SIZE;
	emu.stack = malloc(STACK_SIZE);
	emu.stack_size = STACK_SIZE;
	emu.registers = malloc(NUM_REGISTERS * 8);
	emu.registers[28] = STACK_SIZE;
	emu.registers[29] = STACK_SIZE;
	emu.num_registers = NUM_REGISTERS;

	if (argc > 1)
		ptr = fopen(argv[1], "rb");
	else {
		exit(0);
		//ptr = fopen("cbz-cbnz_test.legv8asm.machine","rb");
		// ptr = fopen("andi.txt.machine","rb");
		// ptr = fopen("assignment1.legv8asm.machine","rb");
	}
	
	fseek(ptr, 0L, SEEK_END);
	int sz = ftell(ptr);
	buffer = malloc(sz);
	fseek(ptr, 0L, SEEK_SET);
    fread(buffer,1,sz,ptr);

	emu.line_num = 0;
	emu.lines_run = 0;
	for (int tmp = 0; tmp < 1000; tmp++) {
		emu.original_lines[tmp] = malloc(32);
	}
	emu.file_size = sz / 4;

	// printf("\n\n\n\n\n---------------START---------------\n");
	// printf("Total Lines: %d\n\n\n", emu.file_size);
	int counter = 0;
	
	while (emu.line_num < sz / 4 && counter < 100000) {
		parse_cmd(toBinary(__builtin_bswap32(buffer[emu.line_num])));
		emu.registers[28] = STACK_SIZE - 1;
		if (emu.exit_status != 0) {
			break;
		}
		emu.line_num++;
		emu.lines_run++;
		counter ++;
	}
	stats();
	if (counter == 99999) printf("WARNING: infinite loop found in file.");
	// printf("\nFinal Line Number: %d", emu.line_num);
	// printf("\n---------------END---------------\n\n\n");
}