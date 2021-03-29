#include"devices.h"
#include<stdio.h>

void pipeOut(uint8_t data)
{
	FILE *pipe = fopen("/tmp/pipe", "r+");
	if(!pipe) return;
	fputc(data, pipe);
	fclose(pipe);
}

uint8_t pipeIn(void)
{
	FILE *pipe = fopen("/tmp/pipe", "r");
	if(!pipe) return 0;
	uint8_t val = fgetc(pipe);
	fclose(pipe);
	return val;
}
