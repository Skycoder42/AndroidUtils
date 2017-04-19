android: QT *= androidextras

HEADERS += \
	$$PWD/androidutils.h

SOURCES += \
	$$PWD/androidutils.cpp

RESOURCES += \
	$$PWD/de_skycoder42_androidutils.qrc

INCLUDEPATH += $$PWD

DISTFILES += \
	$$PWD/android/androidutils.gradle \
	$$PWD/android/src/de/skycoder42/androidutils/AlarmReceiver.java \
	$$PWD/android/src/de/skycoder42/androidutils/AndroidUtils.java

android {
	isEmpty(QPM_ROOT): QPM_ROOT = $$_PRO_FILE_PWD_/vendor
	isEmpty(BUILD_ROOT): BUILD_ROOT = $$OUT_PWD

	copygradle.commands = mkdir -p $$BUILD_ROOT/android-build && $(COPY) $$PWD/android/androidutils.gradle $$BUILD_ROOT/android-build
	setupnative.commands = echo 'addAndroidUtilsPath\\(\\\"$$QPM_ROOT/android/native/pri/java/src/\\\"\\)' >> $$BUILD_ROOT/android-build/androidutils.gradle
	setupgradle.commands = echo 'addAndroidUtilsPath\\(\\\"$$PWD/android/src\\\"\\)' >> $$BUILD_ROOT/android-build/androidutils.gradle

	setupnative.depends = copygradle
	setupgradle.depends = setupnative
	first.depends = $(first) setupgradle
	export(first.depends)
	QMAKE_EXTRA_TARGETS += first copygradle setupnative setupgradle
}

noJniOnLoad: DEFINES += NO_JNI_ONLOAD
