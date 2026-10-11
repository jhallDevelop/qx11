#include "ZGBMain.h"
#include "Banks/SetAutoBank.h"
#include "Keys.h"
#include "SpriteManager.h"
#include "Sound.h"
#include "ActorData.h"
#include "Scroll.h"
#include "Print.h"
#include "GameData.h"
#include "Physics.h"
#include "HUD.h"
#include "Sound.h"
#include "Shake.h"
#include "Render.h"

// Player movement physics

// Shooting
#define MAX_COOLDOWN 15
#define BULLET_SPAWN_DISTANCE 32


// TODO: Wrap these in a struct for each dynamic actor
// Sprites
const UINT8 anim_vert[]  = {1, 0}; 	// Top Left Vert aligned
const UINT8 anim_vertBack[] = {0,2};	// Bottom Left
const UINT8 anim_horiz[] = {1, 1};	// Top Right Horizontal aligned 
const UINT8 anim_diag[] = {1, 1};	// Bottom Left Diagonal
const UINT8 anim_flicker[] = {0, 2};	// Empty spot in sprite sheet


// Shooting
UINT8 g_shootCooldown = 0;



// Sprite Iters
UINT8 i;
Sprite* spr;
UINT8 g_playerID;

// Forward Declare
void UpdateInput(void);
void UpdateMovement(void);
void UpdateShooting(void);
void UpdateVisualEffects(void);
void UpdatePlayerCollisionCheck(void);

// Sound FX helpers
void BulletShootSoundFX(void);
void GoldPickupSoundFX(void);
void BulletHitSoundFX(void);

void START(void) {
	
	// Draw static coin at x coord 14
	UPDATE_HUD_TILE(14, 0, 4);


	// Allocate some actor data and store the pointer to it into custom_data
	g_playerID = MakeDynamicActor();
	THIS->custom_data[0] = g_playerID;

	// store a reference 
	UpdateHUD(g_playerID);


	
}

void UPDATE(void) {
	// BASE 17.38%
	
	UpdateInput();	//3% 
	
	// Update Physics
	Physics_Update(g_playerID);	// 4%

	Render_UpdateDynamicActor(THIS);	// .2%

	UpdateShooting();	// on fire 10%
		
	// Do Collision Check
	UpdatePlayerCollisionCheck();	// 1%
	
	// Flicker if required
	UpdateVisualEffects();	// .2%
	
}

void DESTROY(void) {
	FreeDynamicActor(g_playerID);
}

void BulletShootSoundFX(void){
	// Mute the background music whilst we play this
	// PlayFx(CHANNEL, MUTE_FRAMES, REGISTER_VALUES...)
	//PlayFx(CHANNEL_4, 10, 0x4f, 0xF7, 0xf3, 0x73, 0x86);
	// Play sound
	// PlayFx(CHANNEL_1, mute_frames, NR10, NR11, NR12, NR13, NR14)
	//PlayFx(CHANNEL_1, 0, 0x1D, 0x81, 0xF3, 0x73, 0xC6);
}

void BulletHitSoundFX(void){
	PlayFx(CHANNEL_4, 0, 0x05, 0xF4, 0x42, 0xC0);
}

void GoldPickupSoundFX(void){
	// Play sound
	PlayFx(CHANNEL_1, 0, 0x4f, 0xc7, 0xf3, 0x73);	

}

