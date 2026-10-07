#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Music.h"
#include "Shake.h"
#include "GameData.h"

// MUSIC
DECLARE_MUSIC(Compression);

// MAP
IMPORT_MAP(island);

// List every solid tile ID. Skip 4. The list must end with 0.
UINT8 collision_tiles[] = {1, 0};//{1, 3,4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0};




// Game data
GameData g_gameData;


// Declare Functions
void InitGame(void);
void SpawnActors(void);


void START(void) {
	InitGame();
}

void UPDATE(void) {

}

void DESTROY(void){
	UninstallShaker();
}





// InitGame
void InitGame(void){
	// Music
	// PlayMusic(Compression,1);

	// Sound
	NR52_REG = 0x80;	// Enables sound, always set first
	NR51_REG = 0xFF;	// Enables all channels (left and right)
	NR50_REG = 0x77;	// Max volume

		
	// Init the game mapp
	InitScroll(BANK(island), &island, collision_tiles, 0);

	InitHUD();
	
	// Add screen shake
	InstallShaker();

	// spwan Player and enemy
	SpawnActors();


}

void SpawnActors(void){
	scroll_target = SpriteManagerAdd(SpritePlayer, 50, 50);
	//SpriteManagerAdd(SpriteEnemy, 100, 50);
}
