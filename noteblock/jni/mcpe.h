#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>

class TileSource;
class AABB;
class Random;
class Entity;
class Mob;
class Player;
class TilePos;
class Vec3;
class TextureUVCoordinateSet;
class ItemInstance;
class Minecraft;
class GuiMessage;
class CompoundTag;
class TileEntity;
class LevelChunk;
class Level;
class HitResult;
class TileTickingQueue;
class ChunkSource;
class Bounds;

enum TileEntityType {
};
enum LightLayer {
};

class Material {
public:
	static Material* water;
	static Material* air;
	static Material* leaves;
	static Material* vegetable;
	static Material* dirt;
	static Material* lava;
	static Material* replaceable_plant;
	static Material* wood;
	static Material* stone;
	static Material* sand;
	static Material* plant;
	static Material* cake;
	static Material* decoration;
	static Material* topSnow;
	static Material* cloth;
	static Material* glass;
	static Material* web;
	static Material* metal;
	static Material* ice;
	static Material* snow;
	static Material* clay;
	static Material* fire;
	static Material* sponge;
	static Material* explosive;
	static Material* coral;
	static Material* cactus;
	static Material* portal;
	static Material* bed;
};

class Tile {
public:
	// inner classes
	enum SoundType {
	};

	// fields
	char filler[0x8c];

	// constructors
	Tile(int, Material const*);
	Tile(int, TextureUVCoordinateSet, Material const*);
	Tile(int, std::string, Material const*);
	// virtual functions
	virtual ~Tile(); // 2
	virtual void onFertilized(TileSource*, int, int, int); // 4
	virtual void getShape(TileSource*, int, int, int, AABB&, bool); // 5
	virtual void getShape(unsigned char, AABB&, bool); // 6
	virtual bool isObstructingChests(TileSource*, int, int, int); // 7
	virtual void shouldRenderFace(TileSource*, int, int, int, signed char, AABB const&) const; // 8
	virtual void getTexture(signed char); // 9
	virtual void getTexture(signed char, int); // 10
	virtual void getTexture(TileSource*, int, int, int, signed char); // 11
	virtual void getTessellatedUVs(); // 12
	virtual void getCarriedTexture(signed char, int); // 13
	virtual void getAABB(TileSource*, int, int, int, AABB&, int, bool, int); // 14
	virtual void addAABBs(TileSource*, int, int, int, AABB const*, std::vector<AABB, std::allocator<AABB> >&); // 15
	virtual void getTileAABB(TileSource*, int, int, int, AABB&); // 16
	virtual bool isPathfindable(TileSource*, int, int, int); // 17
	virtual bool mayPick(); // 18
	virtual bool mayPick(int, bool); // 19
	virtual bool mayPlace(TileSource*, int, int, int, signed char); // 20
	virtual bool mayPlace(TileSource*, int, int, int); // 21
	virtual void getTickDelay(); // 22
	virtual void tick(TileSource*, int, int, int, Random*); // 23
	virtual void animateTick(TileSource*, int, int, int, Random*); // 24
	virtual void destroy(TileSource*, int, int, int, int); // 25
	virtual void playerWillDestroy(Player*, int, int, int, int); // 26
	virtual void neighborChanged(TileSource*, int, int, int, int, int, int); // 27
	virtual void onPlace(TileSource*, int, int, int); // 28
	virtual void onRemove(TileSource*, int, int, int); // 29
	virtual void getSecondPart(TileSource&, TilePos const&, TilePos&); // 30
	virtual void onGraphicsModeChanged(bool, bool); // 31
	virtual void getResource(int, Random*); // 32
	virtual void getResourceCount(Random*); // 33
	virtual void getDestroyProgress(Player*); // 34
	virtual void spawnResources(TileSource*, int, int, int, int, float); // 35
	virtual void spawnBurnResources(TileSource*, float, float, float); // 36
	virtual float getExplosionResistance(Entity*); // 37
	virtual void clip(TileSource*, int, int, int, Vec3 const&, Vec3 const&, bool, int); // 38
	virtual void wasExploded(TileSource*, int, int, int); // 39
	virtual void use(Player*, int, int, int); // 40
	virtual void stepOn(Entity*, int, int, int); // 41
	virtual void fallOn(TileSource*, int, int, int, Entity*, float); // 42
	virtual void getPlacementDataValue(Mob*, int, int, int, signed char, float, float, float, int); // 43
	virtual void prepareRender(TileSource*, int, int, int); // 44
	virtual void attack(Player*, int, int, int); // 45
	virtual void handleEntityInside(TileSource*, int, int, int, Entity*, Vec3&); // 46
	virtual int getColor(int); // 47
	virtual int getColor(TileSource*, int, int, int); // 48
	virtual bool isSignalSource(); // 49
	virtual int getSignal(TileSource*, int, int, int); // 50
	virtual int getSignal(TileSource*, int, int, int, int); // 51
	virtual int getDirectSignal(TileSource*, int, int, int, int); // 52
	virtual bool entityInside(TileSource*, int, int, int, Entity*); // 53
	virtual void playerDestroy(Player*, int, int, int, int); // 54
	virtual bool canSurvive(TileSource*, int, int, int); // 55
	virtual std::string getName() const; // 56
	virtual std::string getDescriptionId() const; // 57
	virtual std::string getDescriptionId(ItemInstance const*) const; // 58
	virtual std::string getTypeDescriptionId(int); // 59
	virtual void setDescriptionId(std::string const&); // 60
	virtual void triggerEvent(TileSource*, int, int, int, int, int); // 61
	virtual TextureUVCoordinateSet getTextureNum(int); // 62
	virtual void getMobToSpawn(TileSource&, TilePos const&) const; // 63
	virtual int getIconYOffset() const; // 64
	virtual void setShape(float, float, float, float, float, float); // 65
	virtual void setSoundType(Tile::SoundType const&); // 66
	virtual void setLightBlock(int); // 67
	virtual void setLightEmission(float); // 68
	virtual void setExplodeable(float); // 69
	virtual void setDestroyTime(float); // 70
	virtual void setTicking(bool); // 71
	virtual void getSpawnResourcesAuxValue(int); // 72
	virtual void init(); // 73

