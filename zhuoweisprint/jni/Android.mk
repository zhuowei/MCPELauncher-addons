LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS := -L$(LOCAL_PATH) -ldl -llog -lmcpelauncher
LOCAL_MODULE    := zhuoweisprint
LOCAL_SRC_FILES := nativepatch.c

include $(BUILD_SHARED_LIBRARY)
