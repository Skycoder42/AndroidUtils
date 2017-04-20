package de.skycoder42.androidutils;

import java.util.Map;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.app.Activity;
import org.qtproject.qt5.android.QtNative;
import androidnative.SystemDispatcher;

public class FileChooser {
	public static final int GET_CONTENT_ACTION = 0x1091c657;
	public static final int OPEN_DOCUMENT_ACTION = 0x83895290;
	public static final int CREATE_DOCUMENT_ACTION = 0x7e97d25e;

	public static final String GET_CONTENT_MESSAGE = "AndroidUtils.FileChooser.getContent";
	public static final String OPEN_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.openDocument";
	public static final String CREATE_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.createDocument";

	static {
		SystemDispatcher.addListener(new SystemDispatcher.Listener() {
			public void onDispatched(String type , Map message) {
				if (type.equals(GET_CONTENT_MESSAGE)) {
					getContent((String)message.get("title"),
							   (String)message.get("mime"),
							   (Boolean)message.get("openable"),
							   (Boolean)message.get("localOnly"),
							   (Boolean)message.get("allowMultiple"));
				} else if (type.equals(OPEN_DOCUMENT_MESSAGE)) {
					openDocument((String)message.get("title"),
								 (String)message.get("mime"),
								 (String)message.get("url"),
								 (Boolean)message.get("openable"),
								 (Boolean)message.get("allowMultiple"));
				} else if (type.equals(CREATE_DOCUMENT_MESSAGE)) {
					createDocument((String)message.get("title"),
								   (String)message.get("mime"),
								   (String)message.get("url"),
								   (String)message.get("name"),
								   (Boolean)message.get("openable"));
				} else if (type.equals(SystemDispatcher.ACTIVITY_RESULT_MESSAGE)) {
					onActivityResult(message);
				}
			}
		});
	}

	static private void getContent(final String title, String mime, boolean openable, boolean localOnly, boolean allowMultiple) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
		intent.setType(mime);
		if(openable)
			intent.addCategory(Intent.CATEGORY_OPENABLE);
		if(localOnly)
			intent.putExtra(Intent.EXTRA_LOCAL_ONLY, true);
		if(allowMultiple)
			intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true);

		Runnable runnable = new Runnable () {
			public void run() {
				activity.startActivityForResult(
					Intent.createChooser(intent, title),
					GET_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void openDocument(final String title, String mime, String url, boolean openable, boolean allowMultiple) {

	}

	static private void createDocument(final String title, String mime, String url, String name, boolean openable) {

	}

	static private void onActivityResult(Map message) {
		Log.d("AndroidUtils", message.toString());
	}
}
