#include<curses.h>

#include"8080.h"
#include"mem.h"

#define MEMSIZE 8

void disp8080(i8080 *cpu, int pos)
{
	mvprintw(0, pos, "SZ0A0P1C");
	for(int i = 0; i < 8; i++){
		mvprintw(1, (pos+7) - i, "%d",
			(cpu->F >> i) & 1);
	}
	mvprintw(2, pos, "A: 0x%0*X F: 0x%0*X",
		  2, cpu->A,2,cpu->F);
	mvprintw(3, pos, "B: 0x%0*X C: 0x%0*X",
		  2, cpu->B,2,cpu->C);0*
	mvprintw(4, pos, "D: 0x%0*X E: 0x%0*X",
		  2, cpu->D,2,cpu->E);
	mvprintw(5, pos, "H: 0x%0*X L: 0x%0*X",
		  2, cpu->H,2,cpu->L);
	mvprintw(6, pos, "PC: 0x%0*X",
		  4, cpu->PC);
	mvprintw(7, pos, "SP: 0x%0*X",
		  4, cpu->SP);
	mvprintw(8, pos, "T-States: %*d",
		  10, cpu->tstates);
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
