#ifndef _PANEL_
#define _PANEL_
#include<raylib.h>
#include"8080.h"

typedef struct FrontPanel{
	uint16_t addr;
	uint8_t data;
	uint16_t stat;
	uint8_t contswitchesup;
	uint8_t contswitchesdown;
	uint16_t dataswitches;
	bool running;
	bool power;
}FrontPanel;

/*Switch Masks*/
#define StopRun 0x1
#define Step 	0x2
#define Examine 0x4
#define Deposit 0x8
#define ResetClr 0x10
#define Protect 0x20
#define AuxOne 	0x40
#define AuxTwo 	0x80
/*Status Light Masks*/
#define INTE 	0x1
#define PROT 	0x2
#define MEMR 	0x4
#define INP 	0x8
#define MI 	0x10
#define OUT 	0x20
#define HLTA 	0x40
#define STACK 	0x80
#define WO 	0x100
#define INT 	0x200
#define WAIT 	0x400
#define HLDA 	0x800

void panelLogic(FrontPanel *panel, i8080 *cpu);
#endif /*_PANEL*/
