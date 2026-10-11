#include "HUD.h"
#include "ZGBMain.h"
#include "Print.h"
#include "GameData.h"
#include <assert.h>
#include "ActorData.h"
// Player Stats
#define MAX_LIFE 3
#define MAX_SHIELDS 3
#define MAX_COINS 16

// FONT
IMPORT_FONT(font);

// HUD 
IMPORT_MAP(hud);

void InitHUD(void){
	// Init the HUD
	INIT_HUD(hud);

	INIT_FONT_EX(font, PRINT_WIN);

	// Clear the visible window layer by drawing empty spaces
	for(UINT8 i = 0; i < 20; i++){
		UPDATE_HUD_TILE(i, 0, 0); // x, y, tile index 0
	}

}

void UpdateHUD(UINT8 _playerID){
	DynamicActor* _player = GetDynamicActor(_playerID);
	assert(_player != 0);	// null check
	// Draw the hearts
	for(UINT8 i = 0; i < MAX_LIFE; i++){
		if(i < _player->health){
			UPDATE_HUD_TILE(i, 0, 2);	// Draw full heart (index 1)
		}else{
			UPDATE_HUD_TILE(i, 0, 1);	// Draw Empty Heart (Index 0)
		}
	}

	/*
	// Draw Shields (shift right to x coord 4, 5, 6
	for(INT8 i = 0; i < MAX_SHIELDS; i++){
		if(i < _player->shield){
			UPDATE_HUD_TILE(4 + i, 0, 3);	// Draw Full Shield (index 3)	
		}else{
			UPDATE_HUD_TILE(4 + i, 0, 3);	// Draw empty shield (index 2)
		}
			
	}*/

	// Print out coin count
	print_target = PRINT_WIN;
	PRINT(15, 0, "%i", g_gameData.gold);

}


