#include "filechooser.h"
#include <AndroidNative/systemdispatcher.h>

FileChooser::FileChooser(QObject *parent) :
	QObject(parent),
	_contentUrl(),
	_type(GetContent),
	_mimeType(QStringLiteral("*/*")),
	_flags(OpenableFlag)
{
	auto dispatcher = AndroidNative::SystemDispatcher::instance();
	dispatcher->loadClass(QStringLiteral("de.skycoder42.androidutils.FileChooser"));
}

QUrl FileChooser::contentUrl() const
{
	return _contentUrl;
}

FileChooser::ChooserType FileChooser::type() const
{
	return _type;
}

QString FileChooser::mimeType() const
{
	return _mimeType;
}

FileChooser::ChooserFlags FileChooser::flags() const
{
	return _flags;
}

void FileChooser::setContentUrl(QUrl contentUrl)
{
	if (_contentUrl == contentUrl)
		return;

	_contentUrl = contentUrl;
	emit contentUrlChanged(contentUrl);
}

void FileChooser::setType(FileChooser::ChooserType type)
{
	if (_type == type)
		return;

	_type = type;
	emit typeChanged(type);
}

void FileChooser::setMimeType(QString mimeType)
{
	if (_mimeType == mimeType)
		return;

	_mimeType = mimeType;
	emit mimeTypeChanged(mimeType);
}

void FileChooser::setFlags(ChooserFlags flags)
{
	if (_flags == flags)
		return;

	_flags = flags;
	emit flagsChanged(flags);
}

void FileChooser::open()
{
	QString message;
	QVariantMap data;

	switch (_type) {
	case FileChooser::GetContent:
		message = QStringLiteral("AndroidUtils.FileChooser.getContent");
		data = {
			{"mime", _mimeType},
			{"openable", _flags.testFlag(OpenableFlag)},
			{"localOnly", _flags.testFlag(LocalOnlyFlag)},
			{"allowMultiple", _flags.testFlag(AllowMultipleFlag)}
		};
		break;
	case FileChooser::OpenDocument:
		message = QStringLiteral("AndroidUtils.FileChooser.openDocument");
		data = {
			{"mime", _mimeType},
			{"url", _contentUrl.toString()},
			{"openable", _flags.testFlag(OpenableFlag)},
			{"allowMultiple", _flags.testFlag(AllowMultipleFlag)}
		};
		break;
	case FileChooser::CreateDocument:
		message = QStringLiteral("AndroidUtils.FileChooser.createDocument");
		data = {
			{"mime", _mimeType},
			{"url", _contentUrl.toString()},
			{"title", _contentUrl.fileName()},
			{"openable", _flags.testFlag(OpenableFlag)}
		};
		break;
	default:
		Q_UNREACHABLE();
		break;
	}

	AndroidNative::SystemDispatcher::instance()->dispatch(message, data);
}
