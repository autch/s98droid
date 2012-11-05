
#include "m_s98.h"
#include "net_autch_android_s98droid_MS98NativeInterface.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static s98File* theFile = NULL; // THE s98 file

/*
 * Class:     net_autch_android_s98droid_MS98NativeInterface
 * Method:    ms98Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_MS98NativeInterface_ms98Init
  (JNIEnv *, jclass)
{
	// do nothing
}

/*
 * Class:     net_autch_android_s98droid_MS98NativeInterface
 * Method:    ms98Deinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_MS98NativeInterface_ms98Deinit
  (JNIEnv *, jclass)
{
	if(theFile != NULL) {
		delete theFile;
		theFile = NULL;
	}
}

/*
 * Class:     net_autch_android_s98droid_MS98NativeInterface
 * Method:    ms98OpenFile
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_MS98NativeInterface_ms98OpenFile
  (JNIEnv* env, jclass klass, jstring filename)
{
	if(filename == NULL) return 0;

	const char* cszFileName = env->GetStringUTFChars(filename, NULL);
	uint32_t bytes_read;
	uint8_t *Buffer = 0;
	int fd = -1, ret = 0;
	struct stat stat_buf;
	do
	{
		fd = ::open(cszFileName, O_RDONLY);
		if (fd < 0) break;
		fstat(fd, &stat_buf);
		if (stat_buf.st_size < 4) break;
		Buffer = new uint8_t[stat_buf.st_size];
		if (!Buffer) break;
		if ((bytes_read = ::read(fd, Buffer, stat_buf.st_size)) < 0) break;
		if (stat_buf.st_size != bytes_read) break;

		if(theFile != NULL) {
			delete theFile;
			theFile = NULL;
		}

		theFile = new s98File;
		if(theFile == NULL) break;
		if(!theFile->OpenFromBuffer(Buffer, bytes_read)) break;
		ret = 1;
	} while (0);
	if (Buffer) delete [] Buffer;
	if (fd != -1) ::close(fd);
	env->ReleaseStringUTFChars(filename, cszFileName);
	return ret;
}

/*
 * Class:     net_autch_android_s98droid_MS98NativeInterface
 * Method:    ms98Render
 * Signature: ([BI)I
 */
JNIEXPORT jint JNICALL Java_net_autch_android_s98droid_MS98NativeInterface_ms98Render
  (JNIEnv *env, jclass klass, jbyteArray jbuffer, jint size)
{
	if(theFile == NULL) return 0;

	jbyte* buffer = env->GetByteArrayElements(jbuffer, NULL);
	jint ret;

	ret = theFile->Write((Int16*)buffer, (uint32_t)(size / 4)) * 4;

	env->ReleaseByteArrayElements(jbuffer, buffer, 0);

	return ret;
}

/*
 * Class:     net_autch_android_s98droid_MS98NativeInterface
 * Method:    ms98Close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_autch_android_s98droid_MS98NativeInterface_ms98Close
  (JNIEnv *, jclass)
{
	if(theFile == NULL) return;

	delete theFile;
	theFile = NULL;

}

