#ifndef ACTOR_DATA_H
#define ACTOR_DATA_H
#include <gb/gb.h>

typedef enum Direction {
	DIR_UP, 
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP_LEFT,
	DIR_UP_RIGHT,
	DIR_DOWN_LEFT,
	DIR_DOWN_RIGHT
} Direction;

typedef struct ActorData {
	INT8 health;
	//INT8 shield;
	INT8 flashTimer;
	INT8 dirX;
	INT8 dirY;

	// Physics
	INT8 velX;
	INT8 velY;
	INT8 accX;
	INT8 accY;
	INT8 moveX;
	INT8 moveY;
} ActorData;

void InitActor(ActorData* _data);


#endif
