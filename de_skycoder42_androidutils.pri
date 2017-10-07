QT += qml quick

HEADERS += \
	$$PWD/androidutils.h \
	$$PWD/filechooser.h \
    $$PWD/sharedpreferences.h

SOURCES += \
	$$PWD/androidutils.cpp \
	$$PWD/filechooser.cpp \
    $$PWD/sharedpreferences.cpp

RESOURCES += \
	$$PWD/de_skycoder42_androidutils.qrc

INCLUDEPATH += $$PWD

TRANSLATIONS += $$PWD/androidutils_de.ts \
	$$PWD/androidutils_template.ts

android {
	HEADERS += $$PWD/contentdevice.h
	SOURCES += $$PWD/contentdevice.cpp
}

!qpmx_static: include($$PWD/de_skycoder42_androidutils.prc)
else:android: QT *= androidextras
