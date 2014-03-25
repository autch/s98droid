package net.autch.android.s98droid;

import java.io.File;
import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

public class S98FileParser {
	private static final String TAG = "S98FileParser";
	
	private static long getWordLE(byte[] b, int offset) {
		long r = 0;
		r |= (b[offset] & 0xff);
		r |= ((b[offset + 1] << 8) & 0xff00);
		r |= ((b[offset + 2] << 16) & 0xff0000);
		r |= ((b[offset + 3] << 24) & 0xff000000);
		return r;
	}
	
	public static Map<String, String> getTagInfo(String filename) throws Exception {
		HashMap<String, String> tags = new HashMap<String, String>();
		byte buffer[] = new byte[8192];
		int s98Version = -1;
		int bytesRead = 0;

		RandomAccessFile raf = new RandomAccessFile(filename, "r");
		try {
			bytesRead = raf.read(buffer);
	
			if(bytesRead < 0x20) {
				throw new Exception(filename + ": Seems not to be a valid S98");
			}
			
			tags.put("filename", filename);
			
			String magic = new String(buffer, 0, 4, "Shift_JIS");
			if(magic.equals("S981")) {
				tags.put("version", "1");
				s98Version = 1;
			} else if(magic.equals("S982")) {
				tags.put("version", "2");
				s98Version = 2;
			} else if(magic.equals("S983")) {
				tags.put("version", "3");
				s98Version = 3;
			} else {
				throw new Exception(filename + ": Unexpected S98 version");
			}
	
			long offsetToTag = getWordLE(buffer, 0x10);
			if(offsetToTag > 0) {
				int lenTitle;
				raf.seek(offsetToTag);
				bytesRead = raf.read(buffer);
				
				switch(s98Version) {
				case 1:
				case 2:
					for(lenTitle = 0; buffer[lenTitle] != 0x00; lenTitle++)
						;
					tags.put("title", new String(buffer, 0, lenTitle, "Shift_JIS"));
					break;
				case 3:
					String tagMarker = new String(buffer, 0, 5, "Shift_JIS");
					if(tagMarker.equals("[S98]")) {
						try {
							String charCode;
							int offset = 0;
							if(buffer[5] == (byte)0xef && buffer[6] == (byte)0xbb && buffer[7] == (byte)0xbf) {
								// BOM
								charCode = "UTF-8";
								offset = 5 + 3;
							} else {
								charCode = "Shift_JIS";
								offset = 5;
							}
							if(buffer[bytesRead-1] == 0x00) bytesRead--;
							String wholeTags = new String(buffer, offset, bytesRead - offset, charCode);
							String[] rawTags = wholeTags.split("\\x0a");
							for(int i = 0; i < rawTags.length; i++) {
								if(rawTags[i].length() == 0) continue;
								String[] kv = rawTags[i].split("=");
								tags.put(kv[0].toLowerCase(), kv[1]);
							}
						} catch(Exception e) {
							Log.e(TAG, "Failed to parse file: " + filename, e);
						}
					} else {
						for(lenTitle = 0; buffer[lenTitle] != 0x00; lenTitle++)
							;
						tags.put("title", new String(buffer, 0, lenTitle, "Shift_JIS"));
					}
					break;
				}
			}
			
			if(!tags.containsKey("game")) {
				String[] dirComponents = new File(filename).getPath().split("/");
				tags.put("game", dirComponents[dirComponents.length - 2]);
			}
		} finally {
			raf.close();
		}
		
		return tags;
	}

}
