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
#define PLAYER_ACCEL 1 
#define PLAYER_FRICTION 2

// Shooting
#define MAX_COOLDOWN 15
#define BULLET_SPAWN_DISTANCE 17


// Sprites
const UINT8 anim_vert[]  = {1, 0}; 	// Top Left Vert aligned
const UINT8 anim_vertBack[] = {0,2};	// Bottom Left
const UINT8 anim_horiz[] = {1, 1};	// Top Right Horizontal aligned 
const UINT8 anim_diag[] = {1, 1};	// Bottom Left Diagonal
const UINT8 anim_flicker[] = {0, 2};	// Empty spot in sprite sheet


// TODO: Don't like these globals. 
// Movement
Direction g_direction = DIR_RIGHT;

// Shooting
UINT8 g_shootCooldown = 0;



// Sprite Iters
UINT8 i;
Sprite* spr;
DynamicActor* g_player;

// Forward Declare
void UpdateInput(void);
void UpdateMovement(void);
void UpdateShooting(void);
void UpdateVisualEffects(void);
void UpdatePlayerCollisionCheck(void);


void START(void) {
	
	// Draw static coin at x coord 14
	UPDATE_HUD_TILE(14, 0, 4);

	// Draw stats
	g_player = (DynamicActor*)THIS->custom_data;
	InitDynamicActor(g_player);
	UpdateHUD(g_player);


	//Init some physics attributes
	g_player->accelConst = PLAYER_ACCEL;
	g_player->frictionConst = PLAYER_FRICTION;
	
}

void UPDATE(void) {
	// BASE 17.38%
	
	UpdateInput();	//3% 
	
	// Update Physics
	Physics_Update(g_player);	// 4%

	Render_UpdateDynamicActor(THIS, g_player);	// .2%

	UpdateShooting();	// on fire 10%
		
	// Do Collision Check
	UpdatePlayerCollisionCheck();	// 1%
	
	// Flicker if required
	UpdateVisualEffects();	// .2%
	
}

void DESTROY(void) {
}


// Update input keys
void UpdateInput(void){

	// Calcuate X Velocity
	if(KEY_PRESSED(J_RIGHT)){
		//g_player->vel.x += PLAYER_ACCEL;
		g_player->dir.x = 1;
	} else if(KEY_PRESSED(J_LEFT)){
		//g_player->vel.x -= PLAYER_ACCEL;
		g_player->dir.x = -1;
	} else {
		g_player->dir.x = 0;
	}

	// Calculate Y Velocity
	if(KEY_PRESSED(J_DOWN)){
		g_player->dir.y = 1;
	} else if(KEY_PRESSED(J_UP)){
		g_player->dir.y = -1;

	} else {
		g_player->dir.y = 0;
	}
 

	

	// Diag Movement
	if(g_player->dir.x == 1 && g_player->dir.y == 1){			// Base: Down-Right
		THIS->mirror = NO_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_DOWN_RIGHT;

	}else if(g_player->dir.x == -1 && g_player->dir.y == 1){		// Flip x: Down-Left
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_DOWN_LEFT;

	}else if (g_player->dir.x == 1 && g_player->dir.y == -1){		// Flip: Y: Up-Right
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_UP_RIGHT;

	}else if(g_player->dir.x == -1 && g_player->dir.y == -1){		// Flip X and Y: Up Left
		//THIS->mirror = HV_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_UP_LEFT;

	}

	// Pure Vertical Movement
	else if(g_player->dir.y == 1){			// Base: Down
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_vert, 15);
		g_direction = DIR_DOWN;
	} else if(g_player->dir.y == -1){			// Flip Y: Up
		//THIS->mirror = HV_MIRROR; 
        	//SetSpriteAnim(THIS, anim_vert, 15);
		SetSpriteAnim(THIS, anim_vertBack, 15);
		g_direction = DIR_UP;
	}

	// Pure Horizontal 
	else if(g_player->dir.x == 1){			// Base: Right
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_horiz, 15);
		g_direction = DIR_RIGHT;
	} else if(g_player->dir.x == -1){			// Flip x: Left
		THIS->mirror = V_MIRROR;
        	SetSpriteAnim(THIS, anim_horiz, 15);
		g_direction = DIR_LEFT;
	}

}

void UpdateShooting(void){
	// Decrease the cooldown timer every frame if greater than zero
	if(g_shootCooldown > 0){
		g_shootCooldown--;	
	}

	

	// Shoot
	if(KEY_TICKED(J_A) && g_shootCooldown == 0){
		// Mute the background music whilst we play this
		// PlayFx(CHANNEL, MUTE_FRAMES, REGISTER_VALUES...)
		//PlayFx(CHANNEL_4, 10, 0x4f, 0xF7, 0xf3, 0x73, 0x86);
		// Play sound
		// PlayFx(CHANNEL_1, mute_frames, NR10, NR11, NR12, NR13, NR14)
		PlayFx(CHANNEL_1, 0, 0x1D, 0x81, 0xF3, 0x73, 0xC6);
		// Shoot bullet in direction
		INT16 spawnX = THIS->x;
		INT16 spawnY = THIS->y;
		switch(g_direction){
			case DIR_UP:
				spawnY -= BULLET_SPAWN_DISTANCE;
				g_player->dir.x = 0;
				g_player->dir.y = -1;
			break;

			case DIR_DOWN:
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dir.x = 0;
				g_player->dir.y = 1;

			break;

			case DIR_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				g_player->dir.x = -1;
				g_player->dir.y = -0;

			break;

			case DIR_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				g_player->dir.x = 1;
				g_player->dir.y = 0;

			break;
			
			case DIR_UP_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				spawnY -= BULLET_SPAWN_DISTANCE;

				g_player->dir.x = -1;
				g_player->dir.y = -1;


			break;

			case DIR_UP_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				spawnY -= BULLET_SPAWN_DISTANCE;
				g_player->dir.x = 1;
				g_player->dir.y = -1;


			break;

	
			case DIR_DOWN_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dir.x = -1;
				g_player->dir.y = 1;


			break;

			case DIR_DOWN_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dir.x = 1;
				g_player->dir.y = 1;

	
			break;
		}

		Sprite* bullet = SpriteManagerAdd(SpriteBullet, spawnX, spawnY);

		// pass along some custom data
		DynamicActor* bulletData = (DynamicActor*)bullet->custom_data;

		bulletData->dir.x = g_player->dir.x;
		bulletData->dir.y = g_player->dir.y;
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
	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr->type == SpriteEnemy){
			if(CheckCollision(THIS, spr)){
				SetState(StateGame);
			}
		}

		if(spr->type == SpriteBullet){

			
			if(CheckCollision(THIS, spr)){
				if(g_player->health -1 <= 0){
					// Player Death
					SetState(StateMenu);
				}

				//g_player->flashTimer = FLICKER_TIME;
    				PlayFx(CHANNEL_4, 0, 0x05, 0xF4, 0x42, 0xC0);
				ScreenShake(10, 3);
				// remove the bullet
				SpriteManagerRemoveSprite(spr);
		
				// Bullet subtract health
				g_player->health -= 1;
				// Update the UI
				UpdateHUD(g_player);
			}
		}

		if(spr->type == SpriteGold){
			// Play sound
			PlayFx(CHANNEL_1, 0, 0x4f, 0xc7, 0xf3, 0x73);	
			// Add gold
			//targetData = (ActorData*)spr->custom_data;
			//targetData->gold += 1;
			// Global Game Data
			g_gameData.gold += 1;
			UpdateHUD(g_player);			      
			SpriteManagerRemoveSprite(THIS);

		}
	}

}
