package net.autch.android.s98droid;


import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class PMDFileParser {

	private static int getShort(byte[] b, int offset) {
		return b[offset] | (b[offset + 1] << 8);
	}

	public static String[] getTitleInfo(File f) throws IOException {
		String[] titles = new String[] { "", "" };
		byte[] raw = new byte[8192];
		int offset = 0;

		FileInputStream is = new FileInputStream(f);
		try {
			is.read(raw);
		} finally {
			is.close();
		}

		int parts = raw[0];	// 0, or # of parts
		offset++;
		if(parts == 0) {
			parts = raw[1];
			offset++;
		}
		offset += 2 * parts + 2;

		int adrTitle, adrTitle2;
		int lenTitle, lenTitle2;
		adrTitle = getShort(raw, offset);
		adrTitle2 = getShort(raw, offset + 2);

		if(adrTitle > 0) {
			for(lenTitle = 0; raw[adrTitle + lenTitle] != 0x00; lenTitle++)
				;
			titles[0] = new String(raw, adrTitle, lenTitle, "Shift_JIS");
		}
		if(adrTitle2 > 0) {
			for(lenTitle2 = 0; raw[adrTitle2 + lenTitle2] != 0x00; lenTitle2++)
				;
			titles[1] = new String(raw, adrTitle2, lenTitle2, "Shift_JIS");
		}

		return titles;
	}
}
