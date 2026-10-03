#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include "ActorData.h"
#include "Sound.h"

#define MAX_EXPLOSION_FRAMES 6
const UINT8 anim_explode []= {MAX_EXPLOSION_FRAMES, 0, 1, 2, 3, 4, 5};

void START(void){
	SetSpriteAnim(THIS, anim_explode, 8);
}

void UPDATE(void){
	// Delete once done
	if(THIS->anim_frame == (MAX_EXPLOSION_FRAMES-1)){
		SpriteManagerAdd(SpriteGold, THIS->x, THIS->y);
		SpriteManagerRemoveSprite(THIS);
	}
}

void DESTROY(void){

}
