package net.autch.android.s98droid;

public final class PMDWinNativeInterface {
	static {
		System.loadLibrary("pmdwin");
	}

	public static native void pmdwinInit();
	public static native void pmdwinDeinit();
	public static native int pmdwinOpenFile(String filename);
	public static native int pmdwinRender(byte[] buffer, int size);
	public static native void pmdwinClose();
	
	public static native int pmdwinFGetMemo(byte[] dest, String filename, int al);
	public static native int pmdwinFGetMemo2(byte[] dest, String filename, int al);
	public static native int pmdwinFGetMemo3(byte[] dest, String filename, int al);
}
