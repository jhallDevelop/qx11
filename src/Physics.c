#include "Physics.h"


void PhysicsUpdate(ActorData* _actorData){
	// cap velocity
	if(_actorData->velX > MAX_VELOCITY){
		_actorData->velX = MAX_VELOCITY;
	} else if(_actorData->velX < -MAX_VELOCITY){
		_actorData->velX = -MAX_VELOCITY;
	}

	
	if(_actorData->velY > MAX_VELOCITY){
		_actorData->velY = MAX_VELOCITY;
	} else if(_actorData->velY < -MAX_VELOCITY){
		_actorData->velY = -MAX_VELOCITY;
	}

	// Accumulate velocity at sub pixel
	_actorData->accX += _actorData->velX;
	_actorData->accY += _actorData->velY;

	// Convert to whole pixel
	_actorData->moveX = _actorData->accX / DIV_SCALE;
	_actorData->moveY = _actorData->accY / DIV_SCALE;

	// Keep remainder for next frame
	_actorData->accX = _actorData->accX % DIV_SCALE;
	_actorData->accY = _actorData->accY % DIV_SCALE;

	

}



