#include<errno.h>
#include<string.h>
#include<stdio.h>

#include"devices.h"
#include"mem.h"
extern dedicatedRAM mainMemory;

/*File IO vars*/
static FILE *iofile = NULL;	/*Contains the currently opened file*/
static int fileModeAddr = 0;	/*Contains the address to the file mode string in RAM*/
static int filePathAddr = 0;	/*Contains the address to the file path string in RAM*/
static int errorAddr = 0;	/*Contains error string write address*/
static int fileCtlOp = 0;	/*Contains the current opcode of the file control device*/
static int fileCtlState = 0;	/*Contains the current opcode state of the file control device*/

void fileOut(uint8_t data)
{
	if(!iofile) return;
	fputc(data, iofile);
}

uint8_t fileIn(void)
{
	if(!iofile) return 0;
	return fgetc(iofile);
}

/*File control device*/
void fileCtl(uint8_t data)
{
	if(fileCtlState == 0) fileCtlOp = data;
	switch(fileCtlOp){
		case 0:;	/*Open file*/
			char path[256];
		       	char mode[256];

			/*Read ram into temp buffers*/
			for(int i = 0; i < 256; i++){ 
				path[i] = readRAM(&mainMemory, filePathAddr + i);
				if(path[i] == '\0') break;
			}
			for(int i = 0; i < 256; i++){ 
				mode[i] = readRAM(&mainMemory, fileModeAddr + i);
				if(mode[i] == '\0') break;
			}

			/*Close previous file*/
			if(iofile) fclose(iofile);
			iofile = fopen(path, mode);
			if(!iofile){
				char *err = strerror(errno);
				for(int i = 0; i < 256; i++){ 
					writeRAM(&mainMemory, errorAddr + i, err[i]);
					if(err[i] == '\0') break;
				}
			}
			break;
		case 1:		/*Close file*/
			if(iofile) fclose(iofile);
			iofile = NULL;
			break;
		case 2:		/*Seek set*/
			if(fileCtlState++ == 1){
				if(iofile) fseek(iofile, data, SEEK_SET);
				fileCtlState = 0;
			}
			break;
		case 3:		/*Seek cur*/
			if(fileCtlState++ == 1){
				if(iofile) fseek(iofile, data, SEEK_CUR);
				fileCtlState = 0;
			}
			break;
		case 4:		/*Seek end*/
			if(fileCtlState++ == 1){
				if(iofile) fseek(iofile, data, SEEK_END);
				fileCtlState = 0;
			}
			break;
		case 5:		/*Set path address*/
			if(fileCtlState++ == 1){
				filePathAddr = data;
			}else if(fileCtlState == 3){
				filePathAddr |= (uint16_t)data << 8;
				fileCtlState = 0;
			}
			break;
		case 6:		/*Set mode address*/
			if(fileCtlState++ == 1){
				fileModeAddr = data;
			}else if(fileCtlState == 3){
				fileModeAddr |= (uint16_t)data << 8;
				fileCtlState = 0;
			}
			break;
		case 7:		/*Set error address*/
			if(fileCtlState++ == 1){
				errorAddr = data;
			}else if(fileCtlState == 3){
				errorAddr |= (uint16_t)data << 8;
				fileCtlState = 0;
			}
			break;
		default:	/*No op*/
			break;
	}
}

/*Returns fileCtlState, bit 7 is set if EOF*/
uint8_t fileState(void)
{
	if(!iofile) return fileCtlState;

	int eoftest = fgetc(iofile);
	if(eoftest == EOF)
		return fileCtlState | 0x80;
	else{
		fseek(iofile, -1, SEEK_CUR);
		return fileCtlState;
	}
}
