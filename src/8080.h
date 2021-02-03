#ifndef _i8080_
#define _i8080_
#include"mem.h"

/*Flag masks*/
#define SignFlag 0x80 /*Negative while 1*/
#define ZeroFlag 0x40
#define AcryFlag 0x10 /*Carry from bit 3 to bit 4*/
#define PariFlag 0x04 /*Even while 1*/
#define CaryFlag 0x01


typedef struct i8080{
	union{
		uint16_t AF;
	struct{
		uint8_t F;
		uint8_t A;
	};};
	union{
		uint16_t BC;
	struct{
		uint8_t C;
		uint8_t B;
	};};
	union{
		uint16_t DE;
	struct{
		uint8_t E;
		uint8_t D;
	};};
	union{
		uint16_t HL;
	struct{
		uint8_t L;
		uint8_t H;
	};};
	uint16_t SP;
	uint16_t PC;
	dedicatedRAM RAM;
	uint64_t tstates;
	uint8_t halt;
}i8080;

int singleStep(i8080 *cpu);
void runCPU(i8080 *cpu, int freq);
#endif /*_i8080_*/
