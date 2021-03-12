#include"8080.h"

const uint8_t INR_FLAGS[256] = {
	0x54,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x10,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x10,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x14,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x10,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x14,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x14,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x10,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x90,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x94,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x94,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x90,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x94,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x90,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x90,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x94,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
};

const uint8_t DCR_FLAGS[256] = {
	0x44,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x14,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x10,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x10,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x14,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x10,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x14,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x14,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x10,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x90,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x94,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x94,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x90,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x94,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x90,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x90,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x94,
};

const uint8_t DAA_FLAGS[256] = {
	0x44,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x4,
	0x0,
	0x0,
	0x4,
	0x4,
	0x0,
	0x0,
	0x4,
	0x0,
	0x4,
	0x4,
	0x0,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
	0x80,
	0x84,
	0x84,
	0x80,
	0x80,
	0x84,
	0x84,
	0x80,
	0x84,
	0x80,
	0x80,
	0x84,
};