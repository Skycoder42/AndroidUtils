#include "androidutils.h"
#include <QtGlobal>

#ifdef Q_OS_ANDROID
#include <AndroidNative/systemdispatcher.h>
#include <QtAndroidExtras>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*) {
	Q_UNUSED(vm);

	// It must call this function within JNI_OnLoad to enable System Dispatcher
	AndroidNative::SystemDispatcher::registerNatives();

	return JNI_VERSION_1_6;
}
#endif

void AndroidUtils::setStatusBarColor(const QColor &color)
{
#ifdef Q_OS_ANDROID
	if(QtAndroid::androidSdkVersion() >= 21) {
		QtAndroid::runOnAndroidThreadSync([=](){
			auto activity = QtAndroid::androidActivity();
			if(activity.isValid()) {
				const auto FLAG_TRANSLUCENT_STATUS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																							 "FLAG_TRANSLUCENT_STATUS");
				const auto FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams",
																									   "FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS");
				const auto aColor = QAndroidJniObject::callStaticMethod<jint>("android/graphics/Color",
																			  "parseColor",
																			  "(Ljava/lang/String;)I",
																			  QAndroidJniObject::fromString(color.name()).object());

				QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
				if(window.isValid()) {
					window.callMethod<void>("clearFlags", "(I)V", FLAG_TRANSLUCENT_STATUS);
					window.callMethod<void>("addFlags", "(I)V", FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
					window.callMethod<void>("setStatusBarColor", "(I)V", aColor);
				}
			}
		});
	}
#else
	Q_UNUSED(color);
#endif
}

void AndroidUtils::showToast(const QString &message, bool showLong)
{
#ifdef Q_OS_ANDROID
	QtAndroid::runOnAndroidThread([=](){
		auto context = QtAndroid::androidContext();
		const auto LENGTH_LONG = QAndroidJniObject::getStaticField<jint>("android/widget/Toast", "LENGTH_LONG");
		const auto LENGTH_SHORT = QAndroidJniObject::getStaticField<jint>("android/widget/Toast", "LENGTH_SHORT");
		auto toast = QAndroidJniObject::callStaticObjectMethod("android/widget/Toast",
															   "makeText",
															   "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;",
															   context.object(),
															   QAndroidJniObject::fromString(message).object(),
															   showLong ? LENGTH_LONG : LENGTH_SHORT);
		toast.callMethod<void>("show");
	});
#else
	Q_UNUSED(showLong)
	qInfo() << message;
#endif
}

void AndroidUtils::hapticFeedback()
{
#ifdef Q_OS_ANDROID
	QtAndroid::runOnAndroidThread([=](){
		const auto android_R_id_content = QAndroidJniObject::getStaticField<jint>("android/R$id", "content");
		const auto LONG_PRESS = QAndroidJniObject::getStaticField<jint>("android/view/HapticFeedbackConstants", "LONG_PRESS");
		auto activity = QtAndroid::androidActivity();
		auto view = activity.callObjectMethod("findViewById",
											  "(I)Landroid/view/View;",
											  android_R_id_content);
		view.callMethod<jboolean>("performHapticFeedback",
								  "(I)Z",
								  LONG_PRESS);
	});
#endif
}
