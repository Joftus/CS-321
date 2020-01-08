#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "formats.h"

#define MEMORY_SIZE 4096
#define STACK_SIZE 512

/*
Form_R Testing
printf("	Rm: %d, %s\n", out.Rm_d, out.Rm);
	printf("	shamt: %d, %s\n", out.shamt_d, out.shamt);
	printf("	Rn: %d, %s\n", out.Rn_d, out.Rn);
	printf("	Rd: %d, %s\n\n\n", out.Rd_d, out.Rd);

Form_I Testing
printf("	ALU: %d, %s\n", out.ALU_d, out.ALU_immediate);
	printf("	Rn: %d, %s\n", out.Rn_d, out.Rn);
	printf("	Rd: %d, %s\n\n\n", out.Rd_d, out.Rd);

Form_B Testing
printf("	BR: %d, %s\n\n", out.BR_address_d, out.BR_address);

Form_CB Testing
printf("	COND BR: %d, %s\n", out.COND_BR_address_d, out.COND_BR_address);
	printf("	Rt: %d, %s\n\n", out.Rt_d, out.Rt);

Form_D Testing
printf("	DT_address: %d, %s\n", out.DT_address_d, out.DT_address);
	printf("	op: %d, %s\n", out.op_d, out.op);
	printf("	Rn: %d, %s\n", out.Rn_d, out.Rn);
	printf("	Rt: %d, %s\n\n\n", out.Rt_d, out.Rt);
*/

extern struct EMU emu;

char* dtoBinary(uint32_t a)
{
    uint32_t i;
    char* t = malloc(64);
    int count = 0;

    for(i=0x80000000;i!=0;i>>=1) {
        t[count] = (a&i)?'1':'0';
        count++;
    }
    return t;
}

char *current_cond;
int parse_cond(int cond) {
	// 0 = false, 1 = true
	if (emu.cond_line_set + 1 != emu.line_num) return 0;
	int S = emu.cond_val;
	current_cond = malloc(16);
	current_cond = "";
	emu.cond_line_set ++;
	switch (cond) {
		case 0:		// EQ (Equal)
			{
				current_cond = "EQ";
				if (S == 0) return 1;
				else return 0;
			}
		case 1:		// NE (Not Equal)
			{
				current_cond = "NE";
				if (S != 0) return 1;
				else return 0;
			}
		case 2:		// HS (Unsigned higer or same)
			{
				current_cond = "HS";
				if (abs(S) >= 0) return 1;
				else return 0;
			}
		case 3:		// LO (Unsigned lower)
			{
				current_cond = "LO";
				if (abs(S) < 0) return 1;
				else return 0;
			}
		case 4:		// MI (Negative)
			{
				current_cond = "MI";
				if (S < 0) return 1;
				else return 0;
			}
		case 5:		// PL (Positive or Zero)
			{
				current_cond = "PL";
				if (S >= 0) return 1;
				else return 0;
			}
		case 6:		// VS (Signed overflow)
			{
				return 0;
			}
		case 7:		// VC (No Overflow)
			{
				return 0;
			}
		case 8:		// HI (Unsigned higher)
			{
				current_cond = "HI";
				if (abs(S) > 0) return 1;
				else return 0;
			}
		case 9:		// LS (Unsigned lower or same)
			{
				current_cond = "LS";
				if (abs(S) <= 0) return 1;
				else return 0;
			}
		case 10:	// GE (Greater than or equal to)
			{
				current_cond = "GE";
				if (S >= 0) return 1;
				else return 0;
			}
		case 11:	// LT (Less than)
			{
				current_cond = "LT";
				if (S < 0) return 1;
				else return 0;
			}
		case 12:	// GT (Greather than)
			{
				current_cond = "GT";
				if (S > 0) return 1;
				else return 0;
			}
		case 13:	// LE (Less than or equal to)
			{
				current_cond = "LE";
				if (S <= 0) return 1;
				else return 0;
			}
	}
	return 0;
}

