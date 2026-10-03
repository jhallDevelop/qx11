#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"

#define SPAWN_RANGE 60

#define BULLET_SPEED 1 

ActorData* g_bulletData;

void START(void){
	// initialise random numbers
	g_bulletData = (ActorData*)THIS->custom_data;
	InitActor(g_bulletData);
	
	
}


void UPDATE(void){
	UINT8 collisionTile = 0;
	ActorData* bulletData = (ActorData*)THIS->custom_data;
	// Move the bullet to the right, check for wall collisions
	
	INT8 dirX = bulletData->dirX;
	INT8 dirY = bulletData->dirY;

	// Move in direction
	collisionTile = TranslateSprite(THIS, (BULLET_SPEED << delta_time) * dirX, (BULLET_SPEED << delta_time) * dirY);


	// wall collision check
	if(collisionTile > 0){
		// remove the sprite
		SpriteManagerRemoveSprite(THIS);
	}


}

void DESTROY(void){

}
