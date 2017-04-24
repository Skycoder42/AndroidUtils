#ifndef PREFMODEL_H
#define PREFMODEL_H

#include <QObject>
#include <sharedpreferences.h>

class PrefModel : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QStringList model READ model NOTIFY modelChanged)

public:
	explicit PrefModel(QObject *parent = nullptr);

	QStringList model() const;

public slots:
	void save(const QString &key, const QVariant &value);
	void remove(int index);

signals:
	void modelChanged();

private:
	SharedPreferences *prefs;
};

#endif // PREFMODEL_H
