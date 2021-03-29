#ifndef _IO_
#define _IO_
#include"devices.h"

/*IO access functions*/
uint8_t input(uint8_t port);
void output(uint8_t port, uint8_t data);

/*Device prototypes*/
void null_out(uint8_t data);
uint8_t null_in(void);

#endif /*_IO_*/
