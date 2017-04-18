package de.skycoder42.androidutils;

import androidnative.SystemDispatcher;
import android.os.Handler;
import android.app.Activity;
import android.view.View;
import android.content.Context;
import java.util.Map;
import org.qtproject.qt5.android.QtNative;

public class AndroidUtils {

	static {
		SystemDispatcher.addListener(new SystemDispatcher.Listener() {

			public void onDispatched(String name , Map data) {

				if (name.equals("AndroidUtils.hapticFeedback"))
					AndroidUtils.hapticFeedback((Integer)data.get("feedbackConstant"));
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
}
