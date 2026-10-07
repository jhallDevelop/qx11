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

typedef struct Vec2 {
	INT8 x;
	INT8 y;
} Vec2;

typedef struct DynamicActor {
	UINT8 health;
	UINT8 stateFlags; // isAlive, isEnabled, isVisible, isAtacking
	UINT8 speed;
	Vec2 dir;	

	// Physics
	Vec2 pos;
	Vec2 vel;
	Vec2 acc;
	UINT8 accelConst;
	UINT8 frictionConst; 
} DynamicActor;

typedef struct StaticActor {
	UINT8 stateFlags;
	Vec2 pos;
} StaticActor;

// Init
void InitDynamicActor(DynamicActor* _data);
void InitStaticActor(StaticActor* _data);


#endif
