package net.autch.android.s98droid;

public final class MS98NativeInterface {
	static {
		System.loadLibrary("m_s98");
	}

	public static native void ms98Init();
	public static native void ms98Deinit();
	public static native int ms98OpenFile(String filename);
	public static native int ms98Render(byte[] buffer, int size);
	public static native void ms98Close();
}
