#include "androidutils.h"
#include <QtGlobal>
#include <QQmlEngine>
#include <QCoreApplication>
#include <AndroidNative/systemdispatcher.h>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#else
#include <QDebug>
#endif

#if defined(Q_OS_ANDROID) && !defined(NO_JNI_ONLOAD)
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
	Q_UNUSED(vm);
	// It must call this function within JNI_OnLoad to enable System Dispatcher
	AndroidNative::SystemDispatcher::registerNatives();
	return JNI_VERSION_1_6;
}
#endif

static void setupUtils();
static QObject *createQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

Q_COREAPP_STARTUP_FUNCTION(setupUtils)

AndroidUtils *AndroidUtils::_instance = nullptr;

AndroidUtils::AndroidUtils(QObject *parent) :
	QObject(parent)
{}

AndroidUtils *AndroidUtils::instance()
{
	if(!_instance)
		_instance = new AndroidUtils(qApp);
	return _instance;
}

void AndroidUtils::setStatusBarColor(const QColor &color)
{
#ifdef Q_OS_ANDROID
	AndroidNative::SystemDispatcher::instance()->dispatch("AndroidUtils.setStatusBarColor", {
															 {"color", color.name()}
														 });
#else
	Q_UNUSED(color);
#endif
}

void AndroidUtils::showToast(const QString &message, bool showLong)
{
#ifdef Q_OS_ANDROID
	AndroidNative::SystemDispatcher::instance()->dispatch("androidnative.Toast.showToast", {
															  {"text", message},
															  {"longLength", showLong}
														  });
#else
	Q_UNUSED(showLong)
	qInfo() << message;
#endif
}

void AndroidUtils::hapticFeedback(HapticFeedbackConstant constant)
{
#ifdef Q_OS_ANDROID
		AndroidNative::SystemDispatcher::instance()->dispatch("AndroidUtils.hapticFeedback", {
																 {"feedbackConstant", (int)constant}
															 });
#else
	Q_UNUSED(constant);
#endif
}

static void setupUtils()
{
	AndroidNative::SystemDispatcher::instance()->loadClass("androidnative.Toast");
	AndroidNative::SystemDispatcher::instance()->loadClass("de.skycoder42.androidutils.AndroidUtils");

	qmlRegisterSingletonType<AndroidUtils>("de.skycoder42.androidutils", 1, 0, "AndroidUtils", createQmlSingleton);
	//qmlProtectModule("de.skycoder42.quickextras", 1);
}

static QObject *createQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
	Q_UNUSED(qmlEngine)
	Q_UNUSED(jsEngine)
	return new AndroidUtils(qmlEngine);
}
