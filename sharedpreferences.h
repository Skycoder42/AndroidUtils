#ifndef SHAREDPREFERENCES_H
#define SHAREDPREFERENCES_H

#include <QObject>
#include <QString>
#include <QVariant>

class SharedPreferences : public QObject
{
	Q_OBJECT

public:
	~SharedPreferences();

	static SharedPreferences *getPreferences(QObject *parent = nullptr);
	static SharedPreferences *getSharedPreferences(const QString &name, QObject *parent = nullptr);

	Q_INVOKABLE QStringList keys() const;
	Q_INVOKABLE bool contains(const QString &key) const;
	Q_INVOKABLE QVariant value(const QString &key) const;

	Q_INVOKABLE QVariantMap data() const;

public slots:
	void setValue(const QString &key, const QVariant &value);
	void remove(const QString &key);

signals:
	void loaded();
	void changed(const QString &key, const QVariant &value);

private slots:
	void dispatched(const QString &message, const QVariantMap &data);

private:
	const QString _id;
	QVariantMap _data;

	explicit SharedPreferences(const QString &id, QObject *parent = nullptr);
};

#endif // SHAREDPREFERENCES_H
