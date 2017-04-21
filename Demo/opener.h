#ifndef OPENER_H
#define OPENER_H

#include <QObject>
#include <QUrl>

class Opener : public QObject
{
	Q_OBJECT
public:
	explicit Opener(QObject *parent = nullptr);

public slots:
	void openFile(const QUrl &file);
	void saveFile(const QUrl &file, const QString &data);

signals:
	void dataLoaded(const QString &data);
};

#endif // OPENER_H
