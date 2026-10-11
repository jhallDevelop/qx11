#include "Render.h"
#include <assert.h>
#include "ActorData.h"

// Update the sprite position if active
void Render_UpdateDynamicActor(Sprite* _sprite){
	assert(_sprite != 0);
	DynamicActor* _data = GetDynamicActor(_sprite->custom_data[0]);
	
	TranslateSprite(_sprite, 
		_data->pos.x << delta_time, 
		_data->pos.y << delta_time);

	// Visually update the sprite used

}

