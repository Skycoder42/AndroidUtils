#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include <QObject>
#include <QUrl>

class FileChooser : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

	Q_PROPERTY(QUrl contentUrl READ contentUrl WRITE setContentUrl NOTIFY contentUrlChanged)
	Q_PROPERTY(ChooserType type READ type WRITE setType NOTIFY typeChanged)
	Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
	Q_PROPERTY(ChooserFlags flags READ flags WRITE setFlags NOTIFY flagsChanged)

public:
	enum ChooserType {
		GetContent = 0,
		OpenDocument = 1,
		CreateDocument = 2
	};
	Q_ENUM(ChooserType)

	enum ChooserFlag {
		OpenableFlag = 0x01,
		LocalOnlyFlag = 0x02,
		AlwaysGrantWriteFlag = 0x04,
		PersistPermissionsFlag = 0x08
	};
	Q_DECLARE_FLAGS(ChooserFlags, ChooserFlag)
	Q_FLAG(ChooserFlags)

	explicit FileChooser(QObject *parent = nullptr);

	QString title() const;
	QUrl contentUrl() const;
	ChooserType type() const;
	QString mimeType() const;
	ChooserFlags flags() const;

public slots:
	void open();

	void setTitle(QString title);
	void setContentUrl(QUrl contentUrl);
	void setType(ChooserType type);
	void setMimeType(QString mimeType);
	void setFlags(ChooserFlags flags);

signals:
	void accepted();
	void rejected();

	void titleChanged(QString title);
	void contentUrlChanged(QUrl contentUrl);
	void typeChanged(ChooserType type);
	void mimeTypeChanged(QString mimeType);
	void flagsChanged(ChooserFlags flags);

private slots:
	void onDispatched(const QString &message, const QVariantMap &data);

private:
	static const QString ContentChoosenMessage;

	QString _title;
	QUrl _contentUrl;
	ChooserType _type;
	QString _mimeType;
	ChooserFlags _flags;
};

#endif // FILECHOOSER_H
