DISTFILES += \
	$$PWD/android/androidutils.gradle \
	$$PWD/android/src/de/skycoder42/androidutils/AlarmReceiver.java \
	$$PWD/android/src/de/skycoder42/androidutils/AndroidUtils.java \
	$$PWD/android/src/de/skycoder42/androidutils/FileChooser.java \
    $$PWD/android/src/de/skycoder42/androidutils/PrefHelper.java

android {
	QT *= androidextras
	
	!noJniOnLoad: SOURCES += androidutils_jni.cpp
	
	#isEmpty(ANDROID_NATIVE_PATH): ANDROID_NATIVE_PATH = $$_PRO_FILE_PWD_/vendor/android/native/pri
	isEmpty(ANDROID_NATIVE_PATH): ANDROID_NATIVE_PATH = $$PWD/../../android.native.pri/0.0.5/ #assume qpmx install, both as qpm package

	copygradle.path = /
	copygradle.files = $$PWD/android/androidutils.gradle

	setupnative.path = /
	setupnative.files =
	setupnative.commands = echo 'addAndroidUtilsPath\\(\\\"$$ANDROID_NATIVE_PATH/java/src/\\\"\\)' >> $(INSTALL_ROOT)/androidutils.gradle
	setupnative.depends = install_copygradle

	setupgradle.path = /
	setupgradle.files =
	setupgradle.commands = echo 'addAndroidUtilsPath\\(\\\"$$PWD/android/src\\\"\\)' >> $(INSTALL_ROOT)/androidutils.gradle
	setupgradle.depends = install_copygradle

	INSTALLS += copygradle setupnative setupgradle
}
