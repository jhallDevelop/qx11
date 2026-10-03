#ifndef GAMEDATA_H
#define GAMEDATA_H

#define FLICKER_TIME 4

typedef struct GameData{
	UINT8 gold;
} GameData;

// Make globabally accessible
extern GameData g_gameData;

#endif
