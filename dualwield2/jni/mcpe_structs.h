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

class MinecraftClient {
public:
	Player* getPlayer();
};

class ItemInstance {
	public:
		char filler[20];
		ItemInstance(ItemInstance const&);
		ItemInstance* cloneSafe(ItemInstance const*);
};

/* from Objdump we get
001dd9b0 g    DF .text  00000794 ItemInHandRenderer::render(float)
so
*/

class ItemInHandRenderer {
	public:
		char filler0[52]; //0
		ItemInstance currentItem; //52 (from ItemInHandRenderer::render, also in ItemInHandRenderer::ItemInHandRenderer)
		MinecraftClient* minecraft; //72 (from ItemInHandRenderer::ItemInHandRenderer)
		void render(float);
};

class FillingContainer {
	public:
		ItemInstance* getLinked(int) const; // from Inventory::getSelected
};

class Inventory : public FillingContainer {
public:
	int getSelectedSlot() const;
	void selectSlot(int);
};

class Matrix {
	public:
		char filler0[40]; // from Matrix::IDENTITY
		void translate(Vec3 const&);
};

class MatrixStack {
	public:
		char filler0[10]; // MatrixStack::World
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

class TouchTurnInteractControl {
public:
	enum State {
		NONE = 0,
		PRESS = 1,
		MOVE = 2,
		BREAK = 3,
		BUILD = 4,
	};
	char filler[80];
	float ptrX;
	float ptrY;
	void switchState(State);
};