void control_check(int alu_mem_cb, int reg_1, int reg_2, int reg_3) {
	if (alu_mem_cb == 3) {
		emu.bypass_bubbles += (3 - emu.change_cc_b);
		emu.no_bypass_bubbles += (3 - emu.change_cc_nb);
		emu.change_cc_nb = 3;
		emu.change_cc_b = 3;
		emu.control_hazards++;
	}
}

void data_check(int alu_mem_cb, int reg_1, int reg_2, int reg_3) {
	int uses_olb_reg = 0;
	int uses_tlb_reg = 0;
	if ((emu.olb_reg_1 == reg_1 || emu.olb_reg_1 == reg_2 || emu.olb_reg_1 == reg_3) && emu.olb_reg_1 != 31)
		uses_olb_reg = 1;
	if ((emu.tlb_reg_1 == reg_1 || emu.tlb_reg_1 == reg_2 || emu.tlb_reg_1 == reg_3) && emu.olb_reg_1 != 31)
		uses_tlb_reg = 1;
	if (emu.one_line_before == 2 && uses_olb_reg != 0) {
		emu.no_bypass_bubbles += 2;
		emu.bypass_bubbles += 1;
		emu.change_cc_nb = 2;
		emu.change_cc_b = 1;
		emu.data_hazards++;
	}
	if (emu.one_line_before == 1 && uses_olb_reg != 0) {
		if (2 - emu.change_cc_nb > 0) {
			emu.no_bypass_bubbles += 2;
			emu.change_cc_nb = 2;
			emu.data_hazards++;
		}
		else emu.unused_data_hazards++;
	}
	if (emu.two_lines_before == 2 && uses_tlb_reg != 0) {
		if (1 - emu.change_cc_nb > 0) {
			emu.no_bypass_bubbles += 1;
			emu.change_cc_nb = 1;
			emu.data_hazards++;
		}
		emu.unused_data_hazards++;
	}
	if (emu.two_lines_before == 1 && uses_tlb_reg != 0) {
		if (2 - emu.change_cc_nb > 0) {
			emu.no_bypass_bubbles += 1;
			emu.change_cc_nb = 1;
			emu.data_hazards++;
		}
		emu.unused_data_hazards++;
	}
}

void logger(int alu_mem_cb, int reg_1, int reg_2, int reg_3) {
	// 1 == alu, 2 == mem, 3 == cb
	data_check(alu_mem_cb, reg_1, reg_2, reg_3);
	control_check(alu_mem_cb, reg_1, reg_2, reg_3);
	emu.two_lines_before = emu.one_line_before;
	emu.tlb_reg_1 = emu.olb_reg_1;
	emu.tlb_reg_2 = emu.olb_reg_2;
	emu.tlb_reg_3 = emu.olb_reg_3;

	emu.olb_reg_1 = reg_1;
	emu.olb_reg_2 = reg_2;
	emu.olb_reg_3 = reg_3;
	emu.one_line_before = alu_mem_cb;
}

