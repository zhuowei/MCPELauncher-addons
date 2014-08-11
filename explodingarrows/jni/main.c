// exploding arrow mod
// originally made for iOS by C0deh4cker
// ported to Android by 500 Internal Server Error

// headers - contain declarations of methods we need
// for bools
#include <stdbool.h>
// for null
#include <stddef.h>
// for dlopen and dlsym, used to get methods from libminecraftpe
#include <dlfcn.h>
// for Java Native Interface stuff
#include <jni.h>
// for printing out messages to the system log
#include <android/log.h>
// for accessing methods exported from BlockLauncher
// we'll copy this now
#include "mcpelauncher.h"

#define TAG "ExplodingArrows"

// I think the names for the params are optional; I'm including them here;
// clearer this way.
// typing on a touchscreen is NOT fun :(

typedef struct {
	void* vtable; //0
	char filler[32-4]; //4
	float x; //32
	float y; //36
	float z; //40
} Entity;

static void (*Entity_remove)(Entity* entity);
static void (*Level_explode)(void* level, Entity* entity, float x, float y, float z,
	float power);

static void (*Arrow_normalTick_real)(Entity* arrow);
static void (*Level_playSound_real)(void* level, Entity* entity, void* name,
	float something1, float something2);
// wait, we're done this part

static void* level;
static bool inArrowTick, arrowHit;

static void Arrow_normalTick_hook(Entity* arrow) {
	inArrowTick = true;
	Arrow_normalTick_real(arrow); // call the original method
	// How Can Arrows Be Real If Our NormalTicks Aren't Real?
	inArrowTick = false;
	if (arrowHit) {
		arrowHit = false;
		// 3.1 is size of TNT explosion, says the iOS authors
		Level_explode(level, arrow, arrow->x, arrow->y, arrow->z, 3.1f);
		Entity_remove(arrow);
	}
}

static void Level_playSound_hook(void* levelSelf, void* entity, void* name,
	float something1, float something2) {
	if (inArrowTick) {
		// when the arrow hits something, it makes a sound
		// so we'll use that to detect whether it hit
		// there are more... complicated ways of doing it,
		// but this is a port, so we'll stick with their version
		arrowHit = true;
		// I'm guessing why we don't explode here is to avoid double explosions
		// from the arrow making extra sounds. or something.
		level = levelSelf;
	}
	Level_playSound_real(levelSelf, entity, name, something1, something2);
	// 500ise can't copy and paste 2: electric boogaloo
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	// hook the methods we need for this
	// since we're porting a mod, they already specified which methods to hook.
	// hmmmmmmmmmmmmmmmmmm
	void* mcpelibhandle = dlopen("libminecraftpe.so", RTLD_LAZY);
	// clear any errors from the dynamic linker
	dlerror();
	// ask the linker to give us a handle to the lib.
	// these are all standard unix methods: Google is your friend
	Entity_remove = dlsym(mcpelibhandle, "_ZN6Entity6removeEv");
	// I've replayed Final Countdown a few times now as motivational music
	// :D
	// 500ise Can't Copy and Paste: the Movie
	Level_explode = dlsym(mcpelibhandle, "_ZN5Level7explodeEP6Entityffff");
	
	void* arrowTick = dlsym(mcpelibhandle, "_ZN5Arrow10normalTickEv");
	// replace the arrow::tick method with a call to our own Arrow_tick_hook method
	// store a pointer to a copy of the original method in Arrow_tick_real so we can 
	// call it in our hook
	mcpelauncher_hook(arrowTick, &Arrow_normalTick_hook, (void**) &Arrow_normalTick_real);
	void* levelPlaySound = dlsym(mcpelibhandle, "_ZN5Level9playSoundEP6EntityRKSsff");
	// aren't you bored yet?
	mcpelauncher_hook(levelPlaySound, &Level_playSound_hook, (void**) &Level_playSound_real);
	__android_log_print(ANDROID_LOG_INFO, TAG, "playsound %x %x", (int) levelPlaySound,
		(int) Level_playSound_real);
	// now we need to grab those method names
	// and to write the hooked methods
	// :( sigh. * starts playing intense focus music
	// you know how to get an unmangled objdump symbols output, right?
	// if not, YouTube/Google/manpages is your friend
	
	const char* error = dlerror(); // is there an error?
	if (error) {
		__android_log_print(ANDROID_LOG_INFO, TAG, "dlerror: %s", error); // print the error to the log
	}
	
	return JNI_VERSION_1_2;
}

