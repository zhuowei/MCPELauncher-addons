#if 0
#include "mcpe.h"
#include "MusicTileEntity.h"

/**
 * Writes a tile entity to NBT.
 */
void MusicTileEntity::write(CompoundTag* par1NBTTagCompound)
{
	TileEntity::write(par1NBTTagCompound);
	par1NBTTagCompound->putByte("note", this->note);
}

/**
 * Reads a tile entity from NBT.
 */
void MusicTileEntity::read(CompoundTag* par1NBTTagCompound)
{
	TileEntity::read(par1NBTTagCompound);
	this->note = par1NBTTagCompound->getByte("note");

	if (this->note < 0)
	{
		this->note = 0;
	}

	if (this->note > 24)
	{
		this->note = 24;
	}
}

/**
 * change pitch by -> (currentPitch + 1) % 25
 */
void MusicTileEntity::changePitch()
{
	this->note = (char)((this->note + 1) % 25);
	this->setChanged();
}

/**
 * plays the stored note
 */
void MusicTileEntity::triggerNote(TileSource* par1World, int x, int y, int z)
{
	if (par1World->getMaterial(x, y + 1, z) == Material::air)
	{
		Material* var5 = par1World->getMaterial(x, y - 1, z);
		char instrument = 0;

		if (var5 == Material::stone)
		{
			instrument = 1;
		}

		if (var5 == Material::sand)
		{
			instrument = 2;
		}

		if (var5 == Material::glass)
		{
			instrument = 3;
		}

		if (var5 == Material::wood)
		{
			instrument = 4;
		}

		par1World->tileEvent(TilePos {x, y, z}, instrument, this->note);
	}
}
#endif
