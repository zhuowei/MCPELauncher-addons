#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <mcpe.h>
#include <Substrate.h>

const int baconTileId = 200;

class BaconTile : public Tile {
public:
	using Tile::Tile;
	virtual int getColor(TileSource*, int, int, int); // 48
};

int BaconTile::getColor(TileSource* tileSource, int x, int y, int z) {
	return ((x&1) ^ (z&1))? 0xffff4040: 0xffff8040;
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

static void (*Tile_initTiles_real)();

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

static void Tile_initTiles_hook() {
	Tile_initTiles_real();
	// make a block
	BaconTile* baconTile = new BaconTile(baconTileId, "cobblestone", &Material::stone);
	// add it to the block list
	Tile::tiles[baconTileId] = baconTile;
	// now make a TileItem so that the block can be held in the player's inventory
	TileItem* tileItem = new TileItem(baconTileId - 256);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//void** yepVtable = (void**) dlsym(RTLD_DEFAULT, "_ZTV4Tile");
	//bl_dumpVtable(yepVtable, 0x128);
	MSHookFunction((void*) &Tile::initTiles, (void*) &Tile_initTiles_hook, (void**) &Tile_initTiles_real);
	return JNI_VERSION_1_2;
}
