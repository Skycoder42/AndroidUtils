#ifndef ANDROIDUTILS_H
#define ANDROIDUTILS_H

#include <QObject>
#include <QString>
#include <QColor>

class AndroidUtils : public QObject
{
	Q_OBJECT

public:
	enum HapticFeedbackConstant {
		LongPress = 0,
		VirtualKey = 1,
		KeyboardTap = 3,
		ClockTick = 4,
		ContextClick = 6
	};
	Q_ENUM(HapticFeedbackConstant)

	AndroidUtils(QObject *parent = nullptr);
	static AndroidUtils *instance();

public slots:
	void setStatusBarColor(const QColor &color);
	void showToast(const QString &message, bool showLong = false);
	void hapticFeedback(HapticFeedbackConstant constant= LongPress);

private:
	static AndroidUtils *_instance;
};

#endif // ANDROIDUTILS_H
