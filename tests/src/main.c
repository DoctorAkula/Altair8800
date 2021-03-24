#include<curses.h>

#include"8080.h"
#include"mem.h"

#define MEMSIZE 8

void disp8080(i8080 *cpu, int pos)
{
		mvprintw(0, pos, "A: 0x%X F: 0x%X    ",
	                      cpu->A,  cpu->F);
		mvprintw(1, pos, "B: 0x%X C: 0x%X    ",
	                      cpu->B,  cpu->C);
		mvprintw(2, pos, "C: 0x%X E: 0x%X    ",
	                      cpu->D,  cpu->E);
		mvprintw(3, pos, "H: 0x%X L: 0x%X    ",
	                      cpu->H,  cpu->L);
		mvprintw(4, pos, "PC: 0x%X    ",
	                      cpu->PC);
		mvprintw(5, pos, "SP: 0x%X    ",
	                      cpu->SP);
		mvprintw(6, pos, "T-States: %d            ",
	                      cpu->tstates);
}

void dispDRAM(i8080 *cpu, unsigned addr)
{
	for(int i = 0; i < (1 << MEMSIZE); i++){
		if(i == (cpu->PC & 0xFF)) attron(A_REVERSE);
		else attroff(A_REVERSE);
		if(i == addr) attron(A_UNDERLINE);
		else attroff(A_UNDERLINE);
		mvprintw(i >> 4, (i & 0xF) * 3,
			 "%0*X", 2, readRAM(&cpu->RAM, i));
	}
	attroff(A_UNDERLINE);
	attroff(A_REVERSE);
}

void poke(i8080 *cpu, char in, unsigned addr)
{
	unsigned val;
	if(('0' <= in) && (in <= '9')) val = in - '0';
	else val = in - 'a' + 10;
	unsigned data = (readRAM(&cpu->RAM, addr) << 4) & 0xFF;
	writeRAM(&cpu->RAM, addr, data | val);
}

int main(int argc, char *argv[])
{
	WINDOW *win = initscr();
	cbreak();
	noecho();
	i8080 cpu= {A: 0,F: 2,BC: 0,DE: 0,HL: 0,SP: 0,PC: 0,
		    RAM: newDRAM(MEMSIZE, MEMSIZE), tstates: 0, halt: 0};
	char in;
	unsigned addr;
	do{
		switch(in){
			case 'h':
				addr -= 1;
				addr &= (1 << MEMSIZE) - 1;
				break;
			case 'j':
				addr += 16;
				addr &= (1 << MEMSIZE) - 1;
				break;
			case 'k':
				addr -= 16;
				addr &= (1 << MEMSIZE) - 1;
				break;
			case 'l':
				addr += 1;
				addr &= (1 << MEMSIZE) - 1;
				break;
			case 'g':
				cpu.PC = addr;
				break;
			case 's':
				singleStep(&cpu);
				break;
			case 'r':
				cpu.AF = 2;
				cpu.BC = 0;
				cpu.DE = 0;
				cpu.HL = 0;
				cpu.SP = 0;
				cpu.PC = 0;
				cpu.tstates = 0;
				break;
			case '0': /*Fallthrough*/
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
				poke(&cpu, in, addr);
				break;
			default:
				break;
		}
		disp8080(&cpu, 50);
		dispDRAM(&cpu, addr);
	}while((in = getch()) != 'q');
	endwin();
}
