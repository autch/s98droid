package net.autch.android.s98droid;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.view.KeyEvent;

public class RemoteControlEventReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		KeyEvent key = intent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
		switch (key.getKeyCode()) {
		case KeyEvent.KEYCODE_HEADSETHOOK:
		case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
		    context.startService(new Intent(S98PlayerService.ACTION_PLAY));
		    break;
		case KeyEvent.KEYCODE_MEDIA_PLAY:
		    context.startService(new Intent(S98PlayerService.ACTION_PLAY));
		    break;
		case KeyEvent.KEYCODE_MEDIA_PAUSE:
		    context.startService(new Intent(S98PlayerService.ACTION_PAUSE));
		    break;
		case KeyEvent.KEYCODE_MEDIA_STOP:
		    context.startService(new Intent(S98PlayerService.ACTION_STOP));
		    break;
		case KeyEvent.KEYCODE_MEDIA_NEXT:
//		    context.startService(new Intent(MusicPlayerService.ACTION_SKIP));
		    break;
		case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
//		    context.startService(new Intent(MusicPlayerService.ACTION_REWIND));
		    break;
		}
	}

}
