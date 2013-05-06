package net.autch.android.s98droid;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.ListActivity;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.database.Cursor;
import android.media.AudioManager;
import android.media.RemoteControlClient;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.provider.BaseColumns;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class SelectSong extends ListActivity {
	private static final String TAG = "S98SelectSong";

	private static final int MID_STOP = 0x1001;

	private final List<Map<String, String>> files = new ArrayList<Map<String, String>>();
	private final Handler handler = new Handler();
	private RemoteControlClient myRemoteControlClient;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		final SelectSong me = this;
		final ProgressDialog dialog;
		super.onCreate(savedInstanceState);

		PMDWinNativeInterface.pmdwinInit();
		MS98NativeInterface.ms98Init();

		files.clear();

		ComponentName myEventReceiver = new ComponentName(this, RemoteControlEventReceiver.class);
		AudioManager myAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		myAudioManager.registerMediaButtonEventReceiver(myEventReceiver);
		// build the PendingIntent for the remote control client
		Intent mediaButtonIntent = new Intent(Intent.ACTION_MEDIA_BUTTON);
		mediaButtonIntent.setComponent(myEventReceiver);
		PendingIntent mediaPendingIntent = PendingIntent.getBroadcast(getApplicationContext(), 0, mediaButtonIntent, 0);
		// create and register the remote control client
		myRemoteControlClient = new RemoteControlClient(mediaPendingIntent);
		myAudioManager.registerRemoteControlClient(myRemoteControlClient);

		myRemoteControlClient.setPlaybackState(RemoteControlClient.PLAYSTATE_STOPPED);
		myRemoteControlClient.setTransportControlFlags(RemoteControlClient.FLAG_KEY_MEDIA_PLAY | RemoteControlClient.FLAG_KEY_MEDIA_PAUSE | RemoteControlClient.FLAG_KEY_MEDIA_STOP);
		
		dialog = ProgressDialog.show(this, null, "曲を探しています...", true, false);
		final ContentResolver cr = this.getContentResolver();
		new Thread(new Runnable() {
			private String getWhereClause() {
				StringBuilder s = new StringBuilder();
				
				s.append(MediaStore.Files.FileColumns.MEDIA_TYPE).append("=").append(MediaStore.Files.FileColumns.MEDIA_TYPE_NONE);
				s.append(" AND (");
				s.append("LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.s98'");
				s.append("OR LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.m'");
				s.append("OR LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.m2'");
				s.append("OR LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.mz'");
				s.append("OR LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.mp'");
				s.append("OR LOWER(").append(MediaStore.Files.FileColumns.DATA).append(") LIKE '%.ms'");
				s.append(")");
				return s.toString();
			}

			public void run() {
				Uri uri = MediaStore.Files.getContentUri("external");
				String[] projection = { BaseColumns._ID, MediaColumns.DATA };
				String where = getWhereClause();
				Cursor mediaFiles = cr.query(uri, projection, where, null,
						MediaColumns.DATA);
				try {
					int dataCol = mediaFiles
							.getColumnIndexOrThrow(MediaColumns.DATA);
					while (mediaFiles.moveToNext()) {
						String filename = mediaFiles.getString(dataCol);
						File f = new File(filename);
						try {
							Map<String, String> tags;
							if (f.getName().toLowerCase().endsWith(".s98")) {
								tags = S98FileParser.getTagInfo(f
										.getAbsolutePath());
							} else {
								tags = PMDFileParser.getTagInfo(f
										.getAbsolutePath());
							}
							if (tags.get("title") == null
									|| tags.get("title").length() == 0) {
								tags.put("title", f.getName());
							}
							files.add(tags);
						} catch (/* IO */Exception e) {
							// TODO 自動生成された catch ブロック
							// e.printStackTrace();
							// return false;
						}
					}
					handler.post(new Runnable() {
						public void run() {
							SimpleAdapter adapter = new SimpleAdapter(
									SelectSong.this, files,
									android.R.layout.simple_list_item_2,
									new String[] { "title", "game" },
									new int[] { android.R.id.text1,
											android.R.id.text2 });
							setListAdapter(adapter);
							getListView().setFastScrollEnabled(true);
							getListView().getParent().requestLayout();
							dialog.dismiss();
						}
					});
				} finally {
					mediaFiles.close();
				}
			}
		}).start();
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Map<String, String> item = files.get(position);

		Intent it = new Intent(S98PlayerService.ACTION_PLAY);
		it.putExtra("filename", item.get("filename"));
		it.putExtra("title", item.get("title"));
		it.putExtra("title2", item.get("game"));
		startService(it);

		Toast t = Toast.makeText(this, "演奏を開始します", Toast.LENGTH_SHORT);
		t.show();

		myRemoteControlClient.setPlaybackState(RemoteControlClient.PLAYSTATE_PLAYING);

		// it = new Intent(this, PlayingView.class);
		// startActivity(it);
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		menu.add(Menu.NONE, MID_STOP, Menu.NONE, "停止");
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case MID_STOP:
			startService(new Intent(S98PlayerService.ACTION_STOP));
			Toast t = Toast.makeText(this, "停止しました", Toast.LENGTH_SHORT);
			t.show();
			myRemoteControlClient.setPlaybackState(RemoteControlClient.PLAYSTATE_STOPPED);
		}
		return super.onMenuItemSelected(featureId, item);
	}
}
