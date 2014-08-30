#include <string>
#include <unordered_set>
#include <fstream>

// headers - contain declarations of methods we need
#include <stdint.h>
// for Java Native Interface stuff
#include <jni.h>
// for printing out messages to the system log
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
// for accessing methods exported from BlockLauncher
// we'll copy this now
#include "mcpelauncher.h"

#define TAG "Whitelist"
// found in LocalPlayer::displayClientMessage, also before the first call to Gui constructor
#define MINECRAFT_GUI_OFFSET 416

class LoginPacket;
namespace RakNet {
	class RakNetGUID {
		private:
			uint64_t guid;
	};
	class SystemAddress {
		private:
			char filler[4*4 + 4];
		public:
			const char* ToString(bool, char) const;
	};
	class RakPeer {
		public:
			SystemAddress GetSystemAddressFromGuid(RakNetGUID) const;
	};
};

class RakNetInstance {
	public:
		RakNet::RakPeer* getPeer();
};

typedef void Minecraft;

class ServerSideNetworkHandler {
	// bla bla stuff
	public:
		char filler[8]; //0
		Minecraft* minecraft; //8
		char filler2[4]; //12
		RakNetInstance* rakNetInstance; //16
		void handle(RakNet::RakNetGUID const&, LoginPacket*);
};

class Gui {
	public:
		void displayClientMessage(std::string const&);
};

static void showMessage(Minecraft* minecraft, std::string const& message) {
	Gui* mygui = (Gui*) (((uintptr_t) minecraft) + MINECRAFT_GUI_OFFSET);
	mygui->displayClientMessage(message);
}

static std::unordered_set<std::string> whitelistSet;

static bool isAllowed(const char* ip) {
	std::string ipStr(ip);
	return whitelistSet.count(ip) != 0;
}

#define WHITELIST_PATH "/sdcard/mcpe_whitelist.txt"

static void loadWhitelist() {
	std::ifstream stream(WHITELIST_PATH);
	std::string str;
	while (stream.good()) {
		str.clear();
		std::getline(stream, str);
		if (str.length() == 0) continue;
		whitelistSet.insert(str);
	}
	stream.close();
	
}

static void saveWhitelist() {
	// todo
}

static void (*ServerSideNetworkHandler_handle_real)(ServerSideNetworkHandler*, RakNet::RakNetGUID const&, LoginPacket*);
static void ServerSideNetworkHandler_handle_hook(ServerSideNetworkHandler* handler, RakNet::RakNetGUID const& guid, LoginPacket* packet) {
	RakNet::RakPeer* peer = handler->rakNetInstance->getPeer();
	RakNet::SystemAddress addr = peer->GetSystemAddressFromGuid(guid);
	const char* addressStr = addr.ToString(false, ':');
	__android_log_print(ANDROID_LOG_INFO, TAG, "Server side handler: hook %s", addressStr);
	bool allowed = isAllowed(addressStr);
	std::string message(allowed? "Allowing ": "Blocking ");
	message.append(addressStr);
	showMessage(handler->minecraft, message);
	if (!allowed) return;
	ServerSideNetworkHandler_handle_real(handler, guid, packet);
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	mcpelauncher_hook((void*) &ServerSideNetworkHandler::handle, (void*) &ServerSideNetworkHandler_handle_hook,
		(void**) &ServerSideNetworkHandler_handle_real);
	loadWhitelist();
	return JNI_VERSION_1_2;
}

