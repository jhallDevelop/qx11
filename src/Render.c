#include "Render.h"
#include <assert.h>
// Update the sprite position if active
void Render_UpdateDynamicActor(Sprite* _sprite, DynamicActor* _data){
	assert(_sprite != 0);
	assert(_data != 0);
	// if active and alive
	if(_data->pos.x != 0 || _data->pos.y != 0){
		TranslateSprite(_sprite, 
			_data->pos.x << delta_time, 
			_data->pos.y << delta_time);
	}

	// Visually update the sprite used

}