// Update input keys
void UpdateInput(void){
	DynamicActor* player;
	player = GetDynamicActor(g_playerID);
	player->dir.moveX = 0;
	player->dir.moveY = 0;

	// Calcuate X Velocity
	if(KEY_PRESSED(J_RIGHT)){
		player->dir.moveX = 1;
	} else if(KEY_PRESSED(J_LEFT)){
		player->dir.moveX = -1;
	}
	// Calculate Y Velocity
	if(KEY_PRESSED(J_DOWN)){
		player->dir.moveY = 1;
	} else if(KEY_PRESSED(J_UP)){
		player->dir.moveY = -1;
	}

	// check if only D-pad movmeent for facing and MOVING flag
	if(player->dir.moveX != 0 || player->dir.moveY != 0){
		// true
		player->dir.facingX = player->dir.moveX;
		player->dir.facingY = player->dir.moveY;
		player->stateFlags |= MOVING; 
	}else{
		// false
		player->stateFlags &= ~MOVING;
	}

	

	// Update facing if the player is pressing key

	// Diag Movement
	if(player->dir.moveX == 1 && player->dir.moveY == 1){			// Base: Down-Right
		THIS->mirror = NO_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);

	}else if(player->dir.moveX == -1 && player->dir.moveY == 1){		// Flip x: Down-Left
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);

	}else if (player->dir.moveX == 1 && player->dir.moveY == -1){		// Flip: Y: Up-Right
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);

	}else if(player->dir.moveX == -1 && player->dir.moveY == -1){		// Flip X and Y: Up Left
		//THIS->mirror = HV_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);

	}

	// Pure Vertical Movement
	else if(player->dir.moveY == 1){			// Base: Down
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_vert, 15);
	} else if(player->dir.moveY == -1){			// Flip Y: Up
		//THIS->mirror = HV_MIRROR; 
        	//SetSpriteAnim(THIS, anim_vert, 15);
		SetSpriteAnim(THIS, anim_vertBack, 15);
	}

	// Pure Horizontal 
	else if(player->dir.moveX == 1){			// Base: Right
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_horiz, 15);
	} else if(player->dir.moveX == -1){			// Flip x: Left
		THIS->mirror = V_MIRROR;
        	SetSpriteAnim(THIS, anim_horiz, 15);
	}



}


void UpdateShooting(void){
	DynamicActor* player;
	player = GetDynamicActor(g_playerID);
	// Decrease the cooldown timer every frame if greater than zero
	if(g_shootCooldown > 0){
		g_shootCooldown--;	
	}

	

	// Shoot
	if(KEY_TICKED(J_A) && g_shootCooldown == 0){
		// Shoot bullet in direction with an offset away from player sprite as to not collide
		// Use 16bit Int to avoid overflow issue
		INT16 spawnX = THIS->x;
		INT16 spawnY = THIS->y;
		
		if(player->dir.facingX == 1){
			spawnX += BULLET_SPAWN_DISTANCE;
		}else if(player->dir.facingX == -1){
			spawnX -= BULLET_SPAWN_DISTANCE;
		}

		if(player->dir.facingY == 1){
			spawnY += BULLET_SPAWN_DISTANCE;
		}else if(player->dir.facingY == -1){
			spawnY -= BULLET_SPAWN_DISTANCE;
		}

		Sprite* bullet = SpriteManagerAdd(SpriteBullet,
			spawnX,
			spawnY
		);

	// pass along some custom data
		DynamicActor* bulletData = (DynamicActor*)bullet->custom_data;

		bulletData->dir.moveX = player->dir.facingX;
		bulletData->dir.moveY = player->dir.facingY;
		g_shootCooldown = MAX_COOLDOWN;	// frames to wait before next shot
	}
}

void UpdateVisualEffects(void){
	/*
	if(g_player->flashTimer >0){
		g_player->flashTimer--;	// count down

		// Flicker sprite
		if((g_player->flashTimer % 2) != 0){
			SetSpriteAnim(THIS, anim_flicker, 15);
		}else{
			SetSpriteAnim(THIS, anim_vert, 15);
		}
	}*/

}

void UpdatePlayerCollisionCheck(void){
	DynamicActor* player;
	player = GetDynamicActor(g_playerID);
	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr->type == SpriteEnemy){
			if(CheckCollision(THIS, spr)){
				SetState(StateGame);
			}
		}

		if(spr->type == SpriteBullet){

			
			if(CheckCollision(THIS, spr)){
				if(player->health -1 <= 0){
					// Player Death
					SetState(StateMenu);
				}
				BulletHitSoundFX();
				//g_player->flashTimer = FLICKER_TIME;
    				ScreenShake(10, 3);
				// remove the bullet
				SpriteManagerRemoveSprite(spr);
		
				// Bullet subtract health
				player->health -= 1;
				// Update the UI
				UpdateHUD(g_playerID);
			}
		}

		if(spr->type == SpriteGold){
			GoldPickupSoundFX();
			// Add gold
			//targetData = (ActorData*)spr->custom_data;
			//targetData->gold += 1;
			// Global Game Data
			g_gameData.gold += 1;
			UpdateHUD(g_playerID);			      
			SpriteManagerRemoveSprite(THIS);

		}
	}
}
