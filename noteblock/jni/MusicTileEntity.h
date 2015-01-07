#pragma once
#include "mcpe.h"

class MusicTileEntity: public TileEntity {
public:
	static const TileEntityType type = (TileEntityType) 0x1001; // TODO auto gen this

	/** Note to play */
	char note;
	/** stores the latest redstone state */
	bool previousRedstoneState;

	MusicTileEntity(TilePos const& pos): TileEntity(MusicTileEntity::type, pos, "Music") {
	}
	virtual void write(CompoundTag* par1NBTTagCompound);
	virtual void read(CompoundTag* par1NBTTagCompound);
	void changePitch();
	void triggerNote(TileSource* world, int x, int y, int z);
};
