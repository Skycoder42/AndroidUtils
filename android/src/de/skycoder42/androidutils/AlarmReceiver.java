package de.skycoder42.androidutils;

import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.app.AlarmManager;
import android.util.Log;
import android.support.v4.content.WakefulBroadcastReceiver;

public class AlarmReceiver extends WakefulBroadcastReceiver {
	private static final String TAG = "de.skycoder42.androidutils.AlarmReceiver";
	private static final int ALARM_INTENT_ID = 20;
	private static final String CLASS_NAME_EXTRA = "de.skycoder42.androidutils.classname";

	@Override
	public void onReceive(Context context, Intent intent) {
		try {
			Class classType = Class.forName(intent.getStringExtra(CLASS_NAME_EXTRA));
			Intent serviceIntent = new Intent(context, classType);
			startWakefulService(context, serviceIntent);
		} catch(ClassNotFoundException e) {
			Log.e(TAG, e.getMessage());
			e.printStackTrace();
		}
	}

	static public void scheduleAutoCheck(Context context, boolean autoCheck, String serviceClass) {
		Intent intent = new Intent(context, AlarmReceiver.class);
		intent.putExtra(CLASS_NAME_EXTRA, serviceClass);

		PendingIntent pending = PendingIntent.getBroadcast(context,
			ALARM_INTENT_ID,
			intent,
			PendingIntent.FLAG_UPDATE_CURRENT);

		AlarmManager alarm = (AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
		if(autoCheck) {
			alarm.setRepeating(AlarmManager.RTC_WAKEUP,
				System.currentTimeMillis() + AlarmManager.INTERVAL_DAY,
				AlarmManager.INTERVAL_DAY,
				pending);
		} else {
			alarm.cancel(pending);
			pending.cancel();
		}
	}
}
