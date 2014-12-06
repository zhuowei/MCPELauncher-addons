#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include "Substrate.h"
#include "mcpe.h"

// thanks to @atomictissue for figuring out how commands work.
// ModPE scripting support must be DISABLED for this to work.

static std::string exampleCommand(std::vector<Token> const& args) {
	__android_log_print(ANDROID_LOG_INFO, "BlockLauncher", "Args count: %d", args.size());
	return "This is an example command " + args[1].getText("");
}

static void (*MinecraftClient_init_real)(MinecraftClient*);
static void MinecraftClient_init_hook(MinecraftClient* minecraft) {
	MinecraftClient_init_real(minecraft);
	// add the command
	ServerCommandParser* commandParser = minecraft->getCommandParser();
	// first argument: command name
	// second argument: how many params. e.g. "s" = one parameter, "ss" =  two parameters
	// s stands for string; I guess there might be others?
	commandParser->addCommand("example", "s", exampleCommand);
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	MSHookFunction((void*) &MinecraftClient::init, (void*) &MinecraftClient_init_hook, (void**) &MinecraftClient_init_real);
	return JNI_VERSION_1_2;
}

