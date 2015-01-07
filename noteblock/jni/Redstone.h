#include "mcpe.h"
class Redstone {
public:
	// MCP: isBlockProvidingPowerTo
	static int getDirectSignal(TileSource* tileSource, int x, int y, int z, int side);
	// MCP: isBlockGettingPowered
	static bool hasDirectSignal(TileSource* tileSource, int x, int y, int z);
	// MCP: getIndirectPowerLevelTo
	static int getSignal(TileSource* tileSource, int x, int y, int z);
	// MCP: isBlockIndirectlyGettingPowered
	static bool hasNeighborSignal(TileSource* tileSource, int x, int y, int z);
}

