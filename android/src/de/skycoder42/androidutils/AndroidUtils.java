package de.skycoder42.androidutils;

import java.util.Map;
import android.os.Build;
import android.os.Handler;
import android.content.Context;
import android.app.Activity;
import android.graphics.Color;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import org.qtproject.qt5.android.QtNative;
import androidnative.SystemDispatcher;

public class AndroidUtils {

	static {
		SystemDispatcher.addListener(new SystemDispatcher.Listener() {

			public void onDispatched(String name , Map data) {

				if (name.equals("AndroidUtils.hapticFeedback"))
					AndroidUtils.hapticFeedback((Integer)data.get("feedbackConstant"));
				else if (name.equals("AndroidUtils.setStatusBarColor"))
					AndroidUtils.setStatusBarColor((String)data.get("color"));
			}
		});
	}

	static void hapticFeedback(final int feedbackConstant) {
		final Activity activity = QtNative.activity();

		Runnable runnable = new Runnable () {
			public void run() {
				View rootView = activity.getWindow().getDecorView().getRootView();
				rootView.performHapticFeedback(feedbackConstant);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static void setStatusBarColor(String colorName) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
			final int color = Color.parseColor(colorName);
			final Activity activity = QtNative.activity();

			Runnable runnable = new Runnable () {
				public void run() {
					Window window = activity.getWindow();
					window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
					window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
					window.setStatusBarColor(color);
				};
			};
			activity.runOnUiThread(runnable);
		}
	}
}
