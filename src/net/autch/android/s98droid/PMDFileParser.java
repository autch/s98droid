package net.autch.android.s98droid;

import java.util.HashMap;
import java.util.Map;

public class PMDFileParser {
	private static final String TAG = "PMDFileParser";

	public static Map<String, String> getTagInfo(String filename) throws Exception {
		HashMap<String, String> tags = new HashMap<String, String>();
		byte buffer[] = new byte[4096];
		int ret;

		tags.put("filename", filename);

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
