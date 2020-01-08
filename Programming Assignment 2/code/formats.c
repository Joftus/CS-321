#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"

extern struct EMU emu;

int toDecimal(char *in, int len) {
	int index, total = 0;
	for (index = 0; index < len; index++) {
		switch (in[index]) {
			case '1':
				total += (int) pow(2, len - index - 1);
		}
	}
	return total;
}

int twosToDecimal(char *in, int len) {
	int out, index;
	char *tmp = malloc(len - 1);
	for (index = 1; index < len; index++) {
		switch (in[index]) {
			case '0':
				{
					tmp[index - 1] = '1';
					break;
				}
			case '1':
				{
					tmp[index - 1] = '0';
					break;
				}
		}
	}
	out = toDecimal(tmp, len - 1);
	switch (in[0]) {
		case '0':
			{
				return out;
			}
		case '1':
			{
				return out * -1;
			}

	}
	return 0;
}

/*
11 bit opcode, 5 bit Rm, 6 bit shamt, 5 bit Rn, 5 bit Rd
*/
struct Form_R format_R(char* in) {
	struct Form_R out;
	int index = 0, total = 0;

	out.Rm_len = 5;
	out.shamt_len = 6;
	out.Rn_len = 5;
	out.Rd_len = 5;
	out.Rm = malloc(out.Rm_len);
	out.shamt = malloc(out.shamt_len);
	out.Rn = malloc(out.Rn_len);
	out.Rd = malloc(out.Rd_len);

	total += out.Rm_len;
	while (index < total) {
		out.Rm[index] = in[index];
		index ++;
	}
	total += out.shamt_len;
	while (index < total) {
		out.shamt[out.shamt_len - (total - index)] = in[index];
		index ++;
	}
	total += out.Rn_len;
	while (index < total) {
		out.Rn[out.Rn_len - (total - index)] = in[index];
		index ++;
	}
	total += out.Rd_len;
	while (index < total) {
		out.Rd[out.Rd_len - (total - index)] = in[index];
		index ++;
	}

	out.Rm_d = toDecimal(out.Rm, out.Rm_len);
	out.shamt_d = toDecimal(out.shamt, out.shamt_len);
	out.Rn_d = toDecimal(out.Rn, out.Rn_len);
	out.Rd_d = toDecimal(out.Rd, out.Rd_len);
	return out;
}


/*
10 bit opcode, 12 bit immediate, 5 bit Rn, 5 bit Rd
*/
struct Form_I format_I(char* in) {
	int index = 0, total = 0;
	struct Form_I out;

	out.ALU_len = 12;
	out.Rn_len = 5;
	out.Rd_len = 5;
	out.ALU_immediate = malloc(out.ALU_len);
	out.Rn = malloc(out.Rn_len);
	out.Rd = malloc(out.Rd_len);
	total += out.ALU_len;
	while (index < total) {
		out.ALU_immediate[index] = in[index];
		index ++;
	}
	total += out.Rn_len;
	while (index < total) {
		out.Rn[out.Rn_len - (total - index)] = in[index];
		index ++;
	}
	total += out.Rd_len;
	while (index < total) {
		out.Rd[out.Rd_len - (total - index)] = in[index];
		index ++;
	}
	out.ALU_d = toDecimal(out.ALU_immediate, out.ALU_len);
	out.Rn_d = toDecimal(out.Rn, out.Rn_len);
	out.Rd_d = toDecimal(out.Rd, out.Rd_len);
	return out;
}

/*
11 bit opcode, 9 bit offset, 2 bit op, 5 bit Rn, 5 bit Rt
*/
struct Form_D format_D(char* in) {
	struct Form_D out;
	int index = 0, total = 0;

	out.DT_address_len = 9;
	out.op_len = 2;
	out.Rn_len = 5;
	out.Rt_len = 5;
	out.DT_address = malloc(out.DT_address_len);
	out.op = malloc(out.op_len);
	out.Rn = malloc(out.Rn_len);
	out.Rt = malloc(out.Rt_len);

	total += out.DT_address_len;
	while (index < total) {
		out.DT_address[index] = in[index];
		index ++;
	}
	total += out.op_len;
	while (index < total) {
		out.op[out.op_len - (total - index)] = in[index];
		index ++;
	}
	total += out.Rn_len;
	while (index < total) {
		out.Rn[out.Rn_len - (total - index)] = in[index];
		index ++;
	}
	total += out.Rt_len;
	while (index < total) {
		out.Rt[out.Rt_len - (total - index)] = in[index];
		index ++;
	}

	out.DT_address_d = toDecimal(out.DT_address, out.DT_address_len);
	out.op_d = toDecimal(out.op, out.op_len);
	out.Rn_d = toDecimal(out.Rn, out.Rn_len);
	out.Rt_d = toDecimal(out.Rt, out.Rt_len);
	return out;
}

/*
6 bit opcode 000101, 26 bit address
*/
struct Form_B format_B(char* in) {
	struct Form_B out;
	int index = 0, total = 0;

	out.BR_address_len = 26;
	out.BR_address = malloc(out.BR_address_len);

	total += out.BR_address_len;
	while (index < total) {
		out.BR_address[index] = in[index];
		index ++;
	}

	if (toDecimal(out.BR_address, out.BR_address_len) >= emu.file_size) {
		out.BR_address_d = twosToDecimal(out.BR_address, out.BR_address_len);
	}
	else out.BR_address_d = toDecimal(out.BR_address, out.BR_address_len);
	return out;
}

/*
19 bit address, 5 bit Rt
*/
struct Form_CB format_CB(char* in) {
	struct Form_CB out;
	int index = 0, total = 0;

	out.COND_BR_address_len = 19;
	out.Rt_len = 5;
	out.COND_BR_address = malloc(out.COND_BR_address_len);
	out.Rt = malloc(out.Rt_len);

	total += out.COND_BR_address_len;
	while (index < total) {
		out.COND_BR_address[index] = in[index];
		index ++;
	}
	total += out.Rt_len;
	while (index < total) {
		out.Rt[out.Rt_len - (total - index)] = in[index];
		index ++;
	}

	//out.COND_BR_address_d = toDecimal(out.COND_BR_address, out.COND_BR_address_len);
	if (toDecimal(out.COND_BR_address, out.COND_BR_address_len) >= emu.file_size) {
		out.COND_BR_address_d = twosToDecimal(out.COND_BR_address, out.COND_BR_address_len);
	}
	else out.COND_BR_address_d = toDecimal(out.COND_BR_address, out.COND_BR_address_len);
	out.Rt_d = toDecimal(out.Rt, out.Rt_len);
	return out;
}

/*
9 bit opcode, 2 bit op, 16 bit immediate, 5 bit Rd
*/
struct Form_IM format_IM(char* in) {
	struct Form_IM out;
	int index = 0, total = 0;

	// UNUSED
	printf("\n\n\n\n\nUSING FORM_IM - NEEDS EDITING\n\n\n\n\n");

	out.MOV_immediate_len = 16;
	out.Rd_len = 5;
	out.MOV_immediate = malloc(out.MOV_immediate_len);
	out.Rd = malloc(out.Rd_len);

	total += out.MOV_immediate_len;
	while (index < total) {
		out.MOV_immediate[index] = in[index];
		index ++;
	}
	total += out.Rd_len;
	while (index < total) {
		out.Rd[out.Rd_len - (total - index)] = in[index];
		index ++;
	}

	out.MOV_immediate_d = toDecimal(out.MOV_immediate, out.MOV_immediate_len);
	out.Rd_d = toDecimal(out.Rd, out.Rd_len);
	return out;
}