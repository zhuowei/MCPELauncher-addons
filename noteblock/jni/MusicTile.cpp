#include "MusicTile.h"
#include <android/log.h>
#include <cmath>

#include "SoundPool.h"

using namespace android_media_SoundPool;
#if 0
/**
 * Lets the block know when one of its neighbor changes. Doesn't know which neighbor changed (coordinates passed are
 * their own) Args: x, y, z, neighbor blockID
 */
void MusicTile::neighborChanged(TileSource* par1World, int x, int y, int z, int par5, int par6, int par7)
{
	bool var6 = Redstone::hasNeighborSignal(par1World, x, y, z);
	MusicTileEntity var7 = (MusicTileEntity)par1World.getBlockTileEntity(par2, par3, par4);

	if (var7 != null && var7.previousRedstoneState != var6)
	{
		if (var6)
		{
			var7.triggerNote(par1World, par2, par3, par4);
		}

		var7.previousRedstoneState = var6;
	}
}

/**
 * Called upon block activation (right click on the block.)
 */
void MusicTile::use(Player* player, int x, int y, int z)
{
	if (player->level->isRemote)
	{
		return;
	}
	else
	{
		MusicTileEntity* var10 = (MusicTileEntity*)player->tileSource->getTileEntity(x, y, z);

		if (var10 != nullptr)
		{
			var10.changePitch();
			var10.triggerNote(player->tileSource, x, y, z);
		}
	}
}

/**
 * Called when the block is clicked by a player. Args: x, y, z, entityPlayer
 */
void MusicTile::attack(Player* player, int x, int y, int z)
{
	if (!player->level->isRemote)
	{
		MusicTileEntity* var6 = (MusicTileEntity*) player->tileSource->getTileEntity(TilePos {x, y, z});

		if (var6 != nullptr)
		{
			var6->triggerNote(player->tileSource, x, y, z);
		}
	}
}

#endif

static int wtf = 0;

void MusicTile::use(Player* player, int x, int y, int z)
{
	if (player->level->isRemote)
	{
		return;
	}
	else
	{
		player->tileSource->tileEvent(TilePos {x, y, z}, 0, wtf++);
	}
}

/**
 * Called when the block receives a BlockEvent - see World.addBlockEvent. By default, passes it on to the tile
 * entity at this location. Args: world, x, y, z, blockID, EventID, event parameter
 */
void MusicTile::triggerEvent(TileSource* tileSource, int x, int y, int z, int par5, int par6)
{
	float var7 = (float)std::pow(2.0, (double)(par6 - 12) / 12.0);
	/*std::string var8 = "harp";

	if (par5 == 1)
	{
		var8 = "bd";
	}

	if (par5 == 2)
	{
		var8 = "snare";
	}

	if (par5 == 3)
	{
		var8 = "hat";
	}

	if (par5 == 4)
	{
		var8 = "bassattack";
	}*/

	//tileSource->getLevel()->playSound(x + 0.5f, y + 0.5f, z + 0.5f, "note." + var8, 3.0F, var7);
	//tileSource->getLevel()->playSound(x + 0.5f, y + 0.5f, z + 0.5f, "random.click", 3.0F, var7);
	musicTileSoundPool->play(musicTileSoundInstrument[par5 % 5], 1.0, 1.0, 0, 0, var7);
	//par1World.spawnParticle("note", (double)par2 + 0.5D, (double)par3 + 1.2D, (double)par4 + 0.5D, (double)par6 / 24.0D, 0.0D, 0.0D);
}

SoundPool* MusicTile::musicTileSoundPool;
int MusicTile::musicTileSoundInstrument[5];

const std::string soundPath = "/sdcard/noteblock/";
void MusicTile::initSoundPool() {
	musicTileSoundPool = new SoundPool(5, 3, 0);
	musicTileSoundInstrument[0] = musicTileSoundPool->load(soundPath + "harp.ogg", 1);
	musicTileSoundInstrument[1] = musicTileSoundPool->load(soundPath + "bd.ogg", 1);
	musicTileSoundInstrument[2] = musicTileSoundPool->load(soundPath + "snare.ogg", 1);
	musicTileSoundInstrument[3] = musicTileSoundPool->load(soundPath + "hat.ogg", 1);
	musicTileSoundInstrument[4] = musicTileSoundPool->load(soundPath + "bassattack.ogg", 1);
}
