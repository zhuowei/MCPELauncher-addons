#pragma once

class MinecraftClient;
class Item;
class Player;
class Vec3;
class Level;

// from BlockLauncher's source.

class Vec3 {
	public:
		float x;
		float y;
		float z;
};

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
		char filler0[56]; //0
		ItemInstance currentItem; //56 (from ItemInHandRenderer::render, also in ItemInHandRenderer::ItemInHandRenderer)
		MinecraftClient* minecraft; //76 (from ItemInHandRenderer::ItemInHandRenderer)
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

class Mouse {
	public:
		int getX();
		int getY();
		static Mouse* _instance;
};

class GameMode {
	public:
		void* useItemOn(Player*, Level*, ItemInstance*, int, int, int, signed char, Vec3 const&);
};

class Matrix {
	public:
		char filler0[40]; // from Matrix::identity
		void translate(Vec3 const&);
};

class MatrixStack {
	public:
		char filler0[28]; // MatrixStack::World
		class Ref {
			public:
				MatrixStack* stack;
				Matrix* matrix; // see the calls to MatrixStack::push
				~Ref();
		};

		static MatrixStack World;
		static MatrixStack Projection;
		static MatrixStack View;
		Ref push();
};
