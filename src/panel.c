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
	static FrontPanel oldpanel = {0,0,0,0,0,0,false};
	if(panel->power){
		if(!oldpanel.power){
			panel->addr = rand();
			int size = 1 << (cpu->RAM.addrSize - 2);
			for(int i = 0; i < size; i++)
				((int*)cpu->RAM.RAM)[i] = rand();
		}
		/*Control switch stuff*/
		if(checkUpChange(panel, &oldpanel, Protect))
			setProt(&cpu->RAM, panel->addr);
		if(checkDownChange(panel, &oldpanel, Protect))
			clearProt(&cpu->RAM, panel->addr);
		if(checkUpChange(panel, &oldpanel, Examine))
			panel->addr = panel->dataswitches;
		if(checkDownChange(panel, &oldpanel, Examine)){
			panel->addr++;
			panel->data = readRAM(&cpu->RAM, panel->addr);
		}
		if(checkUpChange(panel, &oldpanel, Deposit)){
			writeRAM(&cpu->RAM, panel->addr, panel->dataswitches & 0xFF);
		}
		if(checkDownChange(panel, &oldpanel, Deposit)){
			panel->addr++;
			writeRAM(&cpu->RAM, panel->addr, panel->dataswitches & 0xFF);
		}
		panel->data = readRAM(&cpu->RAM, panel->addr);

		/*Status light stuff*/
		if(readProt(&cpu->RAM, panel->addr))
			panel->stat |= PROT;
		else
			panel->stat &=~PROT;
	}else{
		panel->addr = 0;
		panel->data = 0;
		panel->stat = 0;
	}
	oldpanel = *panel;
}
