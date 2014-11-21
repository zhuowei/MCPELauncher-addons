#include <dlfcn.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>
#include <mcpelauncher.h>
#include <sys/time.h>

#define LOG_TAG "zhuoweisprint"

#define UP_KEY 0
#define DOWN_KEY 1

#define PLAYER_MOVEMENT_INPUT_OFFSET 3400

typedef struct {
	char randomCrap[140]; // from TouchscreenInput::releaseAllKeys; the first key released is 100
	char keyData[108];
} TouchScreenInput;	

static int zhuoweisprint_sprinting = 0; //todo multiplayer

static void* zhuoweisprint_player = NULL;

static long zhuoweisprint_lastTapTimes[8];

static char zhuoweisprint_lastKeyData[8];

static void (*zhuoweisprint_TouchScreenInput_tick_real)(TouchScreenInput*, void*);
static void (*zhuoweisprint_TouchscreenInput_render_real)(TouchScreenInput*, float);
static float (*zhuoweisprint_Player_getWalkingSpeedModifier_real)(void*);

long long zhuoweisprint_currentTimeMillis() {
	struct timeval tv;
	gettimeofday(&tv, (struct timezone *) NULL);
	long long when = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
	return when;
}

void zhuoweisprint_keydoubletapped(int key, TouchScreenInput* touchScreenInput) {
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Key double tapped: %i \n", key);
	if (key == UP_KEY) { //up
		zhuoweisprint_sprinting = 1;
	} else if (key == DOWN_KEY) { //down
		if (zhuoweisprint_player == 0) return;
		char* movementInput = *((char**) (((uintptr_t) zhuoweisprint_player) + PLAYER_MOVEMENT_INPUT_OFFSET));
		movementInput[14] = !movementInput[14];
	}
}

void zhuoweisprint_tickHook(TouchScreenInput* touchScreenInput, void* player) {

	zhuoweisprint_player = player;
	for (int i = 0; i < 8; i++) {
		if (zhuoweisprint_lastKeyData[i] != touchScreenInput->keyData[i + 100]) {
			__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Key toggled: %i \n", i);
			if (touchScreenInput->keyData[i + 100]) {
				long lastToggleTime = zhuoweisprint_lastTapTimes[i];
				long currentTime = zhuoweisprint_currentTimeMillis();
				long difference = currentTime - lastToggleTime;
				if (difference <= 50 * 14) {
					zhuoweisprint_keydoubletapped(i, touchScreenInput);
				}
				zhuoweisprint_lastTapTimes[i] = currentTime;
			} else {
				if (i == UP_KEY) {
					zhuoweisprint_sprinting = 0;
				}
			}
		}
		zhuoweisprint_lastKeyData[i] = touchScreenInput->keyData[i + 100];
	}
	zhuoweisprint_TouchScreenInput_tick_real(touchScreenInput, player);
}

float zhuoweisprint_getWalkingSpeedModifierHook(void* player) {
	if (zhuoweisprint_sprinting) return 1.3f;
	return 1.0f;
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//this gets called after the library's happily loaded and the Dalvik VM is ready
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "starting\n");

	void* inputTick = dlsym(RTLD_DEFAULT, "_ZN16TouchscreenInput4tickEP6Player");
	mcpelauncher_hook(inputTick, &zhuoweisprint_tickHook, &zhuoweisprint_TouchScreenInput_tick_real);

	void* walkingSpeed = dlsym(RTLD_DEFAULT, "_ZN6Player23getWalkingSpeedModifierEv");
	mcpelauncher_hook(walkingSpeed, &zhuoweisprint_getWalkingSpeedModifierHook, &zhuoweisprint_Player_getWalkingSpeedModifier_real);

	return JNI_VERSION_1_2;
}


