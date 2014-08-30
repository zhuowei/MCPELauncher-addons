#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <algorithm>

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

#define CHATSCREEN_TEXTBOX_TEXT_OFFSET 132

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

class ChatScreen {
	public:
		char filler[20]; //0
		Minecraft* minecraft; //20
		void sendChatMessage();
};

/* from http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c */

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

/* end */

static void showMessage(Minecraft* minecraft, std::string const& message) {
	Gui* mygui = (Gui*) (((uintptr_t) minecraft) + MINECRAFT_GUI_OFFSET);
	mygui->displayClientMessage(message);
}

static std::unordered_set<std::string> whitelistSet;
static std::vector<std::string> whitelistVector;

static bool isAllowed(const char* ip) {
	std::string ipStr(ip);
	return whitelistSet.count(ip) != 0;
}

#define WHITELIST_PATH "/sdcard/mcpe_whitelist.txt"

static void loadWhitelist() {
	std::ifstream stream(WHITELIST_PATH);
	std::string str;
	whitelistVector.clear();
	whitelistSet.clear();
	while (stream.good()) {
		str.clear();
		std::getline(stream, str);
		if (str.length() == 0) continue;
		whitelistVector.push_back(str);
		whitelistSet.insert(str);
	}
	stream.close();
}

static void saveWhitelist() {
	std::ofstream stream(WHITELIST_PATH);
	for (int i = 0; i < whitelistVector.size(); i++) {
		stream << whitelistVector[i] << '\n';
	}
	stream.close();
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

static void addToWhitelist(std::string ip) {
	if (whitelistSet.count(ip) > 0) return;
	whitelistVector.push_back(ip);
	whitelistSet.insert(ip);
	saveWhitelist();
}

static void removeFromWhitelist(std::string ip) {
	if (whitelistSet.count(ip) == 0) return;
	whitelistSet.erase(ip);
	whitelistVector.erase(std::remove(whitelistVector.begin(), whitelistVector.end(), ip), whitelistVector.end());
	saveWhitelist();
}

static void (*ChatScreen_sendChatMessage_real)(ChatScreen*);
static void ChatScreen_sendChatMessage_hook(ChatScreen* chatScreen) {
	std::string* chatMessagePtr = (std::string*) ((uintptr_t) chatScreen + CHATSCREEN_TEXTBOX_TEXT_OFFSET);
	std::string msg = *chatMessagePtr;
	if (msg.length() < 1 || msg[0] != '/') {
		ChatScreen_sendChatMessage_real(chatScreen);
		return;
	}
	std::vector<std::string> parts = split(msg, ' ');
	if (parts[0] != "/whitelist") {
		ChatScreen_sendChatMessage_real(chatScreen);
		return;
	}
	if (parts.size() == 3) {
		if (parts[1] == "add") {
			addToWhitelist(parts[2]);
			showMessage(chatScreen->minecraft, std::string("Added."));
		} else if (parts[1] == "remove") {
			removeFromWhitelist(parts[2]);
			showMessage(chatScreen->minecraft, std::string("Removed."));
		}
	} else {
		showMessage(chatScreen->minecraft, std::string("Usage: /whitelist <add|remove> <ip>"));
	}
}

// this lib is loaded from Java code, and the JVM will call our
// jni_onload method.

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	mcpelauncher_hook((void*) &ServerSideNetworkHandler::handle, (void*) &ServerSideNetworkHandler_handle_hook,
		(void**) &ServerSideNetworkHandler_handle_real);
	void* sendChatMessage = dlsym(RTLD_DEFAULT, "_ZN10ChatScreen15sendChatMessageEv");
	mcpelauncher_hook(sendChatMessage, (void*) &ChatScreen_sendChatMessage_hook,
		(void**) &ChatScreen_sendChatMessage_real);
	loadWhitelist();
	return JNI_VERSION_1_2;
}

