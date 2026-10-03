#ifndef __SHAKEVBLHANDLER_INCLUDE__
#define __SHAKEVBLHANDLER_INCLUDE__

#define SHAKE_NONE  0x00
#define SHAKE_X     0x01
#define SHAKE_Y     0x02

#define SHAKE_RANGE 4

void ShakeVBL_ISR(void);

extern UINT8 shake_duration, shake_type, shake_intensivity; 

#pragma save
#pragma disable_warning 126
inline void Shake(UINT8 duration, UINT8 type, UINT8 intensivity) {
	shake_duration = duration; shake_type = type; shake_intensivity = ((intensivity > SHAKE_RANGE) ? SHAKE_RANGE : intensivity) << 1;
}
#pragma restore

inline void InstallShaker(void) {
	CRITICAL {
		remove_VBL(ShakeVBL_ISR);
		add_VBL(ShakeVBL_ISR);
	}
}

inline void UninstallShaker(void) {
	CRITICAL {
		remove_VBL(ShakeVBL_ISR);
		add_VBL(ShakeVBL_ISR);
	}
}


//void ScreenShake(UINT8 _duration, UINT8 _intensivity);
// Control Screen Shake
inline void ScreenShake(UINT8 _duration, UINT8 _intensivity){
	Shake(_duration, SHAKE_X, _intensivity);
	Shake(_duration, SHAKE_Y, _intensivity);
}

#endif
