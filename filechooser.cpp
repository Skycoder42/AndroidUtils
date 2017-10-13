#include "filechooser.h"
#include <systemdispatcher.h>

const QString FileChooser::ContentChoosenMessage = QStringLiteral("AndroidUtils.FileChooser.contentChoosen");

FileChooser::FileChooser(QObject *parent) :
	QObject(parent),
	_title(),
	_contentUrl(),
	_type(OpenDocument),
	_mimeType(QStringLiteral("*/*")),
	_flags(OpenableFlag)
{
	auto dispatcher = AndroidNative::SystemDispatcher::instance();
	dispatcher->loadClass(QStringLiteral("de.skycoder42.androidutils.FileChooser"));
	connect(dispatcher, &AndroidNative::SystemDispatcher::dispatched,
			this, &FileChooser::onDispatched,
			Qt::QueuedConnection);
}

QString FileChooser::title() const
{
	return _title;
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

FileChooser::ChooserFlags FileChooser::chooserFlags() const
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

void FileChooser::setChooserFlags(ChooserFlags chooserFlags)
{
	if (_flags == chooserFlags)
		return;

	_flags = chooserFlags;
	emit chooserFlagsChanged(chooserFlags);
}

void FileChooser::onDispatched(const QString &message, const QVariantMap &data)
{
	if(message == ContentChoosenMessage) {
		if(data.value(QStringLiteral("success"), false).toBool()) {
			_contentUrl = data.value(QStringLiteral("uri")).toString();
			emit contentUrlChanged(_contentUrl);
			emit accepted();
		} else
			emit rejected();
	}
}

void FileChooser::open()
{
	QString message;
	QVariantMap data;

	switch (_type) {
	case FileChooser::GetContent:
		message = QStringLiteral("AndroidUtils.FileChooser.getContent");
		data = {
			{"title", _title.isEmpty() ? tr("Open File") : _title},
			{"mime", _mimeType},
			{"openable", _flags.testFlag(OpenableFlag)},
			{"localOnly", _flags.testFlag(LocalOnlyFlag)},
			{"grantWrite", _flags.testFlag(AlwaysGrantWriteFlag)}
		};
		break;
	case FileChooser::OpenDocument:
		message = QStringLiteral("AndroidUtils.FileChooser.openDocument");
		data = {
			{"title", _title.isEmpty() ? tr("Open File") : _title},
			{"mime", _mimeType},
			{"url", _contentUrl.toString()},
			{"openable", _flags.testFlag(OpenableFlag)},
			{"grantWrite", _flags.testFlag(AlwaysGrantWriteFlag)},
			{"persistPermissions", _flags.testFlag(PersistPermissionsFlag)}
		};
		break;
	case FileChooser::CreateDocument:
		message = QStringLiteral("AndroidUtils.FileChooser.createDocument");
		data = {
			{"title", _title.isEmpty() ? tr("Save File") : _title},
			{"mime", _mimeType},
			{"url", _contentUrl.toString()},
			{"name", _contentUrl.fileName()},
			{"openable", _flags.testFlag(OpenableFlag)},
			{"persistPermissions", _flags.testFlag(PersistPermissionsFlag)}
		};
		break;
	default:
		Q_UNREACHABLE();
		break;
	}

	AndroidNative::SystemDispatcher::instance()->dispatch(message, data);
}

void FileChooser::setTitle(QString title)
{
	if (_title == title)
		return;

	_title = title;
	emit titleChanged(title);
}
