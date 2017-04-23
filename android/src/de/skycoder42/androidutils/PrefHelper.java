package de.skycoder42.androidutils;

import java.util.Map;
import java.util.AbstractMap;
import java.util.HashMap;
import android.content.Context;
import android.content.SharedPreferences;
import org.qtproject.qt5.android.QtNative;
import androidnative.SystemDispatcher;

public class PrefHelper {
	private static final String DATA_CHANGED_MESSAGE = "AndroidUtils.PrefHelper.changed.";
	private static final String DATA_LOADED_MESSAGE = "AndroidUtils.PrefHelper.loaded.";

	static {
		SystemDispatcher.addListener(new SystemDispatcher.Listener() {

			public void onDispatched(String name , Map data) {
				if (name.equals("AndroidUtils.PrefHelper.getPrefs"))
					getPrefs((String)data.get("id"));
				else if (name.equals("AndroidUtils.PrefHelper.getSharedPrefs"))
					getSharedPrefs((String)data.get("id"), (String)data.get("name"));
				else if (name.equals("AndroidUtils.PrefHelper.remPrefs"))
					remPrefs((String)data.get("id"));
				else if (name.equals("AndroidUtils.PrefHelper.save"))
					save((String)data.get("id"), (String)data.get("key"), data.get("value"));
				else if (name.equals("AndroidUtils.PrefHelper.remove"))
					remove((String)data.get("id"), (String)data.get("key"));
			}
		});
	}

	interface HelperListener extends SharedPreferences.OnSharedPreferenceChangeListener {
		SharedPreferences preferences();
	}

	static Map<String, HelperListener> _activePrefs = new HashMap<>();

	static void getPrefs(String id) {
		registerPrefs(id, QtNative.activity().getPreferences(Context.MODE_PRIVATE));
	}

	static void getSharedPrefs(String id, String name) {
		Context c = QtNative.activity();
		if(c == null)
			c = QtNative.service();
		registerPrefs(id, c.getSharedPreferences(name, Context.MODE_PRIVATE));
	}

	static void remPrefs(final String id) {
		HelperListener listener = _activePrefs.remove(id);
		listener.preferences().unregisterOnSharedPreferenceChangeListener(listener);
	}

	static void save(String id, String key, Object value) {
		SharedPreferences.Editor prefs = _activePrefs.get(id).preferences().edit();
		if (value.getClass() == Boolean.class) {
			prefs.putBoolean(key, (Boolean)value);
		} else if (value.getClass() == Float.class) {
			prefs.putFloat(key, (Float)value);
		} else if (value.getClass() == Integer.class) {
			prefs.putInt(key, (Integer)value);
		} else if (value.getClass() == Long.class) {
			prefs.putLong(key, (Long)value);
		} else if (value.getClass() == String.class) {
			prefs.putString(key, (String)value);
		}
		prefs.apply();
	}

	static void remove(String id, String key) {
		SharedPreferences.Editor prefs = _activePrefs.get(id).preferences().edit();
		prefs.remove(key);
		prefs.apply();
	}

	static void registerPrefs(final String id, final SharedPreferences preferences) {
		HelperListener listener = new HelperListener() {

			public SharedPreferences preferences() {
				return preferences;
			}

			public void onSharedPreferenceChanged(SharedPreferences prefs, String key) {
				if(prefs.contains(key)) {
					Map<String, ?> data = prefs.getAll();
					Map reply = new HashMap();
					reply.put("key", key);
					reply.put("value", data.get(key));
					SystemDispatcher.dispatch(DATA_CHANGED_MESSAGE + id, reply);
				} else {
					Map reply = new HashMap();
					reply.put("key", key);
					reply.put("removed", true);
					SystemDispatcher.dispatch(DATA_CHANGED_MESSAGE + id, reply);
				}
			}
		};
		_activePrefs.put(id, listener);
		preferences.registerOnSharedPreferenceChangeListener(listener);

		SystemDispatcher.dispatch(DATA_LOADED_MESSAGE + id, preferences.getAll());
	}
}
