#include"main.h"

void drawSwitchesAndLights(FrontPanel panel, Texture2D *textures)
{
	/*Power Switch*/
	DrawTexture(textures[panel.power ? switchoff : switchon], 90, 426, WHITE);
	/*Control Switches*/

	for(int i = 0; i < 8; i++)
	{
		int meme = (((panel.contswitchesdown >> i) << 1) & 2) | ((panel.contswitchesup >> i) & 1);
		switch(meme)
		{
			case 0:
				DrawTexture(textures[switchntl], 407 + (i * 132), 426, WHITE);
				break;
			case 1:
				DrawTexture(textures[switchon], 407 + (i * 132), 426, WHITE);
				break;
			case 2:
				DrawTexture(textures[switchoff], 407 + (i * 132), 426, WHITE);
				break;
			case 3:
				DrawTexture(textures[switchntl], 407 + (i * 132), 426, WHITE);
				break;
		}
	}
	/*Upper status lights*/
	for(int i = 0; i < 10; i++)
		DrawTexture(textures[(panel.stat & (1 << i)) ? lighton:lightoff], 206 + (i * 66.5), 85, WHITE);
	/*Lower status lights*/
	for(int i = 0; i < 2; i++)
		DrawTexture(textures[(panel.stat & (1 << i+10)) ? lighton:lightoff], 206 + (i * 70), 198, WHITE);
	/*Data/Addr Switches And lights*/
	for(int i = 0; i < 16; i++)
	{
		int o = -23 * (i / 3);
		if(i < 8)
		{
			enum images data = ((panel.data >> i) & 1) ? lighton : lightoff;
			DrawTexture(textures[data], 1563 - (i * 69) + o, 85, WHITE);
		}
		enum images addr = ((panel.addr >> i) & 1) ? lighton : lightoff;
		DrawTexture(textures[addr], 1563 - (i * 69) + o, 198, WHITE);
		enum images selection = ((panel.dataswitches >> i) & 1) ? switchon : switchoff;
		DrawTexture(textures[selection], 1563 - (i * 69) + o, 310, WHITE);
	}
}

void SoundAndEventHandling(FrontPanel *panel, Sound *sounds)
{
	Vector2 curMouse = GetMousePosition();
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		/*Power switch*/
		if(CheckCollisionPointCircle(curMouse, (Vector2){103.0,445.0}, switchRadius))
		{
			panel->power = !panel->power;
			PlaySound(sounds[Switchon]);
		}
		/*Data switches*/
		for(int i = 0; i < 16; i++)
		{
			int o = -23 * (i / 3);
			Vector2 switchCoord = {(float)(1576 - (i * 69) + o), 329.0f};
			if(CheckCollisionPointCircle(curMouse, switchCoord, switchRadius))
			{
				int mask = 1 << i;
				panel->dataswitches ^= mask;
				PlaySound(sounds[Switchon]);
			}
		}

		/*Control switches*/
		for(int i = 0; i < 8; i++)
		{
			Vector2 switchCoord = {(float)(422 + (i * 132)), 444.0f};
			if(CheckCollisionPointCircle(curMouse, switchCoord, switchRadius))
			{
				int mask = 1 << i;
				panel->contswitchesup ^= mask;
				PlaySound(sounds[Switchon]);
			}
		}
	}else if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) &&
		panel->contswitchesup != 0){
		panel->contswitchesup = 0;
		PlaySound(sounds[Switchoff]);
	}
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		/*Control switches*/
		for(int i = 0; i < 8; i++)
		{
			Vector2 switchCoord = {(float)(422 + (i * 132)), 444.0f};
			if(CheckCollisionPointCircle(curMouse, switchCoord, switchRadius))
			{
				int mask = 1 << i;
				panel->contswitchesdown ^= mask;
				PlaySound(sounds[Switchon]);
			}
		}
	}else if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) &&
		panel->contswitchesdown != 0){
		panel->contswitchesdown = 0;
		PlaySound(sounds[Switchoff]);
	}

	if(!IsSoundPlaying(sounds[humcont]) && panel->power)
		PlaySound(sounds[humcont]);
	else if(!panel->power)
		StopSound(sounds[humcont]);
}

int main(int argc, char *argv[])
{
	/*Init start*/
	{
		//Silly but good enough
		void *seed = malloc(0x100);
		srand((long long)seed);
		free(seed);
	}
	InitWindow(width, height, "Altair Front");
	InitAudioDevice();
	if(!IsAudioDeviceReady())
		return -1;
	SetTargetFPS(60);
	Sound sounds[slength];
	Image images[ilength];
	Texture2D textures[ilength];
	FrontPanel panel = {0,0,0,0,0,0,false,false};
	mainMemory = newDRAM(16, 8);
	i8080 cpu = {AF: 0, BC: 0, DE: 0, HL: 0, SP: 0, PC: 0,
		    RAM: &mainMemory, tstates: 0, halt: 0, inte: 0};
	if(argc == 2){
		FILE *binfile = fopen(argv[1], "rb");
		if(!binfile){
			perror("Cant open ROM file");
			goto skip;
		}
		fseek(binfile, 0, SEEK_END);
		uint16_t len = ftell(binfile);
		rewind(binfile);
		uint8_t *tmprom = calloc(len, sizeof(uint8_t));
		fread(tmprom, sizeof(uint8_t), len, binfile);
		fclose(binfile);
		setROM(tmprom, len);
	}
skip:	for(int i = 0; i < slength; i++)
		sounds[i] = LoadSound(soundfiles[i]);
	for(int i = 0; i < ilength; i++)
		images[i] = LoadImage(imagefiles[i]);
	ImageResize(images + frontpanel, width, height);
	ImageResize(images + lightoff, 23, 23);
	ImageResize(images + lighton, 23, 23);
	ImageResize(images + switchntl, 25, 33);
	ImageResize(images + switchoff, 25, 33);
	ImageResize(images + switchon, 25, 33);
	for(int i = 0; i < ilength; i++)
		textures[i] = LoadTextureFromImage(images[i]);
	SetSoundVolume(sounds[humcont], 1.0f / 32.0f);
	SetSoundVolume(sounds[humstart], 1.0f / 32.0f);
	SetSoundVolume(sounds[Switchon], 1.0f / 2.0f);
	SetSoundVolume(sounds[Switchoff], 1.0f / 2.0f);
	/*Init end*/
	
	/*Main Loop Start*/
	while(!WindowShouldClose())
	{
	BeginDrawing();
		/*Input and sound stuff*/
		SoundAndEventHandling(&panel, sounds);

		/*Backend stuff*/
		panelLogic(&panel, &cpu);

		/*Graphics stuff*/
		DrawTexture(textures[frontpanel], 0, 0, WHITE);
		drawSwitchesAndLights(panel, textures);
	EndDrawing();
	}
	/*Main Loop End*/
	freeDRAM(cpu.RAM);
	freeROM();
}
