#include "androidutils.h"
#include "contentdevice.h"
#include <QAndroidJniEnvironment>
#include <QtAndroid>

ContentDevice::ContentDevice(QObject *parent) :
	QIODevice(parent),
	_context(QtAndroid::androidContext()),
	_url()
{
	Q_ASSERT(_context.isValid());
}

ContentDevice::ContentDevice(const QUrl &url, QObject *parent) :
	QIODevice(parent),
	_context(QtAndroid::androidContext()),
	_url(url)
{
	Q_ASSERT(_context.isValid());
}

ContentDevice::ContentDevice(const QAndroidJniObject &context, const QUrl &url, QObject *parent) :
	QIODevice(parent),
	_context(context),
	_url(url)
{
	Q_ASSERT(_context.isValid());
}

void ContentDevice::setContext(const QAndroidJniObject &context)
{
	_context = context;
	Q_ASSERT(_context.isValid());
}

bool ContentDevice::isSequential() const
{
	return true;
}

bool ContentDevice::open(QIODevice::OpenMode mode)
{
	if(!QIODevice::open(mode))
		return false;

	QAndroidJniEnvironment env;
	try {
		auto uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",
															 "parse",
															 "(Ljava/lang/String;)Landroid/net/Uri;",
															 QAndroidJniObject::fromString(_url.toString(QUrl::FullyEncoded)).object());
		if(!uri.isValid()) {
			AndroidUtils::javaThrow();
			return false;
		}

		auto contentResolver = _context.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
		if(!contentResolver.isValid()) {
			AndroidUtils::javaThrow();
			return false;
		}

		switch(mode) {
		case QIODevice::ReadOnly:
			_stream = contentResolver.callObjectMethod("openInputStream",
													   "(Landroid/net/Uri;)Ljava/io/InputStream;",
													   uri.object());
			break;
		case QIODevice::WriteOnly:
			_stream = contentResolver.callObjectMethod("openOutputStream",
													   "(Landroid/net/Uri;)Ljava/io/OutputStream;",
													   uri.object());
			break;
		default:
			setErrorString(tr("You can only open ContentDevice with QIODevice::ReadOnly OR QIODevice::WriteOnly. Other flags are not supported"));
			QIODevice::close();
			return false;
		}

		if(!_stream.isValid()) {
			AndroidUtils::javaThrow();
			return false;
		}

		return true;
	} catch(QException &e) {
		QIODevice::close();
		setErrorString(e.what());
		return false;
	}
}

void ContentDevice::close()
{
	if(_stream.isValid()) {
		_stream.callMethod<void>("close");
		_stream = QAndroidJniObject();
	}
	QIODevice::close();
}

qint64 ContentDevice::bytesAvailable() const
{
	if(openMode().testFlag(QIODevice::ReadOnly) && _stream.isValid())
		return (qint64)_stream.callMethod<jint>("available");

	return -1;
}

void ContentDevice::flush()
{
	if(openMode().testFlag(QIODevice::WriteOnly) && _stream.isValid())
		_stream.callMethod<void>("flush");
}

QUrl ContentDevice::url() const
{
	return _url;
}

void ContentDevice::setUrl(QUrl url)
{
	if (_url == url)
		return;

	_url = url;
	emit urlChanged(url);
}

qint64 ContentDevice::readData(char *data, qint64 maxlen)
{
	QAndroidJniEnvironment env;
	auto array = env->NewByteArray((jsize)maxlen);
	try {
		auto cnt = _stream.callMethod<jint>("read", "([B)I", array);
		AndroidUtils::javaThrow();
		if(cnt > 0)
			env->GetByteArrayRegion(array, 0, cnt, reinterpret_cast<jbyte*>(data));
		env->DeleteLocalRef(array);
		return (qint64)cnt;
	} catch(QException &e) {
		setErrorString(e.what());
		env->DeleteLocalRef(array);
		return -1;
	}
}

qint64 ContentDevice::writeData(const char *data, qint64 len)
{
	QAndroidJniEnvironment env;
	auto array = env->NewByteArray((jsize)len);
	env->SetByteArrayRegion (array, 0, len, reinterpret_cast<const jbyte*>(data));
	try {
		_stream.callMethod<void>("write", "([B)V", array);
		AndroidUtils::javaThrow();
		env->DeleteLocalRef(array);
		return len;
	} catch(QException &e) {
		setErrorString(e.what());
		env->DeleteLocalRef(array);
		return -1;
	}
}
