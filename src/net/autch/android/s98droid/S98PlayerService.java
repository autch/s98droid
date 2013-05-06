package net.autch.android.s98droid;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class S98PlayerService extends Service {
	private static final String TAG = "S98PlayerService";

	public static final String ACTION_PLAY = "net.autch.android.s98droid.service.ACTION_PLAY";
	public static final String ACTION_PAUSE = "net.autch.android.s98droid.service.ACTION_PAUSE";
	public static final String ACTION_RESUME = "net.autch.android.s98droid.service.ACTION_RESUME";
	public static final String ACTION_STOP = "net.autch.android.s98droid.service.ACTION_STOP";
	
	private static final int SAMPLES_PER_SEC = 48000;
	private static final int BYTES_PER_SECOND = SAMPLES_PER_SEC * 2 * 2;
	private static final int TRACK_BUFFER_SIZE = BYTES_PER_SECOND / 2;
	private static final int BLOCKS_AT_ONCE = BYTES_PER_SECOND / 4;
	private static final int WAIT_PER_BLOCK = 20;

	private static final int NID_PMD_PLAYING = 0x1;

	private AudioTrack track;
	private boolean terminate;
	private final byte[] buffer = new byte[BLOCKS_AT_ONCE];
	private final byte[] empty = new byte[256];
	private String filename, title, title2;
	private Thread thread; 
	
	// 何度も使いまわす
	private Runnable audioStreamer = null;
	private final Runnable audioStreamerS98 = new Runnable() {
		public void run() {
			while(!terminate) {
				try {
					int ret = MS98NativeInterface.ms98Render(buffer, buffer.length);
					if(ret == 0) {
						track.stop();
						Log.d(TAG, "stream terminated");
						MS98NativeInterface.ms98Close();
						break;
					}
					track.write(buffer, 0, buffer.length);
					Thread.sleep(WAIT_PER_BLOCK);
				} catch (InterruptedException e) {
					// thru
				}
			}
		}
	};
	private final Runnable audioStreamerPMDWin = new Runnable() {
		public void run() {
			while(!terminate) {
				try {
					int ret = PMDWinNativeInterface.pmdwinRender(buffer, buffer.length);
					if(ret == 0) {
						track.stop();
						Log.d(TAG, "stream terminated");
						PMDWinNativeInterface.pmdwinClose();
						break;
					}
					track.write(buffer, 0, buffer.length);
					Thread.sleep(WAIT_PER_BLOCK);
				} catch (InterruptedException e) {
					// thru
				}
			}
		}
	};

	@Override
	public void onCreate() {
		super.onCreate();
		Log.d(TAG, "onCreate()");

		track = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLES_PER_SEC, AudioFormat.CHANNEL_OUT_STEREO,
				AudioFormat.ENCODING_PCM_16BIT,	TRACK_BUFFER_SIZE, AudioTrack.MODE_STREAM);
		terminate = false;
		thread = null;
		MS98NativeInterface.ms98Init();
		PMDWinNativeInterface.pmdwinInit();
	}

	@Override
	public void onDestroy() {
		stopSong();
		track.release();
		MS98NativeInterface.ms98Deinit();
		PMDWinNativeInterface.pmdwinDeinit();

		super.onDestroy();
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		super.onStartCommand(intent, flags, startId);
		String action = intent.getAction();
		if(action.equals(ACTION_PLAY)) {
			filename = intent.getStringExtra("filename");
			title = intent.getStringExtra("title");
			title2 = intent.getStringExtra("title2");
			Log.d(TAG, "onStart(): " + filename);
			startSong(filename);
		} else if(action.equals(ACTION_PAUSE)) {
			pauseSong();
		} else if(action.equals(ACTION_RESUME)) {
			resumeSong();
		} else if(action.equals(ACTION_STOP)) {
			stopSong();
		}
		return START_STICKY;
	}

	public synchronized void startSong(String filename) {
		if(thread != null)
			stopSong();
		this.filename = filename;
		
		if(this.filename.toLowerCase().endsWith(".s98")) {
			if(MS98NativeInterface.ms98OpenFile(filename) != 1) {
				Log.e(TAG, "Cannot open " + filename + " for playing");
				stopSelf();
				return;
			}
			audioStreamer = audioStreamerS98;
		} else {
			if(PMDWinNativeInterface.pmdwinOpenFile(filename) != 1) {
				Log.e(TAG, "Cannot open " + filename + " for playing");
				stopSelf();
				return;
			}
			audioStreamer = audioStreamerPMDWin;
		}
		terminate = false;
		track.play();
		track.write(empty, 0, empty.length);

		thread = new Thread(audioStreamer);
		thread.start();

		Notification nf = new Notification(R.drawable.ic_launcher, title, System.currentTimeMillis());
		nf.flags |= Notification.FLAG_ONGOING_EVENT | Notification.FLAG_NO_CLEAR;

		Intent notificationIntent = new Intent(this, SelectSong.class);
		PendingIntent contentIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);

		nf.setLatestEventInfo(getApplicationContext(), title, title2, contentIntent);
		//nm.notify(NID_PMD_PLAYING, nf);
		startForeground(NID_PMD_PLAYING, nf);
	}

	public synchronized void pauseSong() {
		terminate = true;
		if(thread == null) return;
		try {
			thread.join();
		} catch (InterruptedException e) {
		}
		thread = null;
	}

	public synchronized void resumeSong() {
		terminate = false;
		track.play();
		thread = new Thread(audioStreamer);
		thread.start();
	}

	public synchronized void stopSong() {
		terminate = true;
		if(thread == null) return;
		try {
			thread.join();
		} catch (InterruptedException e) {
		}
		thread = null;
		track.stop();
		MS98NativeInterface.ms98Close();
		PMDWinNativeInterface.pmdwinClose();
		//nm.cancel(NID_PMD_PLAYING);
		stopForeground(true);
	}

	public String getFilename() {
		return filename;
	}
	public String getTitle() {
		return title;
	}
	public String getTitle2() {
		return title2;
	}

	public boolean isPlaying() {
		return thread != null && track.getPlayState() == AudioTrack.PLAYSTATE_PLAYING;
	}

	public int getPosition() {
		long pos = track.getPlaybackHeadPosition();
		pos /= SAMPLES_PER_SEC * 2;
		return (int)pos;
	}

	@Override
	public IBinder onBind(Intent intent) {
		// TODO 自動生成されたメソッド・スタブ
		return null;
	}
}
