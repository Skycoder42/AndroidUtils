package de.skycoder42.androidutils;

import java.util.Map;
import java.util.HashMap;
import android.net.Uri;
import android.content.Context;
import android.content.Intent;
import android.content.ContentResolver;
import android.app.Activity;
import org.qtproject.qt5.android.QtNative;
import androidnative.SystemDispatcher;

public class FileChooser {
	private static final int CHOOSE_CONTENT_ACTION = 0x1091c657;
	private static final int CHOOSE_PERSISTENT_ACTION = 0x1091c658;

	private static final String GET_CONTENT_MESSAGE = "AndroidUtils.FileChooser.getContent";
	private static final String OPEN_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.openDocument";
	private static final String CREATE_DOCUMENT_MESSAGE = "AndroidUtils.FileChooser.createDocument";
	private static final String CONTENT_CHOOSEN_MESSAGE = "AndroidUtils.FileChooser.contentChoosen";

	static {
		SystemDispatcher.addListener(new SystemDispatcher.Listener() {
			public void onDispatched(String type , Map message) {
				if (type.equals(GET_CONTENT_MESSAGE)) {
					getContent((String)message.get("title"),
							   (String)message.get("mime"),
							   (Boolean)message.get("openable"),
							   (Boolean)message.get("localOnly"),
							   (Boolean)message.get("grantWrite"));
				} else if (type.equals(OPEN_DOCUMENT_MESSAGE)) {
					openDocument((String)message.get("title"),
								 (String)message.get("mime"),
								 (String)message.get("url"),
								 (Boolean)message.get("openable"),
								 (Boolean)message.get("grantWrite"),
								 (Boolean)message.get("persistPermissions"));
				} else if (type.equals(CREATE_DOCUMENT_MESSAGE)) {
					createDocument((String)message.get("title"),
								   (String)message.get("mime"),
								   (String)message.get("url"),
								   (String)message.get("name"),
								   (Boolean)message.get("openable"),
								   (Boolean)message.get("persistPermissions"));
				} else if (type.equals(SystemDispatcher.ACTIVITY_RESULT_MESSAGE)) {
					onActivityResult((Integer)message.get("requestCode"),
									 (Integer)message.get("resultCode"),
									 (Intent)message.get("data"));
				}
			}
		});
	}

	static private void getContent(final String title, String mime, boolean openable, boolean localOnly, boolean grantWrite) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
		intent.setType(mime);
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		if(grantWrite)
			intent.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
		if(openable)
			intent.addCategory(Intent.CATEGORY_OPENABLE);
		if(localOnly)
			intent.putExtra(Intent.EXTRA_LOCAL_ONLY, true);

		Runnable runnable = new Runnable () {
			public void run() {
				activity.startActivityForResult(
					Intent.createChooser(intent, title),
					CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void openDocument(final String title, String mime, String url, boolean openable, boolean grantWrite, final boolean persistPermissions) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
		intent.setType(mime);
		intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		if(grantWrite)
			intent.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
		if(openable)
			intent.addCategory(Intent.CATEGORY_OPENABLE);
		//TODO add in Android O
//		if(!url.isEmpty())
//			intent.putExtra(Intent.EXTRA_INITIAL_URI, Uri.parse(url));

		Runnable runnable = new Runnable () {
			public void run() {
				activity.startActivityForResult(
					Intent.createChooser(intent, title),
					persistPermissions ? CHOOSE_PERSISTENT_ACTION : CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void createDocument(final String title, String mime, String url, String name, boolean openable, final boolean persistPermissions) {
		final Activity activity = QtNative.activity();

		final Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
		intent.setType(mime);
		intent.addFlags(Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION);
		intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		intent.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
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
					persistPermissions ? CHOOSE_PERSISTENT_ACTION : CHOOSE_CONTENT_ACTION);
			};
		};
		activity.runOnUiThread(runnable);
	}

	static private void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(requestCode == CHOOSE_CONTENT_ACTION ||
		   requestCode == CHOOSE_PERSISTENT_ACTION) {
			Map reply = new HashMap();
			if (resultCode == Activity.RESULT_OK) {
				Uri uri = data.getData();

				if(requestCode == CHOOSE_PERSISTENT_ACTION) {
					int takeFlags = data.getFlags() & (Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
					QtNative.activity()
						.getContentResolver()
						.takePersistableUriPermission(uri, takeFlags);
				}

				reply.put("uri", uri.toString());
				reply.put("success", true);
			} else
				reply.put("success", false);
			SystemDispatcher.dispatch(CONTENT_CHOOSEN_MESSAGE, reply);
		}
	}
}
