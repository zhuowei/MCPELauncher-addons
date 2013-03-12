#include <dlfcn.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>
#include <mcpelauncher.h>

typedef struct {
  char name[128];
  const void* phdr;
  int phnum;
  unsigned entry;
  unsigned base;
  unsigned size;
} soinfo2;

static int zhuoweixray_enabled = 1;

static int zhuoweixray_renderInXrayMode[0x100];

static int (*zhuoweixray_Tile_shouldRenderFace_real)(void*, void*, int, int, int, int);
static float (*zhuoweixray_Tile_getBrightness_real)(void*, void*, int, int, int);
static void* (*zhuoweixray_WeaponItem_mineBlock_real)(void*, void*, void*, void*);

int zhuoweixray_shouldRenderFaceXray(void* tile, void* level, int x, int y, int z, int face) {
	if (zhuoweixray_enabled) {
		int blockId = *((int*) ((int) tile + 8));
		return zhuoweixray_renderInXrayMode[blockId];
	}
	return zhuoweixray_Tile_shouldRenderFace_real(tile, level, x, y, z, face);
}

float zhuoweixray_getBrightnessXray(void* tile, void* level, int x, int y, int z) {
	if (zhuoweixray_enabled) {
		return 1.0;
	}
	return zhuoweixray_Tile_getBrightness_real(tile, level, x, y, z);
}

void* zhuoweixray_swordBreakBlock(void* item, void* itemStack, void* level, void* player) {
	__android_log_print(ANDROID_LOG_INFO, "zhuoweixray", "sword use fn\n");
	zhuoweixray_enabled = !zhuoweixray_enabled;
	return zhuoweixray_WeaponItem_mineBlock_real(item, itemStack, level, player);
}


JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//this gets called after the library's happily loaded and the Dalvik VM is ready
	__android_log_print(ANDROID_LOG_INFO, "zhuoweixray", "starting\n");

	void* shouldRenderFace = dlsym(RTLD_DEFAULT, "_ZN4Tile16shouldRenderFaceEP11LevelSourceiiii");
	mcpelauncher_hook(shouldRenderFace, &zhuoweixray_shouldRenderFaceXray, &zhuoweixray_Tile_shouldRenderFace_real);
	void* getBrightness = dlsym(RTLD_DEFAULT, "_ZN4Tile13getBrightnessEP11LevelSourceiii");
	mcpelauncher_hook(getBrightness, &zhuoweixray_getBrightnessXray, &zhuoweixray_Tile_getBrightness_real);

	soinfo2* mcpelibhandle = (soinfo2*) dlopen("libminecraftpe.so", RTLD_LAZY);
	__android_log_print(ANDROID_LOG_INFO, "zhuoweixray", "base%x\n", mcpelibhandle->base);
	void* swordUseFn = (void*) (mcpelibhandle->base + 0x1301be + 1);
	//mcpelauncher_hook(swordUseFn, &zhuoweixray_swordBreakBlock, &zhuoweixray_WeaponItem_mineBlock_real);
	//int* coalUseFn = (int*) (mcpelibhandle->base + 0x1c9ca0 + 14);
	
	//(*coalUseFn) = &zhuoweixray_coalItemUse;
	for (int i = 0; i < 0x100; i++) {
		zhuoweixray_renderInXrayMode[i] = 0;
	}
	zhuoweixray_renderInXrayMode[14] = zhuoweixray_renderInXrayMode[15] = zhuoweixray_renderInXrayMode[16] = 1;
	zhuoweixray_renderInXrayMode[54] = zhuoweixray_renderInXrayMode[56] = zhuoweixray_renderInXrayMode[73] = 1;
	zhuoweixray_renderInXrayMode[21] = 1;
	return JNI_VERSION_1_2;
}


