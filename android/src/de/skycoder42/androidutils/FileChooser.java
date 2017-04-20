package de.skycoder42.androidutils;

import java.util.Map;
import java.util.HashMap;
import android.util.Log;
import android.net.Uri;
import android.content.Context;
import android.content.Intent;
import android.app.Activity;
import org.qtproject.qt5.android.QtNative;
import androidnative.SystemDispatcher;

public class FileChooser {
	public static final int CHOOSE_CONTENT_ACTION = 0x1091c657;

	public static final String GET_CONTENT_MESSAGE = "AndroidUtils.FileChooser.getContent";
	public static final String OPEN_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.openDocument";
	public static final String CREATE_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.createDocument";
	public static final String CONTENT_CHOOSEN_MESSAGE = "AndroidUtils.FileChooser.contentChoosen";

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
					onActivityResult((Integer)message.get("requestCode"),
									 (Integer)message.get("resultCode"),
									 (Intent)message.get("data"));
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
					CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void openDocument(final String title, String mime, String url, boolean openable, boolean allowMultiple) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.setType(mime);
		if(openable)
			intent.addCategory(Intent.CATEGORY_OPENABLE);
		//TODO add in Android O
//		if(!url.isEmpty())
//			intent.putExtra(Intent.EXTRA_INITIAL_URI, Uri.parse(url));
		if(allowMultiple)
			intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true);

		Runnable runnable = new Runnable () {
			public void run() {
				activity.startActivityForResult(
					Intent.createChooser(intent, title),
					CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void createDocument(final String title, String mime, String url, String name, boolean openable) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
		intent.setType(mime);
		if(openable)
			intent.addCategory(Intent.CATEGORY_OPENABLE);
		//TODO add in Android O
//		if(!url.isEmpty())
//			intent.putExtra(Intent.EXTRA_INITIAL_URI, Uri.parse(url));
		if(!name.isEmpty())
			intent.putExtra(Intent.EXTRA_TITLE, name);

		Runnable runnable = new Runnable () {
			public void run() {
				activity.startActivityForResult(
					Intent.createChooser(intent, title),
					CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(requestCode == CHOOSE_CONTENT_ACTION) {
			Map reply = new HashMap();
			if (resultCode == Activity.RESULT_OK) {
				Uri uri = data.getData();
				reply.put("uri", uri.toString());
				reply.put("success", true);
			} else
				reply.put("success", false);
			SystemDispatcher.dispatch(CONTENT_CHOOSEN_MESSAGE, reply);
		}
	}
}
