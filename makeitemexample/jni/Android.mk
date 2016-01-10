LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := net.zhuoweizhang.mcpelauncher.addon.makeitemexample # should match the name in the AndroidManifest.xml
LOCAL_SRC_FILES := main.cpp
# search for libraries in the jni/ folder of our project,
# link to the libraries for printing to logcat, getting methods from libminecraftpe,
# and for accessing BlockLauncher functions
LOCAL_LDLIBS    := -L$(LOCAL_PATH)/$(TARGET_ARCH_ABI) -llog -ldl -lminecraftpe -lmcpelauncher_tinysubstrate

include $(BUILD_SHARED_LIBRARY)
