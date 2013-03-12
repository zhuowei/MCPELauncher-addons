LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)  
LOCAL_LDLIBS := -ldl -llog
LOCAL_MODULE    := spiderroll
LOCAL_SRC_FILES := nativepatch.c 

include $(BUILD_SHARED_LIBRARY)  