	// static fields
	static Tile* tiles[256];

	// static methods
	static void initTiles();
};

class Item {
public:
	char filler_item[76];
};

class TileItem : public Item {
public:
	char filler_tileitem[84 - 76];
	// constructor
	TileItem(int);
};

class Token {
public:
	enum Type {
	};

	char filler_token[16]; // trial and error. Sigh.

	Token(std::string const&);

	std::string const& getText(std::string const&) const;
	bool compatibleWith(Token::Type) const;
	int getValue(int) const;
	bool getBool(bool) const;
	void _parseRandom();

	static Token tokenize(std::string const&);
};

class ServerCommandParser {
public:
	// inner classes
	class Command {
	public:
		Command(std::function<std::string (std::vector<Token> const&)> const&, std::string const&);
		bool checkParameters(std::vector<Token> const&);
	};
	// constructor
	ServerCommandParser(Minecraft*);

	void executeCommand(GuiMessage const&);
	std::string _give(Minecraft*, std::string const&, Token const&, int, int);
	std::string _playerFlag(Minecraft*, std::string const&, std::string const&, bool);
	std::string result(std::string const&);
	void addCommand(std::string const&, std::string const&, std::function<std::string (std::vector<Token> const&)> const&);
};

class Minecraft {
public:
	Minecraft(int, char**);

	ServerCommandParser* getCommandParser();
};

class MinecraftClient : public Minecraft {
public:
	void init();
};

class TileID {
public:
	unsigned char id;
	char filler[7]; // Isn't in the struct; put here since otherwise MCPE hates me
};

class FullTile {
public:
	unsigned char id;
	unsigned char data;
	char filler[6]; // Isn't in the struct
};

class ChunkPos {
public:
	unsigned char x;
	unsigned char z;
	unsigned char y;
};

class TilePos {
public:
	int x;
	int y;
	int z;
};

