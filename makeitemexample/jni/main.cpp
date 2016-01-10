#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include "Substrate.h"
#include <string>
#include <unordered_map>
#include <memory>

enum CreativeItemCategory {
};

class Item {
public:
	char filler[64 - 4];
	Item(std::string const&, short);
	virtual ~Item();
	void setIcon(std::string const&, int);
	void setCategory(CreativeItemCategory);
	static std::unordered_map<std::string, std::pair<std::string, std::unique_ptr<Item>>> mItemLookupMap;
	static void initItems();
	static void initCreativeItems();
	static void addCreativeItem(short, short);
	static Item* mItems[4096];
};

static void (*Item_initItems_real)();
static void Item_initItems_hook() {
	Item_initItems_real();
	Item* orangePeel = new Item("orangePeel", 3777 - 0x100);
	orangePeel->setIcon("apple", 0);
	orangePeel->setCategory((CreativeItemCategory) 3 /* TOOL */);
	Item::mItems[3777] = orangePeel;
	// Supposed to do the below, but didn't work, so omitting it
	//std::string lowercaseStr = "orangepeel";
	//Item::mItemLookupMap[lowercaseStr] = std::make_pair(lowercaseStr, std::unique_ptr<Item>(orangePeel));
}

static void (*Item_initCreativeItems_real)();
static void Item_initCreativeItems_hook() {
	Item_initCreativeItems_real();
	Item::addCreativeItem(3777, 0);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	MSHookFunction((void*) &Item::initItems, (void*) &Item_initItems_hook, (void**) &Item_initItems_real);
	MSHookFunction((void*) &Item::initCreativeItems, (void*) &Item_initCreativeItems_hook, (void**) &Item_initCreativeItems_real);
	return JNI_VERSION_1_2;
}

