#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"
#include "Physics.h"
#include "Render.h"

#define SPAWN_RANGE 60
#define BULLET_SPEED 1 

UINT8 g_bulletDataID;

void START(void){
	// initialise random numbers
	g_bulletDataID = MakeDynamicActor();
	THIS->custom_data[0] = g_bulletDataID;
}


void UPDATE(void){
	Physics_Update(g_bulletDataID);	// 4%

	Render_UpdateDynamicActor(THIS);	// .2%

}

void DESTROY(void){
	FreeDynamicActor(g_bulletDataID);
}
