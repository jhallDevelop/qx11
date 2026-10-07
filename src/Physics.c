#include "Physics.h"
#include "assert.h"

void Physics_Update(DynamicActor* _actorData){
	assert(_actorData != 0);

	// Apply horizontal accell
	if(_actorData->dir.x != 0){
		_actorData->vel.x += _actorData->dir.x * _actorData->accelConst;
	} else{
	// Apply horizontal friction
		if(_actorData->vel.x > 0){
			_actorData->vel.x -= _actorData->frictionConst;	
		} else if(_actorData->vel.x < 0){
			_actorData->vel.x += _actorData->frictionConst;
		} else{
			_actorData->vel.x = 0;
		}
	}

	// Apply vert accell
	if(_actorData->dir.y != 0){
		_actorData->vel.y += _actorData->dir.y * _actorData->accelConst;
	} else{
	// Apply horizontal friction
		if(_actorData->vel.y > 0){
			_actorData->vel.y -= _actorData->frictionConst;	
		} else if(_actorData->vel.y < 0){
			_actorData->vel.y += _actorData->frictionConst;
		} else{
			_actorData->vel.y = 0;
		}
	}


	// cap velocity
	if(_actorData->vel.x > MAX_VELOCITY){
		_actorData->vel.x = MAX_VELOCITY;
	} else if(_actorData->vel.x < -MAX_VELOCITY){
		_actorData->vel.x = -MAX_VELOCITY;
	}

	
	if(_actorData->vel.y > MAX_VELOCITY){
		_actorData->vel.y = MAX_VELOCITY;
	} else if(_actorData->vel.y < -MAX_VELOCITY){
		_actorData->vel.y = -MAX_VELOCITY;
	}

	// Accumulate velocity at sub pixel
	_actorData->acc.x += _actorData->vel.x;
	_actorData->acc.y += _actorData->vel.y;

	// Convert to whole pixel
	_actorData->pos.x = _actorData->acc.x / DIV_SCALE;
	_actorData->pos.y = _actorData->acc.y / DIV_SCALE;

	// Keep remainder for next frame
	_actorData->acc.x = _actorData->acc.x % DIV_SCALE;
	_actorData->acc.y = _actorData->acc.y % DIV_SCALE;

}




