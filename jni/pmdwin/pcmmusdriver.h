//=============================================================================
//		PCM Music Driver Interface Class : IPCMMUSICDRIVER
//=============================================================================

#ifndef IPCMMUSICDRIVER_H
#define IPCMMUSICDRIVER_H


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#ifndef __cplusplus
typedef unsigned char bool;
#endif


//=============================================================================
// IPCMMUSICDRIVER : 音源ドライバの基本的なインターフェイスを定義したクラス
//=============================================================================

class IPCMMUSICDRIVER {
	virtual bool init(char *path) = 0;
	virtual int music_load(char *filename) = 0;
	virtual int music_load2(uchar *musdata, int size) = 0;
	virtual char* getmusicfilename(char *dest) = 0;
	virtual void music_start(void) = 0;
	virtual void music_stop(void) = 0;
	virtual int getloopcount(void) = 0;
	virtual bool getlength(char *filename, int *length, int *loop) = 0;
	virtual int getpos(void) = 0;
	virtual void setpos(int pos) = 0;
	virtual void getpcmdata(short *buf, int nsamples) = 0;
};


//=============================================================================
// IFMPMD : WinFMP, PMDWin に共通なインターフェイスを定義したクラス
//=============================================================================
class IFMPMD : public IPCMMUSICDRIVER {
	virtual bool loadrhythmsample(char *path) = 0;
	virtual bool setpcmdir(char **path) = 0;
	virtual void setpcmrate(int rate) = 0;
	virtual void setppzrate(int rate) = 0;
	virtual void setfmcalc55k(bool flag) = 0;
	virtual void setppzinterpolation(bool ip) = 0;
	virtual void setfmwait(int nsec) = 0;
	virtual void setssgwait(int nsec) = 0;
	virtual void setrhythmwait(int nsec) = 0;
	virtual void setadpcmwait(int nsec) = 0;
	virtual void fadeout(int speed) = 0;
	virtual void fadeout2(int speed) = 0;
	virtual bool getlength2(char *filename, int *length, int *loop) = 0;
	virtual int getpos2(void) = 0;
	virtual void setpos2(int pos) = 0;
	virtual char* getpcmfilename(char *dest) = 0;
	virtual char* getppzfilename(char *dest, int bufnum) = 0;
};

#endif	// IPCMMUSICDRIVER_H
