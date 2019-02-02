#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
u8 cfgData[4]={0};
void fixConfig(){
	Result res=0;
	res = CFG_GetConfigInfoBlk4(4, 0x00110000, cfgData);
	printf("\ngetConfig: res-%08X val-%08lX\n", (int)res, *(u32*)cfgData);
	if(*(u16*)(cfgData)) {
		printf("There's a time and a place for config modding,\nbut not now.\n");
		return;
	}
	cfgData[0]=1;
	res = CFG_SetConfigInfoBlk4(4, 0x00110000, cfgData);  //update the nand config savegame to skip setup which fixes a format bricked 2ds
	printf("setConfig: res-%08X val-%08lX\n", (int)res,  *(u32*)cfgData);
	res = CFG_UpdateConfigSavegame();                         //confirm our changes
	printf("updateConfig: %08X\n", (int)res);
}

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	printf("2DSaver - zoogie (luma)\n");

	Result res;
	res = nsInit();
	res = cfguInit();
	printf("configInit: %08X\n", (int)res);
	res = CFG_GetConfigInfoBlk4(4, 0x00110000, cfgData);
	printf("getConfig: res-%08X val-%08lX\n\n", (int)res, *(u32*)cfgData);
	printf("Press A to fix 3d slider brick, START to exit\n");

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		
		if (kDown & KEY_A)
			fixConfig(); 
		if (kDown & KEY_START)
			 NS_RebootSystem();
	}

	gfxExit();
	return 0;
}
