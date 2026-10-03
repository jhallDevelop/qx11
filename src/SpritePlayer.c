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

// Sprites
const UINT8 anim_vert[]  = {1, 0}; 	// Top Left Vert aligned
const UINT8 anim_horiz[] = {1, 1};	// Top Right Horizontal aligned 
const UINT8 anim_diag[] = {0, 2};	// Bottom Left Diagonal
const UINT8 anim_flicker[] = {2, 2};	// Empty spot in sprite sheet



// Movement
Direction g_direction = DIR_RIGHT;
// Ship Direction
INT8 dY = 0;
INT8 dX = 0;

// Player movement physics
#define PLAYER_ACCEL 1 
#define PLAYER_FRICTION 1

// Shooting
#define MAX_COOLDOWN 15
UINT8 g_shootCooldown = 0;
#define BULLET_SPAWN_DISTANCE 17



// Sprite Iters
UINT8 i;
Sprite* spr;
ActorData* g_player;

void UpdateInput(void);
void UpdateMovement(void);
void UpdateShooting(void);
void UpdateVisualEffects(void);
void UpdatePlayerCollisionCheck(void);


void START(void) {
	// Init the HUD

	
	
	// Clear the visible window layer by drawing empty spaces
	for(UINT8 i = 0; i < 20; i++){
		UPDATE_HUD_TILE(i, 0, 0); // x, y, tile index 0
	}

	// Draw static coin at x coord 14
	UPDATE_HUD_TILE(14, 0, 4);

	// Draw stats
	g_player = (ActorData*)THIS->custom_data;
	InitActor(g_player);
	UpdateHUD(g_player);

}

void UPDATE(void) {
	// BASE 17.38%
	
	UpdateInput();	//3% 
	
	// Update Physics
	PhysicsUpdate(g_player);	// 4%

	UpdateMovement();	// .2%

	UpdateShooting();	// on fire 10%
		
	// Do Collision Check
	UpdatePlayerCollisionCheck();	// 1%
	
	// Flicker if required
	UpdateVisualEffects();	// .2%
	
}

void DESTROY(void) {
}

// if there is any movement, then move the sprite
void UpdateMovement(void){
	if(g_player->moveX != 0 || g_player->moveY != 0){
		TranslateSprite(THIS, g_player->moveX << delta_time, g_player->moveY << delta_time);
		
	}

}

// Update input keys
void UpdateInput(void){
	if(keys == 0){
		return;
	}

	// zero direction
	// Calcuate X Velocity
	if(KEY_PRESSED(J_RIGHT)){
		g_player->velX += PLAYER_ACCEL;
		dX = 1;
	} else if(KEY_PRESSED(J_LEFT)){
		g_player->velX -= PLAYER_ACCEL;
		dX = -1;
	} else {
		dX = 0;
		// Apply horizontal friction
		if(g_player->velX > 0){
			g_player->velX -= PLAYER_FRICTION;
			
		} else if(g_player->velX < 0){
			g_player->velX += PLAYER_FRICTION;
		} else{
			g_player->velX = 0;
		}
	} 

	// Calculate Y Velocity
	if(KEY_PRESSED(J_DOWN)){
		g_player->velY += PLAYER_ACCEL;
		dY = 1;
	} else if(KEY_PRESSED(J_UP)){
		g_player->velY -= PLAYER_ACCEL;
		dY = -1;

	} else {
		dY = 0;
		// Apply vertical friction
		if(g_player->velY > 0){
			g_player->velY -= PLAYER_FRICTION;
		}else if(g_player->velY < 0){
			g_player->velY += PLAYER_FRICTION;
		} else {
			g_player->velY = 0;
		}
	}

	
	// Diag Movement
	if(dX == 1 && dY == 1){			// Base: Down-Right
		THIS->mirror = NO_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_DOWN_RIGHT;

	}else if(dX == -1 && dY == 1){		// Flip x: Down-Left
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_DOWN_LEFT;

	}else if (dX == 1 && dY == -1){		// Flip: Y: Up-Right
		THIS->mirror = V_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_UP_RIGHT;

	}else if(dX == -1 && dY == -1){		// Flip X and Y: Up Left
		THIS->mirror = HV_MIRROR; 
        	SetSpriteAnim(THIS, anim_diag, 15);
		g_direction = DIR_UP_LEFT;

	}

	// Pure Vertical Movement
	else if(dY == 1){			// Base: Down
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_vert, 15);
		g_direction = DIR_DOWN;
	} else if(dY == -1){			// Flip Y: Up
		THIS->mirror = HV_MIRROR; 
        	SetSpriteAnim(THIS, anim_vert, 15);
		g_direction = DIR_UP;
	}

	// Pure Horizontal 
	else if(dX == 1){			// Base: Right
		THIS->mirror = NO_MIRROR;
        	SetSpriteAnim(THIS, anim_horiz, 15);
		g_direction = DIR_RIGHT;
	} else if(dX == -1){			// Flip x: Left
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
				g_player->dirX = 0;
				g_player->dirY = -1;
			break;

			case DIR_DOWN:
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dirX = 0;
				g_player->dirY = 1;

			break;

			case DIR_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				g_player->dirX = -1;
				g_player->dirY = -0;

			break;

			case DIR_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				g_player->dirX = 1;
				g_player->dirY = 0;

			break;
			
			case DIR_UP_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				spawnY -= BULLET_SPAWN_DISTANCE;

				g_player->dirX = -1;
				g_player->dirY = -1;


			break;

			case DIR_UP_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				spawnY -= BULLET_SPAWN_DISTANCE;
				g_player->dirX = 1;
				g_player->dirY = -1;


			break;

	
			case DIR_DOWN_LEFT:
				spawnX -= BULLET_SPAWN_DISTANCE;
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dirX = -1;
				g_player->dirY = 1;


			break;

			case DIR_DOWN_RIGHT:
				spawnX += BULLET_SPAWN_DISTANCE;
				spawnY += BULLET_SPAWN_DISTANCE;
				g_player->dirX = 1;
				g_player->dirY = 1;

	
			break;
		}

		Sprite* bullet = SpriteManagerAdd(SpriteBullet, spawnX, spawnY);

		// pass along some custom data
		ActorData* bulletData = (ActorData*)bullet->custom_data;

		bulletData->dirX = g_player->dirX;
		bulletData->dirY = g_player->dirY;
		g_shootCooldown = MAX_COOLDOWN;	// frames to wait before next shot
	}

}

void UpdateVisualEffects(void){
	if(g_player->flashTimer >0){
		g_player->flashTimer--;	// count down

		// Flicker sprite
		if((g_player->flashTimer % 2) != 0){
			SetSpriteAnim(THIS, anim_flicker, 15);
		}else{
			SetSpriteAnim(THIS, anim_vert, 15);
		}
	}

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

				g_player->flashTimer = FLICKER_TIME;
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
