#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
_STATE(StateMenu)\
_STATE(StateOptions)\
_STATE(StateCredits)\
STATE_DEF_END

#define SPRITES \
_SPRITE(SpritePlayer, player, FLIP_NONE)\
_SPRITE(SpriteEnemy, enemy, FLIP_NONE)\
_SPRITE(SpriteBullet, bullet, FLIP_NONE)\
_SPRITE(SpriteGold, gold, FLIP_NONE)\
_SPRITE(SpriteCursor, bullet, FLIP_NONE)\
_SPRITE(SpriteExplosion, explosion, FLIP_NONE)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif
