
TARGETS = m_s98/net_autch_android_s98droid_MS98NativeInterface.h \
pmdwin/net_autch_android_s98droid_PMDWinNativeInterface.h

all: $(TARGETS)

m_s98/net_autch_android_s98droid_MS98NativeInterface.h: ../src/net/autch/android/s98droid/MS98NativeInterface.java
	javah -classpath ../bin/classes -o $@ net.autch.android.s98droid.MS98NativeInterface

pmdwin/net_autch_android_s98droid_PMDWinNativeInterface.h: ../src/net/autch/android/s98droid/PMDWinNativeInterface.java
	javah -classpath ../bin/classes -o $@ net.autch.android.s98droid.PMDWinNativeInterface
