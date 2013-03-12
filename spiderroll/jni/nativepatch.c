#include <dlfcn.h>
#include <stdio.h>
#include <android/log.h>
#include <jni.h>

void launchRickroll() {
	//runs the command line AM program to view the desired page. Yes, a bit too unix admin-ish, but who cares?
	system("am start -a android.intent.action.VIEW -d http://www.youtube.com/watch?v=wL-hNMJvcyI");
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//this gets called after the library's happily loaded and the Dalvik VM is ready
	__android_log_print(ANDROID_LOG_INFO, "nativepatch", "starting\n");
	void* zombieVtable = dlsym(RTLD_DEFAULT, "_ZTV6Spider"); //vtable for Spider
	if (zombieVtable == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "nativepatch", "NO EFFING VTABLE WAAAAAAAA\n");
		return;
	}
	__android_log_print(ANDROID_LOG_INFO, "nativepatch", "got the print\n");
	__android_log_print(ANDROID_LOG_INFO, "nativepatch", "launch rickroll address is %p\n", &launchRickroll);

	int *zombieDropLootFn = (int*) (((int) zombieVtable) + 312);
	(*zombieDropLootFn) = (int) &launchRickroll;
	__android_log_print(ANDROID_LOG_INFO, "nativepatch", "done\n");
	return JNI_VERSION_1_2;
}