class TileSource {
public:
	~TileSource();
	Level* getLevel() const;
	Level const& getLevelConst() const;
	TileTickingQueue* getTickQueue() const;
	void setTickingQueue(TileTickingQueue&);
	LevelChunk* getChunk(ChunkPos const&);
	LevelChunk* getChunk(int, int);
	LevelChunk* getWritableChunk(ChunkPos const&);
	bool _shouldFireEvent(LevelChunk&) const;
	bool hasChunksAt(Bounds const&);
	bool hasChunksAt(AABB const&);
	bool hasChunksAt(int, int, int, int);
	bool hasChunksAt(TilePos const&, int);
	bool hasChunksAt(TilePos const&, TilePos const&);
	TileID getTile(TilePos const&);
	Tile* getTilePtr(TilePos const&);
	unsigned char getData(TilePos const&);
	FullTile getTileAndData(TilePos const&);
	void getBrightnessPropagate(LightLayer const&, TilePos const&);
	void getLightColor(TilePos const&, int);
	void getBrightness(LightLayer const&, TilePos const&);
	bool canSeeSky(TilePos const&);
	Material* getMaterial(TilePos const&);
	void getSkyDarken();
	void getRawBrightness(TilePos const&, bool);
	void getBrightness(TilePos const&);
	void _neighborChanged(TilePos const&, TilePos const&, TileID);
	void updateNeighborsAt(TilePos const&, TileID);
	bool hasNeighborSignal(int, int, int);
	void runLightUpdates(LightLayer const&, TilePos const&, TilePos const&);
	void updateLightIfOtherThan(LightLayer const&, TilePos const&, unsigned char);
	bool isTopSolidBlocking(Tile*, unsigned char);
	bool isTopSolidBlocking(TilePos const&);
	void onChunkDiscarded(LevelChunk&);
	void fireAreaChanged(TilePos const&, TilePos const&);
	void fireTilesDirty(int, int, int, int, int, int);
	void fireTileChanged(TilePos const&, FullTile, FullTile, int);
	void _tileChanged(TilePos const&, FullTile, FullTile, int);
	void setTileAndData(TilePos const&, FullTile, int);
	void setTile(int, int, int, TileID, int);
	void setTileNoUpdate(int, int, int, TileID);
	void setTileAndDataNoUpdate(int, int, int, FullTile);
	void removeTile(int, int, int);
	void fireBrightnessChanged(TilePos const&);
	void setBrightness(LightLayer const&, TilePos const&, unsigned char);
	void fireTileEntityChanged(TileEntity&);
	void fireTileEntityRemoved(std::unique_ptr<TileEntity, std::default_delete<TileEntity> >&);
	void fireTileEvent(int, int, int, int, int);
	bool isEmptyTile(TilePos const&);
	bool isEmptyTile(int, int, int);
	LevelChunk* getChunkAt(TilePos const&);
	bool hasTile(TilePos const&);
	void getTopSolidBlock(TilePos const&, bool);
	void getHeightmap(TilePos const&);
	void getHeightmapPos(TilePos const&);
	void findHighestNonEmptyTileAt(TilePos const&);
	TileEntity* getTileEntity(TilePos const&);
	void tileEntityChanged(TileEntity&);
	LevelChunk* getChunkAt(int, int, int);
	void getBiome(TilePos const&);
	void setGrassColor(int, TilePos const&, int);
	void getGrassColor(TilePos const&);
	void findNextTopSolidTileUnder(TilePos&);
	void getTopRainTilePos(TilePos const&);
	bool isWaterAt(TilePos const&);
	void shouldFreeze(TilePos const&, bool);
	void shouldFreezeIgnoreNeighbors(TilePos const&);
	void shouldSnow(TilePos const&, bool);
	bool isHumidAt(TilePos const&);
	bool hasTile(int, int, int);
	void getTile(int, int, int);
	bool isSolidBlockingTile(int, int, int);
	bool isSolidBlockingTile(TilePos const&);
	bool isSolidRenderTile(int, int, int);
	bool isSolidRenderTile(TilePos const&);
	void containsAnyLiquid(AABB const&);
	void containsMaterial(AABB const&, Material const*);
	void containsFireTile(AABB const&);
	void tileEvent(int, int, int, int, int);
	void tileEvent(TilePos const&, int, int);
	unsigned char getData(int, int, int);
	void containsLiquid(AABB const&, Material const*);
	void getTileAndData(int, int, int);
	void clip(HitResult&, Vec3 const&, Vec3 const&, bool, bool);
	void getSeenPercent(Vec3 const&, AABB const&);
	void getBrightness(LightLayer const&, int, int, int);
	void getBrightness(int, int, int);
	void getRawBrightness(int, int, int, bool);
	void getTopSolidBlock(int, int, bool);
	bool canSeeSky(int, int, int);
	void getHeightmap(int, int);
	void setTileAndData(int, int, int, FullTile, int);
	void setBrightness(LightLayer const&, int, int, int, unsigned char);
	TileEntity* getTileEntity(int, int, int);
	Tile* getTilePtr(int, int, int);
	Material* getMaterial(int, int, int);
	bool isTopSolidBlocking(int, int, int);
	void getCubes(AABB const&, float*, bool);
	void getEntities(int, AABB const&, Entity*);
	void getNearestEntityOfType(Entity*, float, float, float, float, int);
	void getEntities(Entity*, AABB const&);
	bool isUnobstructedByEntities(AABB const&, Entity*);
	bool mayPlace(TileID, TilePos const&, signed char, Mob*, bool, Entity*);
	TileSource(Level&, ChunkSource*, bool, bool);
	TileSource(Player&);
};

class Level {
public:
	char filler1[12]; // 0: note that this doesn't account for vtable yet
	bool isRemote; // 12, from LeafTile::tick
	void playSound(float, float, float, std::string const&, float, float);
};

class Entity {
public:
	char filler1[60-4]; // 4
	TileSource* tileSource; // 60
	char filler2[4]; // 64
	Level* level; // 68

	virtual ~Entity();
};

class Player : public Entity {
};
