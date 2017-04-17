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
	$$PWD/android/src/de/skycoder42/androidutils/AlarmReceiver.java

copygradle.commands = $(COPY) $$PWD/android/androidutils.gradle $$OUT_PWD/android-build
setupgradle.commands = echo 'setAndroidUtilsPath\\(\\\"$$PWD/android/src\\\"\\)' >> $$OUT_PWD/android-build/androidutils.gradle

setupgradle.depends = copygradle
first.depends = $(first) setupgradle
export(first.depends)
QMAKE_EXTRA_TARGETS += first copygradle setupgradle
