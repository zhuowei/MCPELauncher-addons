#pragma once
#include "mcpe.h"
namespace android_media_SoundPool {
	class SoundPool;
};
class MusicTile : public Tile {
public:
	MusicTile(int id, std::string textureName, Material const* material) : Tile(id, textureName, material) {
	}
	virtual void use(Player* player, int x, int y, int z);
	void triggerEvent(TileSource* par1World, int x, int y, int z, int par5, int par6);

	static android_media_SoundPool::SoundPool* musicTileSoundPool;
	static int musicTileSoundInstrument[5];
	static void initSoundPool();
};
