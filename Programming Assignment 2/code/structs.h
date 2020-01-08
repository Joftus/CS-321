#include <stdint.h>

struct Form_R {
	char *Rm;
	char *shamt;
	char *Rn;
	char *Rd;
	int Rm_d;
	int shamt_d;
	int Rn_d;
	int Rd_d;
	int Rm_len;
	int shamt_len;
	int Rn_len;
	int Rd_len;
};

struct Form_I {
	char *ALU_immediate;
	char *Rn;
	char *Rd;
	int ALU_d;
	int Rn_d;
	int Rd_d;
	int ALU_len;
	int Rn_len;
	int Rd_len;
};

struct Form_D {
	char *DT_address;
	char *op;
	char *Rn;
	char *Rt;
	int DT_address_d;
	int op_d;
	int Rn_d;
	int Rt_d;
	int DT_address_len;
	int op_len;
	int Rn_len;
	int Rt_len;
};

struct Form_B {
	char *BR_address;
	int BR_address_len;
	int BR_address_d;
};

struct Form_CB {
	char *COND_BR_address;
	char *Rt;
	int COND_BR_address_d;
	int Rt_d;
	int COND_BR_address_len;
	int Rt_len;
};

struct Form_IM {
	char *MOV_immediate;
	char *Rd;
	int MOV_immediate_d;
	int Rd_d;
	int MOV_immediate_len;
	int Rd_len;
};



struct EMU {
	// General Info
	int file_size;

	// Structure
	uint8_t *stack;
	uint8_t *memory; // was 64
	uint32_t *registers;
	int stack_size;
	int memory_size;
	int num_registers;
	int exit_status;

	// Line Tracking
	int line_num;

	// Cond Handeling
	int cond_line_set;
	int cond_val;

	// For DUMP Program output
	const char *original_lines[1000];
	int lines_run;

	// Stats
	int no_bypass;
	int bypass;
	int data_hazards;
	int unused_data_hazards;
	int control_hazards;

	// Hazard Checking
	int two_lines_before;
	int tlb_reg_1;
	int tlb_reg_2;
	int tlb_reg_3;
	int one_line_before;
	int olb_reg_1;
	int olb_reg_2;
	int olb_reg_3;
	int no_bypass_bubbles;
	int bypass_bubbles;
	int change_cc_nb;
	int change_cc_b;
};