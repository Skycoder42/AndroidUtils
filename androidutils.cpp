#include "androidutils.h"
#include <QtGlobal>
#include <QQmlEngine>
#include <QCoreApplication>
#include <systemdispatcher.h>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#else
#include <QDebug>
#endif
#include "filechooser.h"

static void setupUtils();
static QObject *createQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

Q_COREAPP_STARTUP_FUNCTION(setupUtils)

AndroidUtils *AndroidUtils::_instance = nullptr;

void AndroidUtils::javaThrow()
{
#ifdef Q_OS_ANDROID
	QAndroidJniEnvironment env;
	if (env->ExceptionCheck()) {
		auto exception = QAndroidJniObject::fromLocalRef(env->ExceptionOccurred());
		JavaException exc;
		exc._what = exception.callObjectMethod("getLocalizedMessage", "()Ljava/lang/String;").toString().toUtf8();

		QAndroidJniObject stringWriter("java/io/StringWriter");
		QAndroidJniObject printWriter("java/io/PrintWriter", "(Ljava/lang/Writer;)V", stringWriter.object());
		exception.callMethod<void>("printStackTrace", "(Ljava/lang/PrintWriter;)V", printWriter.object());
		exc._stackTrace = stringWriter.callObjectMethod("toString", "()Ljava/lang/String;").toString().toUtf8();

		env->ExceptionClear();

		throw exc;
	}
#endif
}

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

	qmlRegisterSingletonType<AndroidUtils>("de.skycoder42.androidutils", 1, 1, "AndroidUtils", createQmlSingleton);
	qmlRegisterType<FileChooser>("de.skycoder42.androidutils", 1, 1, "FileChooser");
	//qmlProtectModule("de.skycoder42.quickextras", 1);
}

static QObject *createQmlSingleton(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
	Q_UNUSED(qmlEngine)
	Q_UNUSED(jsEngine)
	return new AndroidUtils(qmlEngine);
}



JavaException::JavaException() :
	_what(),
	_stackTrace()
{}

const char *JavaException::what() const noexcept
{
	return _what.constData();
}

const QByteArray JavaException::printStackTrace() const noexcept
{
	return _stackTrace;
}

void JavaException::raise() const
{
	throw *this;
}

QException *JavaException::clone() const
{
	auto e = new JavaException();
	e->_what = _what;
	e->_stackTrace = _stackTrace;
	return e;
}
