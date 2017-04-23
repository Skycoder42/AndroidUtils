#include <QFile>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "opener.h"
#ifdef Q_OS_ANDROID
#include <contentdevice.h>
#include <sharedpreferences.h>
#endif

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	qmlRegisterType<Opener>("de.skycoder42.androidutils", 1, 0, "Opener");

#ifdef Q_OS_ANDROID
	QUrl url("file:///sdcard/temp/baum");
	ContentDevice device(url);

	if(device.open(QIODevice::WriteOnly)) {
		device.write("baum == 42");
		device.close();
	} else
		qCritical() << "ContentDevice:" << device.errorString();

	if(device.open(QIODevice::ReadOnly)) {
		qDebug() << "ContentDevice:" << device.readAll();
		device.close();
	} else
		qCritical() << "ContentDevice:" << device.errorString();

	auto prefs = SharedPreferences::getPreferences();
	QObject::connect(prefs, &SharedPreferences::loaded, prefs, [prefs](){
		qDebug() << "init state" << prefs->data();
		prefs->setValue("test1", "test2");
		prefs->setValue("test2", 42);
		prefs->remove("test2");
	});
#endif

	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
