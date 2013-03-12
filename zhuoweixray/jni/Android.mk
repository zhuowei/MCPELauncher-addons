LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)  
LOCAL_LDLIBS := -ldl -llog -lmcpelauncher -lminecraftpe
LOCAL_MODULE    := zhuoweixray
LOCAL_SRC_FILES := nativepatch.c 

include $(BUILD_SHARED_LIBRARY)  
