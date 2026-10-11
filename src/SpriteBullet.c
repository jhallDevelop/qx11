#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"

#define SPAWN_RANGE 60

#define BULLET_SPEED 1 

UINT8 g_bulletDataID;

void START(void){
	// initialise random numbers
	g_bulletDataID = MakeDynamicActor();
	THIS->custom_data[0] = g_bulletDataID;
}


void UPDATE(void){
	UINT8 collisionTile = 0;
	
	DynamicActor* bulletData;
	bulletData = GetDynamicActor(g_bulletDataID);
	// Move the bullet to the right, check for wall collisions
	
	// Move in direction
	collisionTile = TranslateSprite(THIS, (BULLET_SPEED << delta_time) * bulletData->dir.moveX, (BULLET_SPEED << delta_time) * bulletData->dir.moveY);


	// wall collision check
	if(collisionTile > 0){
		// remove the sprite
		SpriteManagerRemoveSprite(THIS);
	}


}

void DESTROY(void){
	FreeDynamicActor(g_bulletDataID);
}
