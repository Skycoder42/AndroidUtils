#include <QFile>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "opener.h"
#include "prefmodel.h"
#ifdef Q_OS_ANDROID
#include <contentdevice.h>
#include <sharedpreferences.h>
#endif

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	qmlRegisterType<Opener>("de.skycoder42.androidutils.demo", 1, 0, "Opener");
	qmlRegisterType<PrefModel>("de.skycoder42.androidutils.demo", 1, 0, "PrefModel");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
