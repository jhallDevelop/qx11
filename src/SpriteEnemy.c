#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"
#include "ActorData.h"
#include "rand.h"
#include "Physics.h"
#include "Sound.h"
#include "Shake.h"
#include "GameData.h"

#define MAX_HEALTH 3

// Spawning
#define ENEMY_RESPAWN_DISTANCE 16

// Shooting
#define MAX_COOLDOWN 15
UINT8 g_enemyShootCooldown = 0;
#define ENEMY_BULLET_SPAWN_DISTANCE 16


// Sprites
const UINT8 anim_normal[] = {1, 0};
const UINT8 anim_enemyFlicker[] = {1,0};


// MOVEMENT Physics
#define ENEMY_ACCEL 1
#define ENEMY_FRICTION 4

// ai move timer
// move 2 seconds then 
#define MAX_MOVE_TIMER 120 
// init with max, so we get random pos next frame
UINT8 g_moveTimer = MAX_MOVE_TIMER;
INT8 g_nextXPos = 0;
INT8 g_nextYPos = 0;

UINT8 g_enemyID;


void UpdateEnemyCollisionCheck(void);

void START(void){
	g_enemyID = MakeDynamicActor();
	THIS->custom_data[0] = g_enemyID;
	DynamicActor* data;
	data = GetDynamicActor(g_enemyID);
	data->health = MAX_HEALTH;

	// init random numbers
	initrand(DIV_REG);

	// debug printing
}

void AIUpdate(void){
	/*
	// Get player sprite data
	DynamicActor* actor = (DynamicActor*)THIS->custom_data;

	// Shoot towards the player, then reset count
	if(g_enemyShootCooldown == 0){

		INT16 spawnX = THIS->x;
		INT16 spawnY = THIS->y;
		
		switch(g_enemyDirection){
			case DIR_UP:
				spawnY -= ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = 0;
				actor->dir.y = -1;
			break;

			case DIR_DOWN:
				spawnY += ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = 0;
				actor->dir.y = 1;

			break;

			case DIR_LEFT:
				spawnX -= ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = -1;
				actor->dir.y = -0;

			break;

			case DIR_RIGHT:
				spawnX += ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = 1;
				actor->dir.y = 0;

			break;
			
			case DIR_UP_LEFT:
				spawnX -= ENEMY_BULLET_SPAWN_DISTANCE;
				spawnY -= ENEMY_BULLET_SPAWN_DISTANCE;

				actor->dir.x = -1;
				actor->dir.y = -1;


			break;

			case DIR_UP_RIGHT:
				spawnX += ENEMY_BULLET_SPAWN_DISTANCE;
				spawnY -= ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = 1;
				actor->dir.y = -1;


			break;

	
			case DIR_DOWN_LEFT:
				spawnX -= ENEMY_BULLET_SPAWN_DISTANCE;
				spawnY += ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = -1;
				actor->dir.y = 1;


			break;

			case DIR_DOWN_RIGHT:
				spawnX += ENEMY_BULLET_SPAWN_DISTANCE;
				spawnY += ENEMY_BULLET_SPAWN_DISTANCE;
				actor->dir.x = 1;
				actor->dir.y = 1;

	
			break;
		}
		Sprite* bullet = SpriteManagerAdd(SpriteBullet, spawnX, spawnY);
		DynamicActor* bulletData = (DynamicActor*)bullet->custom_data;
		// Set the bullet direction
		bulletData->dir.x = actor->dir.x;
		bulletData->dir.y = actor->dir.y;
		g_enemyShootCooldown = MAX_COOLDOWN;	// frames to wait before next shot		
	}
	
	// Keep counting down
	if(g_enemyShootCooldown > 0){
		g_enemyShootCooldown--;
	}

	// Update the move timer
	if(g_moveTimer >= MAX_MOVE_TIMER){
		g_moveTimer = 0;
		// find a new position near to move towards
		initrand(DIV_REG);
		//g_nextXPos = THIS->x + (rand() % ENEMY_RESPAWN_DISTANCE) + 1;
		//g_nextYPos = THIS->y + (rand() % ENEMY_RESPAWN_DISTANCE) + 1;

		// create the normalised direction using branchless one liner
		// C boolean comparisons evaluate to 1 (true), 0 (false),
		// subtract results of two comparisons to create branchless clamp
		//actor->dirX = (g_nextXPos > THIS->x) - (g_nextXPos < THIS->x);
		//actor->dirY = (g_nextYPos > THIS->y) - (g_nextYPos < THIS->y);
		actor->dir.x = (rand() % 3) -1;
		actor->dir.y = (rand() % 3) -1;

	}
	// increment timer
	g_moveTimer++;
	*/
	
}

void UPDATE(void){
	
	DynamicActor* actorData = GetDynamicActor(g_enemyID);

	// move towards direction
	Physics_Update(g_enemyID);


	UpdateEnemyCollisionCheck();
	/*
	// Process the flash animation if the timer is above zero
	if(actorData->flashTimer > 0){
		actorData->flashTimer--;	// count down

	// Flicker Sprite
		if((actorData->flashTimer % 2) != 0){
			// Flicker Sprite
			SetSpriteAnim(THIS, anim_enemyFlicker, 15);
		}else {
			// Return sprite to normal 
			SetSpriteAnim(THIS, anim_normal, 15);
		}
	}*/

	AIUpdate();
}

void DESTROY(void){

}

void UpdateEnemyCollisionCheck(void){
	/*
	UINT8 i;
	Sprite* spr;

	// Check collision from bullets perspective
	SPRITEMANAGER_ITERATE(i, spr){
		if(spr->type == SpriteBullet){
			if(CheckCollision(THIS, spr)) {
				// Destory both the enemy and bullet
				DynamicActor* targetData = (DynamicActor*)THIS->custom_data;
				targetData->health -= 1;			      
									
				// Remove the Bullet
				SpriteManagerRemoveSprite(spr);

				// If the enemy health is below 0, 
				// delete the enemy and spawn gold
				if(targetData->health <=0){
					// Explosion sound
					//PlayFx(CHANNEL_4, 15, 0x0c, 0xF7, 0x30, 0xc0);
					// Deep low-frequency rumble explosion
    					PlayFx(CHANNEL_4, 0, 0x01, 0xF7, 0x67, 0xC0);

					// Add the explosion
					SpriteManagerAdd(SpriteExplosion, spr->x, spr->y);

				
					// Add a new random Enemy near bullet
					initrand(DIV_REG);
					UINT8 randX = THIS->x + (rand() % ENEMY_RESPAWN_DISTANCE) + 1;
					UINT8 randY = THIS->y + (rand() % ENEMY_RESPAWN_DISTANCE) + 1;
					SpriteManagerAdd(SpriteEnemy, randX, randY);

					// Shake Screen
					ScreenShake(10, 4);

					// Remove self
					SpriteManagerRemoveSprite(THIS);

				} else{
					// Play bounce effect
					// Punchy mid-frequency thud when enemy absorbs bullet
					//targetData->flashTimer = FLICKER_TIME;
    					PlayFx(CHANNEL_4, 0, 0x05, 0xF4, 0x42, 0xC0);
				
				}
				SpriteManagerRemoveSprite(spr);
					
				return;
			}
		}
	}
	*/

}




