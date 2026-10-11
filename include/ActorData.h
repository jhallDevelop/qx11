#ifndef ACTOR_DATA_H
#define ACTOR_DATA_H
#include <gb/gb.h>

#define MAX_DYNAMIC_ACTORS 16
#define MAX_STATIC_ACTORS 4



typedef enum e_ActorState{
	ALIVE = (1 << 0),
	ENABLED = (1 << 1),
	VISIBLE = (1 << 2),
	ATTACKING = (1 << 3),
	MOVING = (1 << 4)
} e_ActorState;

typedef struct Vec2 {
	INT8 x;
	INT8 y;
} Vec2;

typedef struct Direction{
	INT8 moveX : 2;		// Bit 0-1
	INT8 moveY : 2; 	// Bit 2-3
	INT8 facingX : 2; 	// Bit 4-5
	INT8 facingY : 2;	// Bit 6-7
} Direction;

typedef struct DynamicActor {
	UINT8 id;
	UINT8 health;
	e_ActorState stateFlags; // isAlive, isEnabled, isVisible, isAtacking
	
	// Physics
	Vec2 pos;
	Direction dir;
	Vec2 vel;
	Vec2 acc;
} DynamicActor;

typedef struct StaticActor {
	UINT8 id;
	UINT8 stateFlags;
	Vec2 pos;
} StaticActor;

extern DynamicActor g_dynamicActorPool[MAX_DYNAMIC_ACTORS];
extern StaticActor g_staticActorPool[MAX_STATIC_ACTORS];


// Init
void InitDynamicActor(DynamicActor* _data);
void InitStaticActor(StaticActor* _data);


// Actor Pool Accessors
UINT8 MakeDynamicActor(void);
void FreeDynamicActor(UINT8 _id);
DynamicActor* GetDynamicActor(UINT8 _actorID);
StaticActor* GetStaticActor(UINT8 _actorID);
#endif
