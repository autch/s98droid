package net.autch.android.s98droid;

import java.io.FileInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class PMDFileParser {
	private static final String TAG = "PMDFileParser";

	public static Map<String, String> getTagInfo(String filename) throws Exception {
		HashMap<String, String> tags = new HashMap<String, String>();
		byte buffer[] = new byte[4096];
		int ret;

		tags.put("filename", filename);

		InputStream is = new FileInputStream(filename);
		try {
			byte[] prologue = new byte[4];
			
			is.read(prologue, 0, 4);

			if((prologue[0] > 0x0f && prologue[0] != 0xff)
					|| (prologue[1] != 0x18 && prologue[1] != 0x1a)
					|| prologue[2] != 0x00) {
				throw new Exception("This is not a PMD file");
			}
		} finally {
			is.close();
		}

		ret = PMDWinNativeInterface.pmdwinFGetMemo3(buffer, filename, 1);
		if(ret == 0) {
			String s = new String(buffer, "Shift_JIS");
			tags.put("title", s);
		}
		ret = PMDWinNativeInterface.pmdwinFGetMemo3(buffer, filename, 2);
		if(ret == 0) {
			String s = new String(buffer, "Shift_JIS");
			tags.put("game", s);
		}
		
		
		return tags;
	}
}
