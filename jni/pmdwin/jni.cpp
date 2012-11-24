
#define	LOCAL_LOG

#include "pmdwin.h"
#include "net_autch_android_s98droid_PMDWinNativeInterface.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef	LOCAL_LOG
#include <android/log.h>
#endif

#define SAMPLE_RATE 48000

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinInit
  (JNIEnv *, jclass)
{
	pmdwininit((char*)"/sdcard/s98");
	setpcmrate(SAMPLE_RATE);
	setfmcalc55k(true);
	setppsuse(true);
	setrhythmwithssgeffect(true);
	setppsinterpolation(true);
	setp86interpolation(true);
	setppzinterpolation(true);
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinDeinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinDeinit
  (JNIEnv *, jclass)
{
	music_stop();
}

int load_pmdwin(const char* cszFileName)
{
	music_stop();

	int ret = music_load((char*)cszFileName);

	__android_log_print(ANDROID_LOG_DEBUG, "PMDWin", "music_load: %d", ret);

	switch(ret) {
	case PMDWIN_OK:
	case ERR_OPEN_PPC_FILE:
	case WARNING_PPC_ALREADY_LOAD:
	case ERR_OPEN_P86_FILE:
	case ERR_OPEN_PPS_FILE:
	case WARNING_PPS_ALREADY_LOAD:
	case ERR_OPEN_PPZ1_FILE:
	case ERR_OPEN_PPZ2_FILE:
	case WARNING_PPZ1_ALREADY_LOAD:
	case WARNING_PPZ2_ALREADY_LOAD:
		music_start();
		return 1;
	case ERR_WRONG_PPC_FILE:
	case ERR_WRONG_PPZ1_FILE:
	case ERR_WRONG_PPZ2_FILE:
	case ERR_WRONG_P86_FILE:
	case ERR_OPEN_MUSIC_FILE:
	case ERR_WRONG_MUSIC_FILE:
	case ERR_OUT_OF_MEMORY:
	case ERR_OTHER:
		return 0;
	}
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinOpenFile
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinOpenFile
  (JNIEnv* env, jclass klass, jstring filename)
{
	int ret = 0;

	if(filename == NULL) return 0;

	const char* cszFileName = env->GetStringUTFChars(filename, NULL);
	char* pPeriod;


	pPeriod = strrchr(cszFileName, '.');
	if(pPeriod == NULL) {
		env->ReleaseStringUTFChars(filename, cszFileName);
		return 0;
	}

	ret = load_pmdwin(cszFileName);
	env->ReleaseStringUTFChars(filename, cszFileName);
	return ret;
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinRender
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinRender
  (JNIEnv *env, jclass klass, jbyteArray jbuffer, jint size)
{
	jbyte* buffer = env->GetByteArrayElements(jbuffer, NULL);
	jint ret;

	memset(buffer, 0, size);
	getpcmdata((short*)buffer, size / 4);
	ret = size;

	env->ReleaseByteArrayElements(jbuffer, buffer, 0);

	return ret;
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinClose
  (JNIEnv *, jclass)
{
	music_stop();
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinFGetMemo
 * Signature: ([BLjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinFGetMemo
	(JNIEnv* env, jclass klass, jbyteArray jbuffer, jstring jfilename, jint al)
{
	jint ret;
	jbyte* buffer = env->GetByteArrayElements(jbuffer, NULL); // destination
	const char* filename = env->GetStringUTFChars(jfilename, NULL);

	ret = fgetmemo((char*)buffer, (char*)filename, al);

	env->ReleaseByteArrayElements(jbuffer, buffer, 0);
	env->ReleaseStringUTFChars(jfilename, filename);

	return ret;
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinFGetMemo2
 * Signature: ([BLjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinFGetMemo2
	(JNIEnv* env, jclass klass, jbyteArray jbuffer, jstring jfilename, jint al)
{
	jint ret;
	jbyte* buffer = env->GetByteArrayElements(jbuffer, NULL); // destination
	const char* filename = env->GetStringUTFChars(jfilename, NULL);

	ret = fgetmemo2((char*)buffer, (char*)filename, al);

	env->ReleaseByteArrayElements(jbuffer, buffer, 0);
	env->ReleaseStringUTFChars(jfilename, filename);

	return ret;
}

/*
 * Class:     net_autch_android_s98droid_PMDWinNativeInterface
 * Method:    pmdwinFGetMemo3
 * Signature: ([BLjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_PMDWinNativeInterface_pmdwinFGetMemo3
	(JNIEnv* env, jclass klass, jbyteArray jbuffer, jstring jfilename, jint al)
{
	jint ret;
	jbyte* buffer = env->GetByteArrayElements(jbuffer, NULL); // destination
	const char* filename = env->GetStringUTFChars(jfilename, NULL);

	ret = fgetmemo3((char*)buffer, (char*)filename, al);

	env->ReleaseByteArrayElements(jbuffer, buffer, 0);
	env->ReleaseStringUTFChars(jfilename, filename);

	return ret;
}

