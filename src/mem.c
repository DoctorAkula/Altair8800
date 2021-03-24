#include"mem.h"

dedicatedRAM newDRAM(int addrSize, int PageSize)
{
	int size = 1 << addrSize;
	uint8_t *RAM = malloc(size);
	size >>= PageSize;
	uint8_t *prot = calloc(size, sizeof(uint8_t));
	uint16_t addrMask = (1 << addrSize) - 1;
	return (dedicatedRAM) {
		addrSize,
		addrMask,
		RAM,
		PageSize,
		prot
	};
}

void freeDRAM(dedicatedRAM *RAM)
{
	free(RAM->prot);
	free(RAM->RAM);
}

void setProt(dedicatedRAM *RAM, uint16_t addr)
{
	addr &= RAM->addrMask;
	addr >>= RAM->PageSize;
	RAM->prot[addr] = 1;
}

void clearProt(dedicatedRAM *RAM, uint16_t addr)
{
	addr &= RAM->addrMask;
	addr >>= RAM->PageSize;
	RAM->prot[addr] = 0;
}

uint8_t readProt(dedicatedRAM *RAM, uint16_t addr)
{
	addr &= RAM->addrMask;
	addr >>= RAM->PageSize;
	return RAM->prot[addr];
}

uint8_t readRAM(dedicatedRAM *RAM, uint16_t addr)
{
	addr &= RAM->addrMask;
	return RAM->RAM[addr];
}

void writeRAM(dedicatedRAM *RAM, uint16_t addr, uint8_t data)
{
	addr &= RAM->addrMask;
	uint16_t protAddr = addr >> RAM->PageSize;
	if(!RAM->prot[protAddr])
		RAM->RAM[addr] = data;
}

uint16_t readWRAM(dedicatedRAM *RAM, uint16_t addr)
{
	return (uint16_t)readRAM(RAM, addr++) |
		((uint16_t)readRAM(RAM, addr) << 8);
}

void writeWRAM(dedicatedRAM *RAM, uint16_t addr, uint16_t data)
{
	writeRAM(RAM, addr++, data & 0xFF);
	writeRAM(RAM, addr, data >> 8);
}
