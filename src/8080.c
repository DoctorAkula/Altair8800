#include"8080.h"
#include"flags.h"

/*1 if there is a pending interrupt, 0 if not*/
static uint8_t intteruptPending = 0;
/*Opcode placed on the data bus by interrupting device*/
static uint8_t devOpcode = 0;

/*Returns amount of tstates*/
/*helpful macros*/
#define PUSH(ITEM) writeWRAM(cpu->RAM, cpu->SP -= 2, ITEM)
#define POP 	readWRAM(cpu->RAM, cpu->SP);\
		cpu->SP += 2
#define JMP	temp = readWRAM(cpu->RAM, cpu->PC);\
		cpu->PC = temp
#define RST(I)  PUSH(cpu->PC);\
		cpu->PC = I * 8
#define INR(R)	cpu->R++;\
		cpu->F &= 0b00000011;\
		cpu->F |= INR_FLAGS[cpu->R]
#define DCR(R)	cpu->R--;\
		cpu->F &= 0b00000011;\
		cpu->F |= DCR_FLAGS[cpu->R]
#define DAD(R)	temp = cpu->HL + cpu->R;\
		cpu->F = (temp >> 16) | (cpu->F &~1);\
		cpu->HL = temp
#define ADD(R)	cpu->F &= 0b00000010;\
		temp = cpu->R + cpu->A;\
		cpu->F |= temp >> 8;\
		cpu->F |= ((cpu->R & 0xF) + (cpu->A & 0xF) & 0x10);\
		cpu->A = temp;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define ADC(R)	temp = cpu->R;\
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;\
		cpu->F &= 0b00000010;\
		cpu->F |= (temp + cpu->A) >> 8;\
		cpu->F |= ((temp & 0xF) + (cpu->A & 0xF) & 0x10);\
		cpu->A += temp;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define SUB(R)	cpu->F &= 0b00000010;\
		temp = cpu->A - cpu->R;\
		cpu->F |= temp >> 8 & 0x1;\
		cpu->F |= ((cpu->A & 0xF) - (cpu->R & 0xF) & 0x10);\
		cpu->A = temp;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define SBB(R)	temp = cpu->R;\
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;\
		cpu->F &= 0b00000010;\
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;\
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);\
		cpu->A -= temp;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define ANA(R)	cpu->F &= 0b00000010;\
		cpu->F |= ((cpu->A | cpu->R) & 0x8) << 1; /*Aux carry is equal to OR of bit 3 of operand and accumulator*/\
		cpu->A &= cpu->R;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define XRA(R)	cpu->F &= 0b00000010;\
		cpu->A ^= cpu->R;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define ORA(R)	cpu->F &= 0b00000010;\
		cpu->A |= cpu->R;\
		cpu->F |= SZP_FLAGS[cpu->A]
#define CMP(R)	cpu->F &= 0b00000010;\
		temp = (cpu->A - cpu->R);\
		cpu->F |= temp >> 8 & 0x1;\
		cpu->F |= ((cpu->A & 0xF) - (cpu->R & 0xF) & 0x10);\
		cpu->F |= SZP_FLAGS[(uint8_t)temp]
void setInterruptPending(uint8_t busAssert)
{
	intteruptPending = 1;
	devOpcode = busAssert;
}

