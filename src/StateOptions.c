#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Print.h"

#include "Music.h"
DECLARE_MUSIC(Compression);

IMPORT_FONT(font);
IMPORT_MAP(menu_bg);

// Initialise the menu text
void InitOptionsText(void){
	INIT_FONT_EX(font, PRINT_BKG);
	PRINT(5, 8, "BACK");
}

void START(void) {
	// Music
//	PlayMusic(Compression,1);

	// Sound
	NR52_REG = 0x80;	// Enables sound, always set first
	NR51_REG = 0xFF;	// Enables all channels (left and right)
	NR50_REG = 0x77;	// Max volume


	// Load the background to clear the old boot logo map
	InitScroll(BANK(menu_bg), &menu_bg, 0, 0);

	// Initialise the menu
	InitOptionsText();
	
}

void UPDATE(void) {

	// Select the menu item
	if(KEY_TICKED(J_A) || KEY_TICKED(J_B)){
		SetState(StateMenu);

	}
}