void original_line_builder(int op, int val1, int val2, int val3) {
	char *result = malloc(32);
	switch(op) {
		case 1:
			{
				char inst[] = "ADD X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 2:
			{
				char inst[] = "ADDI X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 3:
			{
				char inst[] = "AND X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 4:
			{
				char inst[] = "ANDI X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 5:
			{
				emu.original_lines[emu.lines_run] = "B\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 6:
			{
				if (emu.cond_line_set + 1 == emu.line_num) {
					char inst[] = "B.%s %d\0";
					sprintf(result, inst, current_cond, emu.cond_val);
					emu.original_lines[emu.lines_run] = result;
				}
				logger(3, val1, val2, val3);
				return;
			}
		case 7:
			{
				emu.original_lines[emu.lines_run] = "BL\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 8:
			{
				emu.original_lines[emu.lines_run] = "BR\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 9:
			{
				char inst[] = "CBNZ %d\0";
				sprintf(result, inst, val1);
				emu.original_lines[emu.lines_run] = result;
				logger(3, val1, val2, val3);
				return;
			}
		case 10:
			{
				char inst[] = "CBZ %d\0";
				sprintf(result, inst, val1);
				emu.original_lines[emu.lines_run] = result;
				logger(3, val1, val2, val3);
				return;
			}
		case 11:
			{
				emu.original_lines[emu.lines_run] = "DUMP\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 12:
			{
				char inst[] = "EOR X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 13:
			{
				char inst[] = "EORI X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 14:
			{
				emu.original_lines[emu.lines_run] = "HALT\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 15:
			{
				char inst[] = "LDUR X%d, [X%d, #%d]\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(2, val1, val2, val3);
				return;
			}
		case 16:
			{
				char inst[] = "LSL X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 17:
			{
				char inst[] = "LSR X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 18:
			{
				char inst[] = "MUL X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 19:
			{
				char inst[] = "ORR X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 20:
			{
				char inst[] = "ORRI X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 21:
			{
				emu.original_lines[emu.lines_run] = "PRNL\0";
				logger(0, val1, val2, val3);
				return;
			}
		case 22:
			{
				char inst[] = "PRNT X%d\0";
				sprintf(result, inst, val1);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 23:
			{
				char inst[] = "STUR X%d, [X%d, #%d]\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(2, val1, val2, val3);
				return;
			}
		case 24: 
			{
				char inst[] = "SUB X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 25:
			{
				char inst[] = "SUBI X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 26:
			{
				char inst[] = "SUBIS X%d, X%d, #%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(1, val1, val2, val3);
				return;
			}
		case 27:
			{
				char inst[] = "SUBS X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
		case 28:
			{
				char inst[] = "UDIV X%d, X%d, X%d\0";
				sprintf(result, inst, val1, val2, val3);
				emu.original_lines[emu.lines_run] = result;
				logger(0, val1, val2, val3);
				return;
			}
	}
}



// A
int ADD(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] + emu.registers[out.Rm_d];
	original_line_builder(1, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int ADDI(char* in) {
	struct Form_I out = format_I(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] + out.ALU_d;
	original_line_builder(2, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}

int AND(char* in) {
	struct Form_R out = format_R(in);
	char *result = malloc(32);		// register width = 32 bits
	char *num_1 = malloc(32);
	char *num_2 = malloc(32);
	int index;

	num_1 = dtoBinary(emu.registers[out.Rn_d]);
	num_2 = dtoBinary(emu.registers[out.Rm_d]);

	for (index = 1; index < 33; index++)
		result[32 - index] = (num_1[32 - index] == '1' && num_2[32 - index] == '1') ? '1' : '0';
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(3, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int ANDI(char* in) {
	struct Form_I out = format_I(in);
	char *result = malloc(32);		// register width = 32 bits
	char *tmp = malloc(32);
	int index;
	tmp = dtoBinary(emu.registers[out.Rn_d]);
	for (index = 1; index < 33; index++) {
		if (32 - index < out.ALU_len)
			result[32 - index] = (tmp[32 - index] == '1' && out.ALU_immediate[32 - index] == '1') ? '1' : '0';
		else
			result[32 - index] = '0';
	}
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(4, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}



// B
int B(char* in) {
	struct Form_B out = format_B(in);
	emu.line_num += out.BR_address_d - 1;
	original_line_builder(5, 0, 0, 0);
	return 1;
}

int BCond(char* in) {
	struct Form_CB out = format_CB(in);
	if (parse_cond(out.Rt_d) == 1) emu.line_num += out.COND_BR_address_d - 1;
	original_line_builder(6, 0, 0, 0);
	return 1;
}

int BL(char* in) {
	struct Form_B out = format_B(in);
	emu.registers[30] = emu.line_num;
	emu.line_num = out.BR_address_d;
	original_line_builder(7, 0, 0, 0);
	return 1;
}

int BR(char* in) {
	struct Form_R out = format_R(in);
	emu.line_num = emu.registers[30];
	original_line_builder(8, 0, 0, 0);
	return 1;
}



// C
int CBNZ(char* in) {
	struct Form_CB out = format_CB(in);
	if (emu.registers[out.Rt_d] != 0) {
		if (out.COND_BR_address_d + emu.line_num > 0 && out.COND_BR_address_d + emu.line_num < emu.file_size)
			emu.line_num += out.COND_BR_address_d;
	}
	original_line_builder(9, emu.registers[out.Rt_d], 0, 0);
	return 1;
}

int CBZ(char* in) {
	struct Form_CB out = format_CB(in);
	if (emu.registers[out.Rt_d] == 0) {
		if (out.COND_BR_address_d + emu.line_num > 0 && out.COND_BR_address_d + emu.line_num < emu.file_size)
			emu.line_num += out.COND_BR_address_d;
	}
	original_line_builder(10, emu.registers[out.Rt_d], 0, 0);
	return 1;
}

int from_halt = 0;

// D
int DUMP() {
	if (from_halt == 0) original_line_builder(11, 0, 0, 0);
	int index;
	printf("\nRegisters:\n");
	for (index = 0; index < emu.num_registers - 5; index++) {
		if (index < 10) printf("      X%d:  %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 16) printf("(IP0) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 17) printf("(IP1) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 28) printf(" (SP) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 29) printf(" (FP) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 30) printf(" (LR) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else if (index == 31) printf("(XZR) X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
		else printf("      X%d: %016X (%d)\n", index, emu.registers[index], emu.registers[index]);
	}
	printf("\nStack:");
	for (index = 0; index < emu.stack_size; index++) {
		if (index % 16 == 0) printf("\n%08X  %02X ", index, emu.stack[index]);
		else if (index % 8 == 0) printf(" %02X ", emu.stack[index]);
		else printf("%02X ", emu.stack[index]);
	}
	printf("\n\nMain Memory:");
	for (index = 0; index < emu.memory_size; index++){
		if (index % 16 == 0) printf("\n%08X  %02X ", index, emu.memory[index]);
		else if (index % 8 == 0) printf(" %02X ", emu.memory[index]);
		else printf("%02X ", emu.memory[index]);
	}
	printf("\n\nProgram:\n");
	for (index = 0; index < emu.lines_run + 1; index++) {
		printf("    %s\n", emu.original_lines[index]);
	}
	printf("\n\n");
	return 1;
}



// E
int EOR(char* in) {
	struct Form_R out = format_R(in);
	char *result = malloc(32);		// register width = 32 bits
	char *num_1 = malloc(32);
	char *num_2 = malloc(32);
	int index;

	num_1 = dtoBinary(emu.registers[out.Rn_d]);
	num_2 = dtoBinary(emu.registers[out.Rm_d]);

	for (index = 1; index < 33; index++) {
		if (num_1[32 - index] == '1' && num_2[32 - index] == '0')
			result[32 - index] = '1';
		else if (num_1[32 - index] == '0' && num_2[32 - index] == '1')
			result[32 - index] = '1';
		else result[32 - index] = '0';
	}
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(12, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int EORI(char* in) {
	struct Form_I out = format_I(in);
	char *result = malloc(32);		// register width = 32 bits
	char *tmp = malloc(32);
	int index;
	tmp = dtoBinary(emu.registers[out.Rn_d]);
	for (index = 1; index < 33; index++) {
		if (32 - index < out.ALU_len) {
			if (tmp[32 - index] == '1' && out.ALU_immediate[32 - index] == '0')
				result[32 - index] = '1';
			else if (tmp[32 - index] == '0' && out.ALU_immediate[32 - index] == '1')
				result[32 - index] = '1';
			else result[32 - index] = '0';
		}
		else
			result[32 - index] = (tmp[32 - index] == '1') ? '1' : '0';
	}
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(13, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}



// H
int HALT(char* in) {
	original_line_builder(14, 0, 0, 0);
	from_halt = 1;
	DUMP();
	emu.exit_status = 1;
	return 1;
}



// L
int LDUR(char* in) {
	struct Form_D out = format_D(in);
	if (out.Rn_d == 28 || out.Rn_d == 29)
		emu.registers[out.Rt_d] = emu.stack[emu.registers[out.Rn_d] + out.DT_address_d];
	else 
		emu.registers[out.Rt_d] = emu.memory[emu.registers[out.Rn_d] + out.DT_address_d];
	original_line_builder(15, out.Rt_d, out.Rn_d, out.DT_address_d);
	return 1;
}

int LDURB(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int LDURH(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int LDURSW(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int LSL(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] << out.shamt_d;
	original_line_builder(16, out.Rd_d, out.Rn_d, out.shamt_d);
	return 1;
}

int LSR(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] >> out.shamt_d;
	original_line_builder(17, out.Rd_d, out.Rn_d, out.shamt_d);
	return 1;
}



// M
int MUL(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] * emu.registers[out.Rm_d];
	original_line_builder(18, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}



// O
int ORR(char* in) {
	struct Form_R out = format_R(in);
	char *result = malloc(32);		// register width = 32 bits
	char *num_1 = malloc(32);
	char *num_2 = malloc(32);
	int index;

	num_1 = dtoBinary(emu.registers[out.Rn_d]);
	num_2 = dtoBinary(emu.registers[out.Rm_d]);

	for (index = 1; index < 33; index++) 
		result[32 - index] = (num_1[32 - index] == '1' || num_2[32 - index] == '1') ? '1' : '0';
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(19, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int ORRI(char* in) {
	struct Form_I out = format_I(in);
	char *result = malloc(32);		// register width = 32 bits
	char *tmp = malloc(32);
	int index;

	tmp = dtoBinary(emu.registers[out.Rn_d]);
	for (index = 1; index < 33; index++) {
		if (32 - index < out.ALU_len)
			result[32 - index] = (tmp[32 - index] == '1' || out.ALU_immediate[32 - index] == '1') ? '1' : '0';
		else 
			result[32 - index] = (tmp[32 - index] == '1') ? '1' : '0';
	}
	emu.registers[out.Rd_d] = toDecimal(result, 32);
	original_line_builder(20, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}



// P
int PRNL(char* in) {
	printf("\n");
	original_line_builder(21, 0, 0, 0);
	return 1;
}

int PRNT(char* in) {
	struct Form_R out = format_R(in);
	original_line_builder(22, 0, 0, 0);
	printf("X%d:  %016X (%d)\n", out.Rd_d, emu.registers[out.Rd_d], emu.registers[out.Rd_d]);
	return 1;
}



// S
int SDIV(char* in) {
	return 1;
}

int SMULH(char* in) {
	struct Form_R out = format_R(in);
	return 1;
}

int STUR(char* in) {
	struct Form_D out = format_D(in);
	if (out.Rn_d == 28 || out.Rn_d == 29)
		emu.registers[out.Rt_d] = emu.stack[emu.registers[out.Rn_d] + out.DT_address_d];
	else 
		emu.registers[out.Rt_d] = emu.memory[emu.registers[out.Rn_d] + out.DT_address_d];
	original_line_builder(23, out.Rt_d, out.Rn_d, out.DT_address_d);
	return 1;
}

int STURB(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int STURH(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int STURW(char* in) {
	struct Form_D out = format_D(in);
	return 1;
}

int SUB(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] - emu.registers[out.Rm_d];
	original_line_builder(24, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int SUBI(char* in) {
	struct Form_I out = format_I(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] - out.ALU_d;
	original_line_builder(25, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}

int SUBIS(char* in) {
	struct Form_I out = format_I(in);
	uint32_t num_1 = emu.registers[out.Rn_d];
	uint32_t num_2 = out.ALU_d;
	emu.cond_line_set = emu.line_num;
	emu.cond_val = num_1 - num_2;
	emu.registers[out.Rd_d] = num_1 - num_2;
	original_line_builder(26, out.Rd_d, out.Rn_d, out.ALU_d);
	return 1;
}

int SUBS(char* in) {
	struct Form_R out = format_R(in);
	uint32_t num_1 = emu.registers[out.Rn_d];
	uint32_t num_2 = emu.registers[out.Rm_d];
	emu.cond_line_set = emu.line_num;
	emu.cond_val = num_1 - num_2;
	emu.registers[out.Rd_d] = num_1 - num_2;
	original_line_builder(27, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}



// U
int UDIV(char* in) {
	struct Form_R out = format_R(in);
	emu.registers[out.Rd_d] = emu.registers[out.Rn_d] / emu.registers[out.Rm_d];
	original_line_builder(28, out.Rd_d, out.Rn_d, out.Rm_d);
	return 1;
}

int UMULH(char* in) {
	struct Form_R out = format_R(in);
	return 1;
}