package net.autch.android.s98droid;


import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import android.app.ListActivity;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
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
	private ProgressDialog dialog; 

	private S98PlayerService player;
	private final ServiceConnection connection = new ServiceConnection() {
		public void onServiceDisconnected(ComponentName name) {
			Log.d(TAG, "onServiceDisconnected");
			player = null;
		}

		public void onServiceConnected(ComponentName name, IBinder service) {
			Log.d(TAG, "onServiceConnected");
			player = ((S98PlayerService.S98PlayerServiceBinder)service).getService();
		}
	};


	private final DirDiver.Callback enumFiles = new DirDiver.Callback() {
		public boolean process(File f) {
			try {
				Map<String, String> tags = S98FileParser.getTagInfo(f.getAbsolutePath());
				if(tags.get("title") == null || tags.get("title").length() == 0) {
					tags.put("title", f.getName());
				}
				files.add(tags);
			} catch (/*IO*/Exception e) {
				// TODO 自動生成された catch ブロック
				e.printStackTrace();
				return false;
			}
			return true;
		}
	}; 

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		final SelectSong me = this;
		super.onCreate(savedInstanceState);

		files.clear();

		dialog = ProgressDialog.show(this, null, "曲を探しています...", true, false);

		new Thread(new Runnable() {
			public void run() {
				DirDiver diver = new DirDiver(Environment.getExternalStorageDirectory().getPath(), ".s98");
				diver.setCallback(enumFiles);
				diver.start();

				handler.post(new Runnable() {
					public void run() {
						SimpleAdapter adapter = new SimpleAdapter(SelectSong.this, files, android.R.layout.simple_list_item_2,
								new String[] { "title", "game" }, new int[] { android.R.id.text1, android.R.id.text2 });
						setListAdapter(adapter);
						getListView().setFastScrollEnabled(true);
						getListView().getParent().requestLayout();
						dialog.dismiss();
					}
				});
			}
		}).start();
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		Map<String, String> item = files.get(position);

		Intent it = new Intent(this, S98PlayerService.class);
		it.putExtra("filename", item.get("filename"));
		it.putExtra("title", item.get("title"));
		it.putExtra("title2", item.get("title2"));
		startService(it);

		Toast t = Toast.makeText(this, "演奏を開始します", Toast.LENGTH_SHORT);
		t.show();

		// it = new Intent(this, PlayingView.class);
		// startActivity(it);
	}

	@Override
	protected void onPause() {
		super.onPause();
		unbindService(connection);
	}

	@Override
	protected void onResume() {
		super.onResume();

		Intent it = new Intent(this, S98PlayerService.class);
		if(!bindService(it, connection, Context.BIND_AUTO_CREATE))
			Log.e(TAG, "Cannot bind to S98 player service");
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
			player.stopSong();
			Toast t = Toast.makeText(this, "停止しました", Toast.LENGTH_SHORT);
			t.show();
		}
		return super.onMenuItemSelected(featureId, item);
	}
}
