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
		LongPress,
		VirtualKey,
		KeyboardTap,
		ClockTick,
		ContextClick
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
