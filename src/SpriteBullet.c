#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"
#include "Physics.h"
#include "Render.h"

#define SPAWN_RANGE 60
#define BULLET_SPEED 1 

void START(void){
	// initialise random numbers
	
	UINT8 bulletDataID = MakeDynamicActor();
	THIS->custom_data[0] = bulletDataID;
	DynamicActor* bulletData = GetDynamicActor(bulletDataID);
	bulletData->stateFlags |= MOVING;
}


void UPDATE(void){
	Physics_Update(THIS->custom_data[0]);	// 4%

	Render_UpdateDynamicActor(THIS);	// .2%

}

void DESTROY(void){
	FreeDynamicActor(THIS->custom_data[0]);
}
