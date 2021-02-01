#ifndef _MAIN_
#define _MAIN_
#include<raylib.h>
#include"8080.h"
#include"panel.h"
#include"mem.h"

const int width = 1789;
const int height = 634;
const int switchRadius = 12;

const char *imagefiles[] = 
{
	"../resources/frontpanel.png",
	"../resources/lightOFF.png",
	"../resources/lightON.png",
	"../resources/switchNEUTRAL.png",
	"../resources/switchOFF.png",
	"../resources/switchON.png",
};

const char *soundfiles[] = 
{
	"../resources/humCont.wav",
	"../resources/HumStart.wav",
	"../resources/switchOFF.wav",
	"../resources/switchON.wav",
};

enum sounds{humcont, humstart, Switchoff, Switchon, slength};
enum images{frontpanel, lightoff, lighton, switchntl, switchoff, switchon, ilength};
#endif /*_MAIN_*/
