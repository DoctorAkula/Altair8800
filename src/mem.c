#include"mem.h"

dedicatedRAM newDRAM(int addrSize, int PageSize)
{
	int size = 1 << addrSize;
	uint8_t *RAM = malloc(size);
	size >>= PageSize;
	uint8_t *prot = calloc(size, sizeof(uint8_t));
	return (dedicatedRAM) {
		addrSize,
		RAM,
		PageSize,
		prot
	};
}

void setProt(dedicatedRAM *RAM, uint16_t addr)
{
	int mask = (1 << RAM->addrSize) - 1;
	addr &= mask;
	addr >>= RAM->PageSize;
	RAM->prot[addr] = 1;
}

void clearProt(dedicatedRAM *RAM, uint16_t addr)
{
	int mask = (1 << RAM->addrSize) - 1;
	addr &= mask;
	addr >>= RAM->PageSize;
	RAM->prot[addr] = 0;
}

uint8_t readProt(dedicatedRAM *RAM, uint16_t addr)
{
	int mask = (1 << RAM->addrSize) - 1;
	addr &= mask;
	addr >>= RAM->PageSize;
	return RAM->prot[addr];
}

uint8_t readRAM(dedicatedRAM *RAM, uint16_t addr)
{
	int mask = (1 << RAM->addrSize) - 1;
	addr &= mask;
	return RAM->RAM[addr];
}

void writeRAM(dedicatedRAM *RAM, uint16_t addr, uint8_t data)
{
	int mask = (1 << RAM->addrSize) - 1;
	addr &= mask;
	uint16_t protAddr = addr >> RAM->PageSize;
	if(!RAM->prot[protAddr])
		RAM->RAM[addr] = data;
}
