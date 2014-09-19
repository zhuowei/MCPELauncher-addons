#include <jni.h>
#include <GLES/gl.h>
#include "Substrate.h"
#include "mcpe_structs.h"

// from Minecraft::selectLevel
#define MINECRAFT_LOCAL_PLAYER_OFFSET 3192
// from Player::getCarriedItem
#define PLAYER_INVENTORY_OFFSET 3212

// Used to store the location of the actual ItemInHandRenderer::render method that we overrode
// so that we can call the original method in our hook.
static void (*ItemInHandRenderer_render_real)(ItemInHandRenderer*, float);

// note that this method has one extra parameter (the renderer one)
// in C++, class instance methods (methods that are called on an object)
// have an implicit parameter: the "this" parameter, for identifying the current object.
// we write hooks using C-style methods, so we have to manually add this
// parameter to our method.

static void ItemInHandRenderer_render_hook(ItemInHandRenderer* renderer, float partialTicks) {
	// call the actual Minecraft method first to render the right hand
	ItemInHandRenderer_render_real(renderer, partialTicks);
	// store the current camera position
	glPushMatrix();
	// move the camera 1 units to the left
	glTranslatef(-1, 0, 0);
	uintptr_t playerPtr = *((uintptr_t*) ((uintptr_t) renderer->minecraft + MINECRAFT_LOCAL_PLAYER_OFFSET));
	Inventory* inventory = *((Inventory**) (playerPtr + PLAYER_INVENTORY_OFFSET));
	
	// save the current active slot
	ItemInstance backup(renderer->currentItem);
	// change the active slot to the item held in slot 1
	renderer->currentItem.cloneSafe(inventory->getLinked(1));

	// and call the actual ItemInHandRenderer method in MCPE again
	ItemInHandRenderer_render_real(renderer, partialTicks);

	// restore active slot
	renderer->currentItem.cloneSafe(&backup);

	// restore 
	glPopMatrix();
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	// use Substrate to hook the ItemInHandRenderer method.
	// MSHookFunction will modify ItemInHandRenderer::render so that
	// whenever someone tries to call the render method,
	// it will go to our hook method, above, instead.
	MSHookFunction((void*) &ItemInHandRenderer::render, (void*) &ItemInHandRenderer_render_hook, (void**)&ItemInHandRenderer_render_real);
	return JNI_VERSION_1_2;
}

