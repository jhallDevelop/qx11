#include "ActorData.h"

void InitDynamicActor(DynamicActor* _data){
	_data->health = 0;
	_data->stateFlags = 0;
	_data->speed = 0;
	_data->dir.x = 0;
	_data->dir.y = 0;
	_data->pos.x = 0;
	_data->pos.y = 0;
	_data->vel.x = 0;
	_data->vel.y = 0;
	_data->acc.x = 0;
	_data->acc.y = 0;
	_data->accelConst = 0;
	_data->frictionConst = 0;

}

void IniStaticActor(StaticActor* _data){
	_data->stateFlags = 0;
	_data->pos.x = 0;
	_data->pos.y = 0;
}


