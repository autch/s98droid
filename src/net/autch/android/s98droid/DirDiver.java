package net.autch.android.s98droid;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Arrays;

public class DirDiver {
	private final File root;
	private final FileFilter filter;
	private Callback callback;

	public interface Callback {
		boolean process(File f);
	}

	public DirDiver(String rootdir, String extension) {
		root = new File(rootdir);
		final String fsExtension = extension.toLowerCase();

		filter = new FileFilter() {
			public boolean accept(File pathname) {
				return pathname.isDirectory()
				|| pathname.getName().toLowerCase().endsWith(fsExtension);
			}
		};
	}

	public DirDiver(String rootdir, FileFilter filter) {
		root = new File(rootdir);
		this.filter = filter;
	}

	public void setCallback(Callback callback) {
		this.callback = callback;
	}

	public void start() {
		dirDiver(root);
	}

	private void dirDiver(File pivot) {
		ArrayList<File> dirs = new ArrayList<File>();
		File[] files = pivot.listFiles(filter);
		if(files == null) return;
		Arrays.sort(files);
		for(File f : files) {
			if(f.toString().startsWith("."))
				continue;
			if(f.isDirectory()) {
				dirs.add(f);
			} else {
				if(!callback.process(f))
					break;
			}
		}
		for(File d : dirs) {
			dirDiver(d);
		}
	}
}
