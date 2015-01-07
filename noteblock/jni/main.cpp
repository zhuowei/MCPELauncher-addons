#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>
#include "SoundDesc.h"
#include "MusicTile.h"
#include "SoundPool.h"

#define LOG_TAG "noteblock"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))

using namespace android_media_SoundPool;

static const int musicTileId = 25;

static void hexdump(char* d, size_t size) {
	for (size_t s = 0; s < size; s++) {
		LOGI("%hhx ", d[s]);
	}
}

// not used in the final program; left here for debugging
static void bl_dumpVtable(void** vtable, size_t size) {
	// thanks, MrARM
	Dl_info info;
	for (int i = 0; i < (size / sizeof(void*)); i++) {
		if (!dladdr(vtable[i], &info)) {
			__android_log_print(ANDROID_LOG_INFO, "BlockLauncher", "BLANK // %d", i);
		} else {
			__android_log_print(ANDROID_LOG_INFO, "BlockLauncher", "%s; // %d", info.dli_sname, i);
		}
	}
}

static void (*Tile_initTiles_real)();

static void Tile_initTiles_hook() {
	Tile_initTiles_real();
	// make a block
	MusicTile* musicTile = new MusicTile(musicTileId, "cobblestone", Material::stone);
	// add it to the block list
	Tile::tiles[musicTileId] = musicTile;
	// now make a TileItem so that the block can be held in the player's inventory
	TileItem* musicItem = new TileItem(musicTileId - 256);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	void** yepVtable = (void**) dlsym(RTLD_DEFAULT, "_ZTV10TileEntity");
	bl_dumpVtable(yepVtable, 0x3c);
	void** yepVtable2 = (void**) dlsym(RTLD_DEFAULT, "_ZTV10EntityTile");
	bl_dumpVtable(yepVtable2, 0x12c);
	/*void* todump = dlsym(RTLD_DEFAULT, "SA_burp");
	hexdump((char*) todump, 0x20);
	SoundDesc* desc = (SoundDesc*) todump;
	Dl_info info;
	LOGI("AAAAAAA %s", desc->wtf3.c_str());;
	LOGI("Hello world!");*/
	JNIEnv* env;
	vm->AttachCurrentThread(&env, NULL);
	SoundPool::InitJNI(env);
	MusicTile::initSoundPool();

	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	
	return JNI_VERSION_1_2;
}
