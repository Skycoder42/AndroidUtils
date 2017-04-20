#include "opener.h"
#include <QFile>
#ifdef Q_OS_ANDROID
#include <contentdevice.h>
#include <QtAndroid>
#endif

Opener::Opener(QObject *parent) :
	QObject(parent)
{}

void Opener::openFile(const QUrl &file)
{
#ifdef Q_OS_ANDROID
	ContentDevice device(QtAndroid::androidActivity(), file);
#else
	QFile device(file.toLocalFile());
#endif
	device.open(QIODevice::ReadOnly);
	auto data = QString::fromUtf8(device.readAll());
	device.close();
	emit dataLoaded(data);
}

void Opener::saveFile(const QUrl &file, const QString &data)
{
#ifdef Q_OS_ANDROID
	ContentDevice device(QtAndroid::androidActivity(), file);
#else
	QFile device(file.toLocalFile());
#endif
	device.open(QIODevice::WriteOnly);
	device.write(data.toUtf8());
	device.close();
}
