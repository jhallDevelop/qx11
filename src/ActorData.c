#include "ActorData.h"

void InitActor(ActorData* _data){
	_data->health = 0;
	//_data->shield = 0;
	_data->flashTimer = 0;
	_data->dirX = 0;
	_data->dirY = 0;
	_data->velX = 0;
	_data->velY = 0;
	_data->accX = 0;
	_data->accY = 0;
	_data->moveX = 0;
	_data->moveY = 0;
}

