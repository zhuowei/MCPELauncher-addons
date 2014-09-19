#pragma once

class Minecraft;
class Item;

// from BlockLauncher's source.
class ItemInstance {
	public:
		int count; //0
		int damage; //4
		Item* item;//8
		void* block; //12
		bool wtf; //16
		bool wtf2; //17
		bool wtf3; //18
		bool wtf4; //19
		ItemInstance(ItemInstance const&);
		ItemInstance* cloneSafe(ItemInstance const*);
};

/* from Objdump we get
001dd9b0 g    DF .text  00000794 ItemInHandRenderer::render(float)
so
*/

class ItemInHandRenderer {
	public:
		char filler0[8]; //0
		ItemInstance currentItem; //8 (from ItemInHandRenderer::tick)
		Minecraft* minecraft; //28 (from first line of ItemInHandRenderer::tick)
		void render(float);
};

class FillingContainer {
	public:
		ItemInstance* getLinked(int); // from Inventory::getSelected
};

class Inventory : public FillingContainer {
	public:
		char filler0[40]; //0
		int selected; //40 (from Inventory::selectSlot)
};
