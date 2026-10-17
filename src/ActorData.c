#include "ActorData.h"
#include <assert.h>

DynamicActor g_dynamicActorPool[MAX_DYNAMIC_ACTORS];
StaticActor g_staticActorPool[MAX_STATIC_ACTORS];

// Initialisers
void InitDynamicActor(DynamicActor* _data){
	assert(_data != 0);
	_data->id = 255;	
	_data->health = 0;
	_data->stateFlags = 0;
	_data->pos.x = 0;
	_data->pos.y = 0;
	_data->dir.moveX = 0;
	_data->dir.moveY = 0;
	_data->dir.facingX = 0;
	_data->dir.facingY = 1;	
	_data->vel.x = 0;
	_data->vel.y = 0;
	_data->acc.x = 0;
	_data->acc.y = 0;

}

void InitStaticActor(StaticActor* _data){
	assert(_data != 0);
	_data->id = 255;
	_data->stateFlags = 0;
	_data->pos.x = 0;
	_data->pos.y = 0;
}

// Dynamic Actor Pool Accessors

UINT8 MakeDynamicActor(void){
	for(UINT8 i = 0; i < MAX_DYNAMIC_ACTORS; i++){
		// find the next actor that isn't already alive
		if(!(g_dynamicActorPool[i].stateFlags & ALIVE)){
			// Initialise the data
			InitDynamicActor(&g_dynamicActorPool[i]);

			// set this block to alive
			g_dynamicActorPool[i].stateFlags |= ALIVE;
			return i;
		}
	}
	return 255; // ERROR: Pool is full
}

void FreeDynamicActor(UINT8 _id){
	if(_id < MAX_DYNAMIC_ACTORS){
		// clear all the data
		InitDynamicActor(&g_dynamicActorPool[_id]);
	}
}

// Static Actor Pool Accessors

UINT8 MakeStaticActor(void){
	UINT8 i; 
	for(i = 0; i < MAX_STATIC_ACTORS; i++){
		// find the next actor that isn't already alive
		if(g_staticActorPool[i].stateFlags & ALIVE){
			// Init the data
			InitStaticActor(&g_staticActorPool[i]);
			// set this block to alive
			g_staticActorPool[i].stateFlags |= ALIVE;
			return i;
		}
	}
	return 255; // ERROR: Pool is full
}

void FreeStaticActor(UINT8 _id){
	if(_id < MAX_STATIC_ACTORS){
		// clear all the data
		InitStaticActor(&g_staticActorPool[_id]);
	}
}

DynamicActor* GetDynamicActor(UINT8 _actorID){
	assert(_actorID < MAX_DYNAMIC_ACTORS);
	return &g_dynamicActorPool[_actorID];

}

StaticActor* GetStaticActor(UINT8 _actorID){
	assert(_actorID < <MAX_STATIC_ACTORS);
	return &g_staticActorPool[_actorID];
}
