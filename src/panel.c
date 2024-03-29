#include"panel.h"

bool checkUpChange(FrontPanel *c, FrontPanel *o, int mask)
{
	if((c->contswitchesup & mask) &&
	  !(o->contswitchesup & mask))
		return true;
	else
		return false;
}

bool checkDownChange(FrontPanel *c, FrontPanel *o, int mask)
{
	if((c->contswitchesdown & mask) &&
	  !(o->contswitchesdown & mask))
		return true;
	else
		return false;
}

void panelLogic(FrontPanel *panel, i8080 *cpu)
{
	static FrontPanel oldpanel = {0,0,0,0,0,0,false,false};
	if(panel->power){
		/*Power switch stuff*/
		if(!oldpanel.power){
			panel->addr = rand();
			int size = 1 << (cpu->RAM->addrSize - 2);
			for(int i = 0; i < size; i++)
				((int*)cpu->RAM->RAM)[i] = rand();
		}
		/*Control switch stuff*/
		if(checkUpChange(panel, &oldpanel, Protect))
			setProt(cpu->RAM, panel->addr);
		if(checkDownChange(panel, &oldpanel, Protect))
			clearProt(cpu->RAM, panel->addr);
		if(checkUpChange(panel, &oldpanel, Examine)){
			panel->addr = panel->dataswitches;
		}
		if(checkDownChange(panel, &oldpanel, Examine))
			panel->addr++;
		if(checkUpChange(panel, &oldpanel, Deposit))
			writeRAM(cpu->RAM, panel->addr, panel->dataswitches & 0xFF);
		if(checkDownChange(panel, &oldpanel, Deposit)){
			panel->addr++;
			writeRAM(cpu->RAM, panel->addr, panel->dataswitches & 0xFF);
		}
		if(checkUpChange(panel, &oldpanel, Step)){
			singleStep(cpu);
			panel->addr = cpu->PC;
			if(cpu->halt == 1) panel->addr = ~0;
		}
		//if(checkDownChange(panel, &oldpanel, Step));
		if(checkUpChange(panel, &oldpanel, ResetClr)){
			cpu->halt = 0;
			cpu->PC = 0;
			panel->addr = 0;
		}

		if(checkDownChange(panel, &oldpanel, ResetClr)){
			cpu->halt = 0;
			cpu->PC = 0;
			panel->addr = 0;
			stopTimer();
			extern dedicatedRAM mainMemory;
			loadRAM(&mainMemory);
		}
		
		if(panel->contswitchesdown & StopRun)
			panel->running = true;

		if(panel->contswitchesup & StopRun){
			panel->running = false;
			panel->addr = cpu->PC;
		}

		panel->data = readRAM(cpu->RAM, panel->addr);

		/*Running stuff*/
		if(panel->running){
			runCPU(cpu, 2000000 / 60);
			if(cpu->halt == 1) {
				panel->addr = ~0;
				panel->data = 0x76;
			}else{
				panel->addr = cpu->PC;
				panel->data = readRAM(cpu->RAM, panel->addr);
			}
		}


		/*Status light stuff*/
		panel->stat |= MI;

		if(readProt(cpu->RAM, panel->addr))
			panel->stat |= PROT;
		else
			panel->stat &=~PROT;

		if(panel->running)
			panel->stat &=~WAIT;
		else
			panel->stat |= WAIT;

		if(cpu->halt == 1)
			panel->stat |= HLTA;
		else
			panel->stat &=~HLTA;
	}else{
		cpu->halt = 0;
		panel->addr = 0;
		panel->data = 0;
		panel->stat = 0;
		panel->running = false;
		stopTimer();
	}
	oldpanel = *panel;
}
