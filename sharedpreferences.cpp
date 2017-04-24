#include "sharedpreferences.h"

#include <QUuid>
#include <AndroidNative/systemdispatcher.h>

SharedPreferences::SharedPreferences(const QString &id, QObject *parent) :
	QObject(parent),
	_id(id),
	_data()
{
	AndroidNative::SystemDispatcher::instance()->loadClass(QStringLiteral("de.skycoder42.androidutils.PrefHelper"));

	connect(AndroidNative::SystemDispatcher::instance(), &AndroidNative::SystemDispatcher::dispatched,
			this, &SharedPreferences::dispatched,
			Qt::QueuedConnection);
}

SharedPreferences::~SharedPreferences()
{
	AndroidNative::SystemDispatcher::instance()->dispatch(QStringLiteral("AndroidUtils.PrefHelper.remPrefs"), {
															  {QStringLiteral("id"), _id}
														  });
}

SharedPreferences *SharedPreferences::getPreferences(QObject *parent)
{
	auto id = QUuid::createUuid().toString();
	auto prefs = new SharedPreferences(id, parent);
	AndroidNative::SystemDispatcher::instance()->dispatch(QStringLiteral("AndroidUtils.PrefHelper.getPrefs"), {
															  {QStringLiteral("id"), id}
														  });
	return prefs;
}

SharedPreferences *SharedPreferences::getSharedPreferences(const QString &name, QObject *parent)
{
	auto id = QUuid::createUuid().toString();
	auto prefs = new SharedPreferences(id, parent);
	AndroidNative::SystemDispatcher::instance()->dispatch(QStringLiteral("AndroidUtils.PrefHelper.getSharedPrefs"), {
															  {QStringLiteral("id"), id},
															  {QStringLiteral("name"), name}
														  });
	return prefs;
}

QStringList SharedPreferences::keys() const
{
	return _data.keys();
}

bool SharedPreferences::contains(const QString &key) const
{
	return _data.contains(key);
}

QVariant SharedPreferences::value(const QString &key) const
{
	return _data.value(key);
}

QVariantMap SharedPreferences::data() const
{
	return _data;
}

void SharedPreferences::setValue(const QString &key, const QVariant &value)
{
	_data.insert(key, value);
	AndroidNative::SystemDispatcher::instance()->dispatch(QStringLiteral("AndroidUtils.PrefHelper.save"), {
															  {QStringLiteral("id"), _id},
															  {QStringLiteral("key"), key},
															  {QStringLiteral("value"), value}
														  });
}

void SharedPreferences::remove(const QString &key)
{
	_data.remove(key);
	AndroidNative::SystemDispatcher::instance()->dispatch(QStringLiteral("AndroidUtils.PrefHelper.remove"), {
															  {QStringLiteral("id"), _id},
															  {QStringLiteral("key"), key}
														  });
}

void SharedPreferences::dispatched(const QString &message, const QVariantMap &data)
{
	if(message == QStringLiteral("AndroidUtils.PrefHelper.changed.") + _id) {
		auto key = data.value(QStringLiteral("key")).toString();
		auto removed = data.value(QStringLiteral("removed"), false).toBool();
		auto value = data.value(QStringLiteral("value"));
		if(removed) {
			if(_data.remove(key) > 0)
				emit changed(key, QVariant());
		} else {
			_data.insert(key, value);
			emit changed(key, value);
		}
	} else if(message == QStringLiteral("AndroidUtils.PrefHelper.loaded.") + _id) {
		_data = data;
		emit loaded();
	}
}
