#include <jni.h>
#include <android/log.h>
//#include <GLES/gl.h>
#include "Substrate.h"
#include "mcpe_structs.h"

// from Player::getCarriedItem
#define PLAYER_INVENTORY_OFFSET 3456

// Used to store the location of the actual ItemInHandRenderer::render method that we overrode
// so that we can call the original method in our hook.
static void (*ItemInHandRenderer_render_real)(ItemInHandRenderer*, float);
static void (*TouchTurnInteractControl_switchState_real)(TouchTurnInteractControl* self, TouchTurnInteractControl::State state);

// note that this method has one extra parameter (the renderer one)
// in C++, class instance methods (methods that are called on an object)
// have an implicit parameter: the "this" parameter, for identifying the current object.
// we write hooks using C-style methods, so we have to manually add this
// parameter to our method.


// if left hand active, original slot is the original right hand slot
// else -1
static int originalSlot = -1;
static MinecraftClient* mcinstance;
static const int kLeftHandSlot = 1;

static void renderWithLeftHandActive(ItemInHandRenderer* renderer, float partialTicks) {
	// call the actual Minecraft method first to render the right hand
	uintptr_t playerPtr = (uintptr_t) mcinstance->getPlayer();
	Inventory* inventory = *((Inventory**) (playerPtr + PLAYER_INVENTORY_OFFSET));
	ItemInstance backup(renderer->currentItem);

	// change the active slot to the item held in slot 1
	renderer->currentItem.cloneSafe(inventory->getLinked(originalSlot));

	// and call the actual ItemInHandRenderer method in MCPE again
	ItemInHandRenderer_render_real(renderer, partialTicks);

	// restore active slot
	renderer->currentItem.cloneSafe(&backup);

	// store the current camera position
	MatrixStack::Ref matref = MatrixStack::World.push();
	// move the camera 1 units to the left
	Vec3 oneleft {-1.0f, 0.0f, 0.0f};
	matref.matrix->translate(oneleft);
	ItemInHandRenderer_render_real(renderer, partialTicks);
}

static void ItemInHandRenderer_render_hook(ItemInHandRenderer* renderer, float partialTicks) {
	mcinstance = renderer->minecraft;
	if (originalSlot != -1) {
		renderWithLeftHandActive(renderer, partialTicks);
		return;
	}
	// call the actual Minecraft method first to render the right hand
	ItemInHandRenderer_render_real(renderer, partialTicks);
	// store the current camera position
	MatrixStack::Ref matref = MatrixStack::World.push();
	// move the camera 1 units to the left
	Vec3 oneleft {-1.0f, 0.0f, 0.0f};
	matref.matrix->translate(oneleft);
	uintptr_t playerPtr = (uintptr_t) mcinstance->getPlayer();
	Inventory* inventory = *((Inventory**) (playerPtr + PLAYER_INVENTORY_OFFSET));
	ItemInstance backup(renderer->currentItem);	

	// change the active slot to the item held in slot 1
	renderer->currentItem.cloneSafe(inventory->getLinked(kLeftHandSlot));

	// and call the actual ItemInHandRenderer method in MCPE again
	ItemInHandRenderer_render_real(renderer, partialTicks);

	// restore active slot
	renderer->currentItem.cloneSafe(&backup);

	// restore 
	// MatrixStack::View.pop();
	// the MatrixStack::Ref auto pops
}

static void TouchTurnInteractControl_switchState_hook(TouchTurnInteractControl* self, TouchTurnInteractControl::State state) {
	int screenWidth = ((AppPlatform_android*) AppPlatform::_singleton)->getScreenWidth();
	//__android_log_print(ANDROID_LOG_INFO, "DualWield", "switch state %d %f %f %d", state, self->ptrX, self->ptrY, screenWidth);
	TouchTurnInteractControl_switchState_real(self, state);
	if (!mcinstance) return;
	bool isLeft;
	switch (state) {
		case TouchTurnInteractControl::BUILD:
		case TouchTurnInteractControl::BREAK:
			isLeft = self->ptrX < (screenWidth / 2);
			break;
		default:
			isLeft = false;
	}

	uintptr_t playerPtr = (uintptr_t) mcinstance->getPlayer();
	if (!playerPtr) return;
	Inventory* inventory = *((Inventory**) (playerPtr + PLAYER_INVENTORY_OFFSET));

	if (!isLeft) {
		if (originalSlot != -1) inventory->selectSlot(originalSlot);
		originalSlot = -1;
	} else {
		originalSlot = inventory->getSelectedSlot();
		inventory->selectSlot(kLeftHandSlot);
	}

};

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	// use Substrate to hook the ItemInHandRenderer method.
	// MSHookFunction will modify ItemInHandRenderer::render so that
	// whenever someone tries to call the render method,
	// it will go to our hook method, above, instead.
	MSHookFunction((void*) &ItemInHandRenderer::render, (void*) &ItemInHandRenderer_render_hook, (void**)&ItemInHandRenderer_render_real);
	MSHookFunction((void*) &TouchTurnInteractControl::switchState, (void*) &TouchTurnInteractControl_switchState_hook,
		(void**) &TouchTurnInteractControl_switchState_real);
	return JNI_VERSION_1_2;
}

