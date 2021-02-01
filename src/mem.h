#ifndef _mem_
#define _mem_
#include<stdint.h>
#include<stdlib.h>
typedef struct dedicatedRAM{
	uint8_t addrSize; 	/*Size of address bus in bits*/
	uint8_t *RAM;		/*Self explanitory*/
	uint8_t PageSize; 	/*Size of page address bus in bits*/
	uint8_t *prot; 		/*Memory protected flag buffer*/
}dedicatedRAM;

dedicatedRAM newDRAM(int MemSize, int PageSize);
void setProt(dedicatedRAM *RAM, uint16_t addr);
void clearProt(dedicatedRAM *RAM, uint16_t addr);
uint8_t readProt(dedicatedRAM *RAM, uint16_t addr);
uint8_t readRAM(dedicatedRAM *RAM, uint16_t addr);
void writeRAM(dedicatedRAM *RAM, uint16_t addr, uint8_t data);
#endif /*_mem_*/
