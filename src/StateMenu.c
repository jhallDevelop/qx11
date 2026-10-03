#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Print.h"
#include "SpriteManager.h"
#include "Music.h"
DECLARE_MUSIC(menu);


// Menu Cursor
Sprite* g_menuCursor;
UINT8 g_menuSelection = 0;
#define CURSOR_X	24
#define CURSOR_Y_OFFSET 64
#define CURSOR_Y_MOVE 	16
UINT8 g_cursorY = CURSOR_Y_OFFSET;
#define MAX_MENU_ITEMS 3

IMPORT_FONT(font);
IMPORT_MAP(menu_bg);


// TITLES
#define GAME_TITLE  "QX11"
#define START_GAME_TITLE "START"
#define OPTIONS_TITLE "OPTIONS"
#define CREDITS_TITLE "CREDITS"

// Initialise the menu text
void InitMenuText(void){
	INIT_FONT_EX(font, PRINT_BKG);	// PRINT_BKG to draw on the background, PRINT_WIN to draw on window
	PRINT(4, 2, GAME_TITLE);
	PRINT(5, 8, START_GAME_TITLE);
	g_menuCursor = SpriteManagerAdd(SpriteCursor, CURSOR_X, g_cursorY);

	PRINT(5, 10, OPTIONS_TITLE);

	PRINT(5, 12, CREDITS_TITLE);
}

void START(void) {
	
	// Music
	// PlayMusic(Compression,1);
	// Sound
	NR52_REG = 0x80;	// Enables sound, always set first
	NR51_REG = 0xFF;	// Enables all channels (left and right)
	NR50_REG = 0x77;	// Max volume


	// Load the background to clear the old boot logo map
	InitScroll(BANK(menu_bg), &menu_bg, 0, 0);

	// Initialise the menu
	InitMenuText();
	
	// start at selection 0
	g_menuSelection = 0;

	// Debugging
	//INIT_CONSOLE(font, 3, 2);
	
}

void UPDATE(void) {

	// Track the cursor movement based on joypad input
	// Scroll Down
	if(KEY_TICKED(J_DOWN)){
		g_menuSelection++;
		if(g_menuSelection >= MAX_MENU_ITEMS){
			g_menuSelection = 0;
		}
		g_menuCursor->y = CURSOR_Y_OFFSET + (g_menuSelection * CURSOR_Y_MOVE);
	}

	// Scroll Up
	if(KEY_TICKED(J_UP)){
		if(g_menuSelection == 0){
			g_menuSelection = MAX_MENU_ITEMS -1;
		}else{
			g_menuSelection --;
		}
		g_menuCursor->y = CURSOR_Y_OFFSET + (g_menuSelection * CURSOR_Y_MOVE);
	}
	
	//DPRINT_POS(1, 1);
	//DPrintf("cursor %d ", g_menuSelection);


	// Select the menu item
	if(KEY_TICKED(J_A) || KEY_TICKED(J_START)){
		switch(g_menuSelection){
			case 0: 
				SetState(StateGame);
			break;

			case 1:
				SetState(StateOptions);
			break;

			case 2:
				SetState(StateCredits);
			break;
		}
	}
}
