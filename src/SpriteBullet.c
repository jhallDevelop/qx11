#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"

#define SPAWN_RANGE 60

#define BULLET_SPEED 1 

DynamicActor* g_bulletData;

void START(void){
	// initialise random numbers
	g_bulletData = (DynamicActor*)THIS->custom_data;
	InitDynamicActor(g_bulletData);
	
	
}


void UPDATE(void){
	UINT8 collisionTile = 0;
	DynamicActor* bulletData = (DynamicActor*)THIS->custom_data;
	// Move the bullet to the right, check for wall collisions
	
	// Move in direction
	collisionTile = TranslateSprite(THIS, (BULLET_SPEED << delta_time) * bulletData->dir.x, (BULLET_SPEED << delta_time) * bulletData->dir.y);


	// wall collision check
	if(collisionTile > 0){
		// remove the sprite
		SpriteManagerRemoveSprite(THIS);
	}


}

void DESTROY(void){

}
