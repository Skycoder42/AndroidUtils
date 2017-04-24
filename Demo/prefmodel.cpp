#include "prefmodel.h"
#include <QDebug>

PrefModel::PrefModel(QObject *parent) :
	QObject(parent),
	prefs(SharedPreferences::getPreferences(this))
{
	connect(prefs, &SharedPreferences::loaded,
			this, &PrefModel::modelChanged);
	connect(prefs, &SharedPreferences::changed,
			this, &PrefModel::modelChanged);
}

QStringList PrefModel::model() const
{
	auto data = prefs->data();
	QStringList model;
	for(auto it = data.constBegin(); it != data.constEnd(); it++)
		model.append(it.key() + ": " + it.value().toString());
	return model;
}

void PrefModel::save(const QString &key, const QVariant &value)
{
	prefs->setValue(key, value);
}

void PrefModel::remove(int index)
{
	prefs->remove(prefs->keys().at(index));
}
