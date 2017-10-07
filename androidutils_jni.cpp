#if defined(Q_OS_ANDROID)
#include <AndroidNative/systemdispatcher.h>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
	Q_UNUSED(vm);
	// It must call this function within JNI_OnLoad to enable System Dispatcher
	AndroidNative::SystemDispatcher::registerNatives();
	return JNI_VERSION_1_6;
}
#endif
