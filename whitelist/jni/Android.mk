# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := zhuowei_arrows # should match the name in the AndroidManifest.xml
LOCAL_SRC_FILES := main.cpp
# search for libraries in the jni/ folder of our project,
# link to the libraries for printing to logcat, getting methods from libminecraftpe,
# and for accessing BlockLauncher functions
LOCAL_LDLIBS    := -L$(LOCAL_PATH) -llog -ldl -lmcpelauncher -lminecraftpe

include $(BUILD_SHARED_LIBRARY)