int singleStep(i8080 *cpu)
{
	cpu->halt = 0;
	int clks;
	uint32_t temp;
	uint8_t opcode;
	if(cpu->inte && intteruptPending){
		cpu->inte = 0;
		intteruptPending = 0;
		opcode = devOpcode;
	}else{
		opcode = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
	}
	switch(opcode)
	{
		case 0x0:	/*NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x1:	/*LXI B, D16*/
		cpu->BC = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x2:	/*STAX B*/
		writeRAM(cpu->RAM, cpu->BC, cpu->A);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x3:	/*INX B*/
		cpu->BC++;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4:	/*INR B*/
		INR(B);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5:	/*DCR B*/
		DCR(B);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6:	/*MVI B, D8*/
		cpu->B = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x7:	/*RLC*/
		cpu->F = (cpu->A >> 7) | (cpu->F &~1);
		cpu->A = (cpu->A << 1) | (cpu->F & 1);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9:	/*DAD B*/
		DAD(BC);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xa:	/*LDAX B*/
		cpu->A = readRAM(cpu->RAM, cpu->BC);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xb:	/*DCX B*/
		cpu->BC--;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xc:	/*INR C*/
		INR(C);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xd:	/*DCR C*/
		DCR(C);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xe:	/*MVI C, D8*/
		cpu->C = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xf:	/*RRC*/
		cpu->F = (cpu->A & 1) | (cpu->F &~1);
		cpu->A = (cpu->A >> 1) | ((cpu->F & 1) << 7);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x10:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x11:	/*LXI D, D16*/
		cpu->DE = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x12:	/*STAX D*/
		writeRAM(cpu->RAM, cpu->DE, cpu->A);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x13:	/*INX D*/
		cpu->DE++;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x14:	/*INR D*/
		INR(D);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x15:	/*DCR D*/
		DCR(D);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x16:	/*MVI D, D8*/
		cpu->D = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x17:	/*RAL*/
		temp = cpu->F;
		cpu->F = (cpu->A >> 7) | (cpu->F &~1);
		cpu->A = (cpu->A << 1) | (temp & 1);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x18:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x19:	/*DAD D*/
		DAD(DE);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x1a:	/*LDAX D*/
		cpu->A = readRAM(cpu->RAM, cpu->DE);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x1b:	/*DCX D*/
		cpu->DE--;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x1c:	/*INR E*/
		INR(E);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x1d:	/*DCR E*/
		DCR(E);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x1e:	/*MVI E, D8*/
		cpu->E = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x1f:	/*RAR*/
		temp = cpu->F;
		cpu->F = (cpu->A & 1) | (cpu->F &~1);
		cpu->A = (cpu->A >> 1) | ((temp & 1) << 7);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x20:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x21:	/*LXI H, D16*/
		cpu->HL = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x22:	/*SHLD A16*/
		temp = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		writeWRAM(cpu->RAM, temp, cpu->HL);
		clks = 16;
		cpu->tstates += clks;
		break;
		case 0x23:	/*INX H*/
		cpu->HL++;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x24:	/*INR H*/
		INR(H);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x25:	/*DCR H*/
		DCR(H);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x26:	/*MVI H, D8*/
		cpu->H = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x27:	/*DAA*/
		if((cpu->F & AcryFlag) 
		  || (cpu->A & 0x0f) > 0x09) {
			cpu->F |= (cpu->A + 0x06) >> 8; // carry can be set but not reset
		    	cpu->F = (cpu->F &~AcryFlag) |
			((((cpu->A & 0x0f) + 0x06) & AcryFlag) ? AcryFlag : 0);
		    	cpu->A += 0x06;
		}
		if((cpu->F & CaryFlag)
		 || (cpu->A & 0xf0) > 0x90) {
		    	cpu->F |= (cpu->A + 0x60) >> 8; // carry can be set but not reset
		    	cpu->A += 0x60;
		}
		cpu->F &= 0b00010011;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x28:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x29:	/*DAD H*/
		DAD(HL);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x2a:	/*LHLD A16*/
		temp = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		cpu->HL = readWRAM(cpu->RAM, temp);
		clks = 16;
		cpu->tstates += clks;
		break;
		case 0x2b:	/*DCX H*/
		cpu->HL--;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x2c:	/*INR L*/
		INR(L);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x2d:	/*DCR L*/
		DCR(L);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x2e:	/*MVI L, D8*/
		cpu->L = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x2f:	/*CMA*/
		cpu->A =~cpu->A;
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x30:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x31:	/*LXI SP, D16*/
		cpu->SP = readWRAM(cpu->RAM, cpu->PC);
		cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x32:	/*STA A16*/
		temp = readWRAM(cpu->RAM, cpu->PC);
		writeRAM(cpu->RAM, temp, cpu->A);
		cpu->PC += 2;
		clks = 13;
		cpu->tstates += clks;
		break;
		case 0x33:	/*INX SP*/
		cpu->SP++;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x34:	/*INR M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		temp++;
		temp &= 0xFF;
		cpu->F &= 0b00000011;
		cpu->F |= INR_FLAGS[temp];
		writeRAM(cpu->RAM, cpu->HL, temp);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x35:	/*DCR M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		temp--;
		temp &= 0xFF;
		cpu->F &= 0b00000011;
		cpu->F |= DCR_FLAGS[temp];
		writeRAM(cpu->RAM, cpu->HL, temp);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x36:	/*MVI M, D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		writeRAM(cpu->RAM, cpu->HL, temp);
		cpu->PC++;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x37:	/*STC*/
		cpu->F |= CaryFlag;
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x38:	/*-NOP*/
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x39:	/*DAD SP*/
		DAD(SP);
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0x3a:	/*LDA A16*/
		temp = readWRAM(cpu->RAM, cpu->PC);
		cpu->A = readRAM(cpu->RAM, temp);
		cpu->PC += 2;
		clks = 13;
		cpu->tstates += clks;
		break;
		case 0x3b:	/*DCX SP*/
		cpu->SP--;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x3c:	/*INR A*/
		INR(A);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x3d:	/*DCR A*/
		DCR(A);
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x3e:	/*MVI A, D8*/
		cpu->A = readRAM(cpu->RAM, cpu->PC);
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x3f:	/*CMC*/
		cpu->F ^= CaryFlag;
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x40:	/*MOV B, B*/
		cpu->B = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x41:	/*MOV B, C*/
		cpu->B = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x42:	/*MOV B, D*/
		cpu->B = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x43:	/*MOV B, E*/
		cpu->B = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x44:	/*MOV B, H*/
		cpu->B = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x45:	/*MOV B, L*/
		cpu->B = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x46:	/*MOV B, M*/
		cpu->B = readRAM(cpu->RAM, cpu->HL);;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x47:	/*MOV B, A*/
		cpu->B = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x48:	/*MOV C, B*/
		cpu->C = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x49:	/*MOV C, C*/
		cpu->C = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4a:	/*MOV C, D*/
		cpu->C = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4b:	/*MOV C, E*/
		cpu->C = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4c:	/*MOV C, H*/
		cpu->C = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4d:	/*MOV C, L*/
		cpu->C = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x4e:	/*MOV C, M*/
		cpu->C = readRAM(cpu->RAM, cpu->HL);;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x4f:	/*MOV C, A*/
		cpu->C = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x50:	/*MOV D, B*/
		cpu->D = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x51:	/*MOV D, C*/
		cpu->D = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x52:	/*MOV D, D*/
		cpu->D = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x53:	/*MOV D, E*/
		cpu->D = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x54:	/*MOV D, H*/
		cpu->D = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x55:	/*MOV D, L*/
		cpu->D = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x56:	/*MOV D, M*/
		cpu->D = readRAM(cpu->RAM, cpu->HL);;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x57:	/*MOV D, A*/
		cpu->D = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x58:	/*MOV E, B*/
		cpu->E = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x59:	/*MOV E, C*/
		cpu->E = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5a:	/*MOV E, D*/
		cpu->E = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5b:	/*MOV E, E*/
		cpu->E = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5c:	/*MOV E, H*/
		cpu->E = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5d:	/*MOV E, L*/
		cpu->E = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x5e:	/*MOV E, M*/
		cpu->E = readRAM(cpu->RAM, cpu->HL);;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x5f:	/*MOV E, A*/
		cpu->E = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x60:	/*MOV H, B*/
		cpu->H = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x61:	/*MOV H, C*/
		cpu->H = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x62:	/*MOV H, D*/
		cpu->H = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x63:	/*MOV H, E*/
		cpu->H = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x64:	/*MOV H, H*/
		cpu->H = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x65:	/*MOV H, L*/
		cpu->H = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x66:	/*MOV H, M*/
		cpu->H = readRAM(cpu->RAM, cpu->HL);;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x67:	/*MOV H, A*/
		cpu->H = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x68:	/*MOV L, B*/
		cpu->L = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x69:	/*MOV L, C*/
		cpu->L = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6a:	/*MOV L, D*/
		cpu->L = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6b:	/*MOV L, E*/
		cpu->L = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6c:	/*MOV L, H*/
		cpu->L = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6d:	/*MOV L, L*/
		cpu->L = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6e:	/*MOV L, M*/
		cpu->L = readRAM(cpu->RAM, cpu->HL);;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x6f:	/*MOV L, A*/
		cpu->L = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x70:	/*MOV M, B*/
		writeRAM(cpu->RAM, cpu->HL, cpu->B);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x71:	/*MOV M, C*/
		writeRAM(cpu->RAM, cpu->HL, cpu->C);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x72:	/*MOV M, D*/
		writeRAM(cpu->RAM, cpu->HL, cpu->D);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x73:	/*MOV M, E*/
		writeRAM(cpu->RAM, cpu->HL, cpu->E);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x74:	/*MOV M, H*/
		writeRAM(cpu->RAM, cpu->HL, cpu->H);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x75:	/*MOV M, L*/
		writeRAM(cpu->RAM, cpu->HL, cpu->L);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x76:	/*HLT*/
		cpu->halt = 1;
		clks = 7;
		cpu->PC--;
		cpu->tstates += clks;
		break;
		case 0x77:	/*MOV M, A*/
		writeRAM(cpu->RAM, cpu->HL, cpu->A);
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0x78:	/*MOV A, B*/
		cpu->A = cpu->B;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x79:	/*MOV A, C*/
		cpu->A = cpu->C;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7a:	/*MOV A, D*/
		cpu->A = cpu->D;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7b:	/*MOV A, E*/
		cpu->A = cpu->E;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7c:	/*MOV A, H*/
		cpu->A = cpu->H;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7d:	/*MOV A, L*/
		cpu->A = cpu->L;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7e:	/*MOV A, M*/
		cpu->A = readRAM(cpu->RAM, cpu->HL);;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x7f:	/*MOV A, A*/
		cpu->A = cpu->A;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0x80:	/*ADD B*/
		ADD(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x81:	/*ADD C*/
		ADD(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x82:	/*ADD D*/
		ADD(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x83:	/*ADD E*/
		ADD(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x84:	/*ADD H*/
		ADD(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x85:	/*ADD L*/
		ADD(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x86:	/*ADD M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		cpu->F &= 0b00000010;
		cpu->F |= (temp + cpu->A) >> 8;
		cpu->F |= ((temp & 0xF) + (cpu->A & 0xF) & 0x10);
		cpu->A += temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x87:	/*ADD A*/
		ADD(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x88:	/*ADC B*/
		ADC(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x89:	/*ADC C*/
		ADC(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8a:	/*ADC D*/
		ADC(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8b:	/*ADC E*/
		ADC(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8c:	/*ADC H*/
		ADC(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8d:	/*ADC L*/
		ADC(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8e:	/*ADC M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;
		cpu->F &= 0b00000010;
		cpu->F |= (temp + cpu->A) >> 8;
		cpu->F |= ((temp & 0xF) + (cpu->A & 0xF) & 0x10);
		cpu->A += temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x8f:	/*ADC A*/
		ADC(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x90:	/*SUB B*/
		SUB(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x91:	/*SUB C*/
		SUB(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x92:	/*SUB D*/
		SUB(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x93:	/*SUB E*/
		SUB(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x94:	/*SUB H*/
		SUB(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x95:	/*SUB L*/
		SUB(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x96:	/*SUB M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->A -= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x97:	/*SUB A*/
		SUB(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x98:	/*SBB B*/
		SBB(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x99:	/*SBB C*/
		SBB(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9a:	/*SBB D*/
		SBB(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9b:	/*SBB E*/
		SBB(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9c:	/*SBB H*/
		SBB(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9d:	/*SBB L*/
		SBB(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9e:	/*SBB M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->A -= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0x9f:	/*SBB A*/
		SBB(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa0:	/*ANA B*/
		ANA(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa1:	/*ANA C*/
		ANA(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa2:	/*ANA D*/
		ANA(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa3:	/*ANA E*/
		ANA(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa4:	/*ANA H*/
		ANA(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa5:	/*ANA L*/
		ANA(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa6:	/*ANA M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		cpu->F &= 0b00000010;
		cpu->F |= ((cpu->A | temp) & 0x8) << 1; //Aux carry is equal to OR of bit 3 of operand and accumulator
		cpu->A &= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xa7:	/*ANA A*/
		ANA(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa8:	/*XRA B*/
		XRA(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xa9:	/*XRA C*/
		XRA(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xaa:	/*XRA D*/
		XRA(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xab:	/*XRA E*/
		XRA(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xac:	/*XRA H*/
		XRA(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xad:	/*XRA L*/
		XRA(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xae:	/*XRA M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		cpu->F &= 0b00000010;
		cpu->A ^= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xaf:	/*XRA A*/
		XRA(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb0:	/*ORA B*/
		ORA(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb1:	/*ORA C*/
		ORA(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb2:	/*ORA D*/
		ORA(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb3:	/*ORA E*/
		ORA(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb4:	/*ORA H*/
		ORA(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb5:	/*ORA L*/
		ORA(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb6:	/*ORA M*/
		temp = readRAM(cpu->RAM, cpu->HL);
		cpu->F &= 0b00000010;
		cpu->A |= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xb7:	/*ORA A*/
		ORA(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb8:	/*CMP B*/
		CMP(B);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xb9:	/*CMP C*/
		CMP(C);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xba:	/*CMP D*/
		CMP(D);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xbb:	/*CMP E*/
		CMP(E);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xbc:	/*CMP H*/
		CMP(H);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xbd:	/*CMP L*/
		CMP(L);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xbe:	/*CMP M*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->F |= SZP_FLAGS[(uint8_t)(temp - cpu->A)];
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xbf:	/*CMP A*/
		CMP(A);
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xc0:	/*RNZ*/
		if(cpu->F & ZeroFlag) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xc1:	/*POP B*/
		cpu->BC = POP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xc2:	/*JNZ A16*/
		if(!(cpu->F & ZeroFlag)) {
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xc3:	/*JMP A16*/
		JMP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xc4:	/*CNZ A16*/
		cpu->PC += 2;
		if(cpu->F & ZeroFlag) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xc5:	/*PUSH B*/
		PUSH(cpu->BC);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xc6:	/*ADI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->F |= (temp + cpu->A) >> 8;
		cpu->F |= ((temp & 0xF) + (cpu->A & 0xF) & 0x10);
		cpu->A += temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xc7:	/*RST 0*/
		RST(0);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xc8:	/*RZ*/
		if(!(cpu->F & ZeroFlag)) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xc9:	/*RET*/
		cpu->PC = POP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xca:	/*JZ A16*/
		if(cpu->F & ZeroFlag){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xcb:	/*-JMP A16*/
		JMP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xcc:	/*CZ A16*/
		cpu->PC += 2;
		if(!(cpu->F & ZeroFlag)) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xcd:	/*CALL A16*/
		PUSH(cpu->PC + 2);
		JMP;
		clks = 17;
		cpu->tstates += clks;
		break;
		case 0xce:	/*ACI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;
		cpu->F &= 0b00000010;
		cpu->F |= (temp + cpu->A) >> 8;
		cpu->F |= ((temp & 0xF) + (cpu->A & 0xF) & 0x10);
		cpu->A += temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xcf:	/*RST 1*/
		RST(1);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xd0:	/*RNC*/
		if(cpu->F & CaryFlag) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xd1:	/*POP D*/
		cpu->DE = POP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xd2:	/*JNC A16*/
		if(!(cpu->F & CaryFlag)){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xd3:	/*OUT D8*/
		output(readRAM(cpu->RAM, cpu->PC), cpu->A);
		cpu->PC++;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xd4:	/*CNC A16*/
		cpu->PC += 2;
		if(cpu->F & CaryFlag) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xd5:	/*PUSH D*/
		PUSH(cpu->DE);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xd6:	/*SUI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->A -= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xd7:	/*RST 2*/
		RST(2);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xd8:	/*RC*/
		if(!(cpu->F & CaryFlag)) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xd9:	/*-RET*/
		cpu->PC = POP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xda:	/*JC*/
		if(cpu->F & CaryFlag){
			JMP;
		}
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xdb:	/*IN D8*/
		cpu->A = input(readRAM(cpu->RAM, cpu->PC));
		cpu->PC++;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xdc:	/*CC*/
		if(!(cpu->F & CaryFlag)) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xdd:	/*-CALL*/
		PUSH(cpu->PC + 2);
		JMP;
		clks = 17;
		cpu->tstates += clks;
		break;
		case 0xde:	/*SBI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		temp = (cpu->F & CaryFlag) ? temp + 1 : temp;
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->A -= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xdf:	/*RST 3*/
		RST(3);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xe0:	/*RPO*/
		if(cpu->F & PariFlag) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xe1:	/*POP H*/
		cpu->HL = POP;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xe2:	/*JPO A16*/
		if(!(cpu->F & PariFlag)){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xe3:	/*XTHL*/
		temp = readWRAM(cpu->RAM, cpu->SP);
		writeWRAM(cpu->RAM, cpu->SP, cpu->HL);
		cpu->HL = temp;
		clks = 18;
		cpu->tstates += clks;
		break;
		case 0xe4:	/*CPO A16*/
		cpu->PC += 2;
		if(cpu->F & PariFlag) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xe5:	/*PUSH H*/
		PUSH(cpu->HL);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xe6:	/*ANI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->F |= ((cpu->A | temp) & 0x8) << 1; //Aux carry is equal to OR of bit 3 of operand and accumulator
		cpu->A &= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xe7:	/*RST 4*/
		RST(4);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xe8:	/*RPE*/
		if(!(cpu->F & PariFlag)) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xe9:	/*PCHL*/
		cpu->PC = cpu->HL;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xea:	/*JPE A16*/
		if(cpu->F & PariFlag){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xeb:	/*XCHG*/
		temp = cpu->HL;
		cpu->HL = cpu->DE;
		cpu->DE = temp;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xec:	/*CPE A16*/
		cpu->PC += 2;
		if(!(cpu->F & PariFlag)) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xed:	/*-CALL*/
		PUSH(cpu->PC + 2);
		JMP;
		clks = 17;
		cpu->tstates += clks;
		break;
		case 0xee:	/*XRI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->A ^= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xef:	/*RST 5*/
		RST(5);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xf0:	/*RP*/
		if(cpu->F & SignFlag) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xf1:	/*POP PSW*/
		cpu->AF = POP;
		clks = 10;
		break;
		case 0xf2:	/*JP A16*/
		if(!(cpu->F & SignFlag)){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xf3:	/*DI*/
		cpu->inte = 0;
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xf4:	/*CP A16*/
		cpu->PC += 2;
		if(cpu->F & SignFlag) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xf5:	/*PUSH PSW*/
		PUSH(cpu->AF);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xf6:	/*ORI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->A |= temp;
		cpu->F |= SZP_FLAGS[cpu->A];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xf7:	/*RST 6*/
		RST(6);
		clks = 11;
		cpu->tstates += clks;
		break;
		case 0xf8:	/*RM*/
		if(!(cpu->F & SignFlag)) clks = 5;
		else{
			cpu->PC = POP;
			clks = 11;
		}
		cpu->tstates += clks;
		break;
		case 0xf9:	/*SPHL*/
		cpu->SP = cpu->HL;
		clks = 5;
		cpu->tstates += clks;
		break;
		case 0xfa:	/*JM A16*/
		if(cpu->F & SignFlag){
			JMP;
		}else cpu->PC += 2;
		clks = 10;
		cpu->tstates += clks;
		break;
		case 0xfb:	/*EI*/
		cpu->inte = 1;
		clks = 4;
		cpu->tstates += clks;
		break;
		case 0xfc:	/*CM A16*/
		cpu->PC += 2;
		if(!(cpu->F & SignFlag)) clks = 11;
		else{
			PUSH(cpu->PC);
			JMP;
			clks = 17;
		}
		cpu->tstates += clks;
		break;
		case 0xfd:	/*-CALL*/
		PUSH(cpu->PC + 2);
		JMP;
		clks = 17;
		cpu->tstates += clks;
		break;
		case 0xfe:	/*CPI D8*/
		temp = readRAM(cpu->RAM, cpu->PC);
		cpu->F &= 0b00000010;
		cpu->F |= (cpu->A - temp) >> 8 & 0x1;
		cpu->F |= ((cpu->A & 0xF) - (temp & 0xF) & 0x10);
		cpu->F |= SZP_FLAGS[(uint8_t)(cpu->A - temp)];
		cpu->PC++;
		clks = 7;
		cpu->tstates += clks;
		break;
		case 0xff:	/*RST 7*/
		RST(7);
		clks = 11;
		cpu->tstates += clks;
		break;
	}
	cpu->F |= 2;
	cpu->F &= 0b11010111;
	return clks;
}
#undef PUSH
#undef POP
#undef JMP
#undef RST
#undef INR
#undef DCR
#undef DAD
#undef ADD
#undef ADC
#undef SUB
#undef SBB
#undef ANA
#undef XRA
#undef ORA
#undef CMP

void runCPU(i8080 *cpu, int tstates)
{
	while(tstates > 0) tstates -= singleStep(cpu);
}
