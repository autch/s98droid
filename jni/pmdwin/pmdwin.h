//=============================================================================
//	Professional Music Driver [P.M.D.] version 4.8
//			Programmed By M.Kajihara
//			Windows Converted by C60
//=============================================================================

#ifndef PMDWIN_H
#define PMDWIN_H

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "opnaw.h"
#include "ppz8l.h"
#include "ppsdrv.h"
#include "p86drv.h"
#include "PCMMusDriver.h"

//#include "types.h"


//=============================================================================
//	バージョン情報
//=============================================================================
#define	DLLVersion			 36		// 上１桁：major, 下２桁：minor version
#define	InterfaceVersion	117		// 上１桁：major, 下２桁：minor version


//=============================================================================
//	DLL の戻り値
//=============================================================================
#define	PMDWIN_OK				 	  0		// 正常終了
#define	ERR_OPEN_MUSIC_FILE			  1		// 曲 データを開けなかった
#define	ERR_WRONG_MUSIC_FILE		  2		// PMD の曲データではなかった
#define	ERR_OPEN_PPC_FILE		 	  3		// PPC を開けなかった
#define	ERR_OPEN_P86_FILE		 	  4		// P86 を開けなかった
#define	ERR_OPEN_PPS_FILE		 	  5		// PPS を開けなかった
#define	ERR_OPEN_PPZ1_FILE		 	  6		// PPZ1 を開けなかった
#define	ERR_OPEN_PPZ2_FILE		 	  7		// PPZ2 を開けなかった
#define	ERR_WRONG_PPC_FILE		 	  8		// PPC/PVI ではなかった
#define	ERR_WRONG_P86_FILE		 	  9		// P86 ではなかった
#define	ERR_WRONG_PPS_FILE		 	 10		// PPS ではなかった
#define	ERR_WRONG_PPZ1_FILE		 	 11		// PVI/PZI ではなかった(PPZ1)
#define	ERR_WRONG_PPZ2_FILE		 	 12		// PVI/PZI ではなかった(PPZ2)
#define	WARNING_PPC_ALREADY_LOAD	 13		// PPC はすでに読み込まれている
#define	WARNING_P86_ALREADY_LOAD	 14		// P86 はすでに読み込まれている
#define	WARNING_PPS_ALREADY_LOAD	 15		// PPS はすでに読み込まれている
#define	WARNING_PPZ1_ALREADY_LOAD	 16		// PPZ1 はすでに読み込まれている
#define	WARNING_PPZ2_ALREADY_LOAD	 17		// PPZ2 はすでに読み込まれている

#define	ERR_WRONG_PARTNO			 30		// パート番号が不適
//#define	ERR_ALREADY_MASKED			 31		// 指定パートはすでにマスクされている
#define	ERR_NOT_MASKED				 32		// 指定パートはマスクされていない
#define	ERR_MUSIC_STOPPED			 33		// 曲が止まっているのにマスク操作をした
#define	ERR_EFFECT_USED				 34		// 効果音で使用中なのでマスクを操作できない

#define	ERR_OUT_OF_MEMORY			 99		// メモリを確保できなかった
#define	ERR_OTHER					999		// その他のエラー


//----------------------------------------------------------------------------
//	ＰＭＤＷｉｎ専用の定義
//----------------------------------------------------------------------------
#define	SOUND_55K			  55555
#define	SOUND_55K_2			  55466
#define	SOUND_48K			  48000
#define	SOUND_44K			  44100
#define	SOUND_22K			  22050
#define	SOUND_11K			  11025

#define	PPZ8_i0				  44100
#define	PPZ8_i1				  33080
#define	PPZ8_i2				  22050
#define	PPZ8_i3				  16540
#define	PPZ8_i4				  11025
#define	PPZ8_i5				   8270
#define	PPZ8_i6				   5513
#define	PPZ8_i7				   4135

#define	DEFAULT_REG_WAIT	  15000
#define	MAX_PCMDIR				 64
#define	MAX_MEMOBUF			   1024


//----------------------------------------------------------------------------
//	その他定義
//----------------------------------------------------------------------------
#define	nbufsample			  30000
#define	OPNAClock		(3993600*2)

#define	NumOfFMPart			      6
#define	NumOfSSGPart		      3
#define	NumOfADPCMPart		      1
#define	NumOFOPNARhythmPart	      1
#define	NumOfExtPart		      3
#define	NumOfRhythmPart		      1
#define	NumOfEffPart		      1
#define	NumOfPPZ8Part		      8
#define	NumOfAllPart		(NumOfFMPart+NumOfSSGPart+NumOfADPCMPart+NumOFOPNARhythmPart+NumOfExtPart+NumOfRhythmPart+NumOfEffPart+NumOfPPZ8Part)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int Sample;

#ifndef __cplusplus
typedef unsigned char bool;
#endif


typedef struct stereosampletag
{
	Sample left;
	Sample right;
} StereoSample;

#pragma pack( push, enter_include1 )
#pragma pack(2)

typedef struct stereo16bittag
{
	short left;
	short right;
} Stereo16bit;

typedef struct pcmendstag
{
	ushort pcmends;
	ushort pcmadrs[256][2];
} PCMEnds;

#pragma pack( pop, enter_include1 )


#define	PVIHeader	"PVI2"
#define	PPCHeader	"ADPCM DATA for  PMD ver.4.4-  "
//#define ver 		"4.8o"
//#define vers		0x48
//#define verc		"o"
//#define date		"Jun.19th 1997"

//#define max_part1	22		// ０クリアすべきパート数(for PMDPPZ)
#define max_part2	11		// 初期化すべきパート数  (for PMDPPZ)

#define mdata_def	64
#define voice_def	 8
#define effect_def	64

#define fmvd_init	0		// ９８は８８よりもＦＭ音源を小さく


/******************************************************************************
;	WORK AREA
******************************************************************************/
typedef struct PMDworktag {
	int		partb;				//	処理中パート番号
	int		tieflag;			//	&のフラグ(1 : tie)
	int		volpush_flag;		//	次の１音音量down用のflag(1 : voldown)
	int		rhydmy;				//	R part ダミー演奏データ
	int		fmsel;				//	FM 表(=0)か裏(=0x100)か flag
	int		omote_key[3];		//	FM keyondata表
	int		ura_key[3];			//	FM keyondata裏
	int		loop_work;			//	Loop Work
	bool	ppsdrv_flag;		//	ppsdrv を使用するか？flag(ユーザーが代入)
	int		pcmrepeat1;			//	PCMのリピートアドレス1
	int		pcmrepeat2;			//	PCMのリピートアドレス2
	int		pcmrelease;			//	PCMのRelease開始アドレス
	int		lastTimerAtime;		//	一個前の割り込み時のTimerATime値
	int		music_flag;			//	B0:次でMSTART 1:次でMSTOP のFlag
	int		slotdetune_flag;	//	FM3 Slot Detuneを使っているか
	int		slot3_flag;			//	FM3 Slot毎 要効果音モードフラグ
	int		fm3_alg_fb;			//	FM3chの最後に定義した音色のalg/fb
	int		af_check;			//	FM3chのalg/fbを設定するかしないかflag
	int		lfo_switch;			//	局所LFOスイッチ
} PMDWORK;


typedef struct effworktag {
	int		*effadr;			//	effect address
	int		eswthz;				//	トーンスゥイープ周波数
	int		eswtst;				//	トーンスゥイープ増分
	int		effcnt;				//	effect count
	int		eswnhz;				//	ノイズスゥイープ周波数
	int		eswnst;				//	ノイズスゥイープ増分
	int		eswnct;				//	ノイズスゥイープカウント
	int		effon;				//	効果音　発音中
	int		psgefcnum;			//	効果音番号
	int		hosei_flag;			//	ppsdrv 音量/音程補正をするかどうか
	int		last_shot_data;		//	最後に発音させたPPSDRV音色
} EFFWORK;


//	演奏中のデータエリア
typedef struct qqtag {
	uchar	*address;			//	2 エンソウチュウ ノ アドレス
	uchar	*partloop;			//	2 エンソウ ガ オワッタトキ ノ モドリサキ
	int		leng;				//	1 ノコリ LENGTH
	int		qdat;				//	1 gatetime (q/Q値を計算した値)
	uint	fnum;				//	2 エンソウチュウ ノ BLOCK/FNUM
	int		detune;				//	2 デチューン
	int		lfodat;				//	2 LFO DATA
	int		porta_num;			//	2 ポルタメントの加減値（全体）
	int		porta_num2;			//	2 ポルタメントの加減値（一回）
	int		porta_num3;			//	2 ポルタメントの加減値（余り）
	int		volume;				//	1 VOLUME
	int		shift;				//	1 オンカイ シフト ノ アタイ
	int		delay;				//	1 LFO	[DELAY] 
	int		speed;				//	1	[SPEED]
	int		step;				//	1	[STEP]
	int		time;				//	1	[TIME]
	int		delay2;				//	1	[DELAY_2]
	int		speed2;				//	1	[SPEED_2]
	int		step2;				//	1	[STEP_2]
	int		time2;				//	1	[TIME_2]
	int		lfoswi;				//	1 LFOSW. B0/tone B1/vol B2/同期 B3/porta
								//	         B4/tone B5/vol B6/同期
	int		volpush;			//	1 Volume PUSHarea
	int		mdepth;				//	1 M depth
	int		mdspd;				//	1 M speed
	int		mdspd2;				//	1 M speed_2
	int		envf;				//	1 PSG ENV. [START_FLAG] / -1でextend
	int		eenv_count;			//	1 ExtendPSGenv/No=0 AR=1 DR=2 SR=3 RR=4
	int		eenv_ar;			//	1 	/AR		/旧pat
	int		eenv_dr;			//	1	/DR		/旧pv2
	int		eenv_sr;			//	1	/SR		/旧pr1
	int		eenv_rr;			//	1	/RR		/旧pr2
	int		eenv_sl;			//	1	/SL
	int		eenv_al;			//	1	/AL
	int		eenv_arc;			//	1	/ARのカウンタ	/旧patb
	int		eenv_drc;			//	1	/DRのカウンタ
	int		eenv_src;			//	1	/SRのカウンタ	/旧pr1b
	int		eenv_rrc;			//	1	/RRのカウンタ	/旧pr2b
	int		eenv_volume;		//	1	/Volume値(0〜15)/旧penv
	int		extendmode;			//	1 B1/Detune B2/LFO B3/Env Normal/Extend
	int		fmpan;				//	1 FM Panning + AMD + PMD
	int		psgpat;				//	1 PSG PATTERN [TONE/NOISE/MIX]
	int		voicenum;			//	1 音色番号
	int		loopcheck;			//	1 ループしたら１ 終了したら３
	int		carrier;			//	1 FM Carrier
	int		slot1;				//	1 SLOT 1 ノ TL
	int		slot3;				//	1 SLOT 3 ノ TL
	int		slot2;				//	1 SLOT 2 ノ TL
	int		slot4;				//	1 SLOT 4 ノ TL
	int		slotmask;			//	1 FM slotmask
	int		neiromask;			//	1 FM 音色定義用maskdata
	int		lfo_wave;			//	1 LFOの波形
	int		partmask;			//	1 PartMask b0:通常 b1:効果音 b2:NECPCM用
								//	   b3:none b4:PPZ/ADE用 b5:s0時 b6:m b7:一時
	int		keyoff_flag;		//	1 KeyoffしたかどうかのFlag
	int		volmask;			//	1 音量LFOのマスク
	int		qdata;				//	1 qの値
	int		qdatb;				//	1 Qの値
	int		hldelay;			//	1 HardLFO delay
	int		hldelay_c;			//	1 HardLFO delay Counter
	int		_lfodat;			//	2 LFO DATA
	int		_delay;				//	1 LFO	[DELAY] 
	int		_speed;				//	1		[SPEED]
	int		_step;				//	1		[STEP]
	int		_time;				//	1		[TIME]
	int		_delay2;			//	1		[DELAY_2]
	int		_speed2;			//	1		[SPEED_2]
	int		_step2;				//	1		[STEP_2]
	int		_time2;				//	1		[TIME_2]
	int		_mdepth;			//	1 M depth
	int		_mdspd;				//	1 M speed
	int		_mdspd2;			//	1 M speed_2
	int		_lfo_wave;			//	1 LFOの波形
	int		_volmask;			//	1 音量LFOのマスク
	int		mdc;				//	1 M depth Counter (変動値)
	int		mdc2;				//	1 M depth Counter
	int		_mdc;				//	1 M depth Counter (変動値)
	int		_mdc2;				//	1 M depth Counter
	int		onkai;				//	1 演奏中の音階データ (0ffh:rest)
	int		sdelay;				//	1 Slot delay
	int		sdelay_c;			//	1 Slot delay counter
	int		sdelay_m;			//	1 Slot delay Mask
	int		alg_fb;				//	1 音色のalg/fb
	int		keyon_flag;			//	1 新音階/休符データを処理したらinc
	int		qdat2;				//	1 q 最低保証値
	int		onkai_def;			//	1 演奏中の音階データ (転調処理前 / ?fh:rest)
	int		shift_def;			//	1 マスター転調値
	int		qdat3;				//	1 q Random
} QQ;


typedef struct OpenWorktag {
	QQ *MusPart[NumOfAllPart];	// パートワークのポインタ
	uchar	*mmlbuf;			//	Musicdataのaddress+1
	uchar	*tondat;			//	Voicedataのaddress
	uchar	*efcdat;			//	FM  Effecdataのaddress
	uchar	*prgdat_adr;		//	曲データ中音色データ先頭番地
	ushort	*radtbl;			//	R part offset table 先頭番地
	uchar	*rhyadr;			//	R part 演奏中番地
	int		rhythmmask;			//	Rhythm音源のマスク x8c/10hのbitに対応
	int		fm_voldown;			//	FM voldown 数値
	int		ssg_voldown;		//	PSG voldown 数値
	int		pcm_voldown;		//	ADPCM voldown 数値
	int		rhythm_voldown;		//	RHYTHM voldown 数値
	int		prg_flg;			//	曲データに音色が含まれているかflag
	int		x68_flg;			//	OPM flag
	int		status;				//	status1
	int		status2;			//	status2
	int		tempo_d;			//	tempo (TIMER-B)
	int		fadeout_speed;		//	Fadeout速度
	int		fadeout_volume;		//	Fadeout音量
	int		tempo_d_push;		//	tempo (TIMER-B) / 保存用
	int		syousetu_lng;		//	小節の長さ
	int		opncount;			//	最短音符カウンタ
	int		TimerAtime;			//	TimerAカウンタ
	int		effflag;			//	PSG効果音発声on/off flag(ユーザーが代入)
	int		psnoi;				//	PSG noise周波数
	int		psnoi_last;			//	PSG noise周波数(最後に定義した数値)
	int		pcmstart;			//	PCM音色のstart値
	int		pcmstop;			//	PCM音色のstop値
	int		rshot_dat;			//	リズム音源 shot flag
	int		rdat[6];			//	リズム音源 音量/パンデータ
	int		rhyvol;				//	リズムトータルレベル
	int		kshot_dat;			//	ＳＳＧリズム shot flag
	int		play_flag;			//	play flag
	int		fade_stop_flag;		//	Fadeout後 MSTOPするかどうかのフラグ
	bool	kp_rhythm_flag;		//	K/RpartでRhythm音源を鳴らすかflag
	int		pcm_gs_flag;		//	ADPCM使用 許可フラグ (0で許可)
	int		slot_detune1;		//	FM3 Slot Detune値 slot1
	int		slot_detune2;		//	FM3 Slot Detune値 slot2
	int		slot_detune3;		//	FM3 Slot Detune値 slot3
	int		slot_detune4;		//	FM3 Slot Detune値 slot4
	int		TimerB_speed;		//	TimerBの現在値(=ff_tempoならff中)
	int		fadeout_flag;		//	内部からfoutを呼び出した時1
	int		revpan;				//	PCM86逆相flag
	int		pcm86_vol;			//	PCM86の音量をSPBに合わせるか?
	int		syousetu;			//	小節カウンタ
	int		port22h;			//	OPN-PORT 22H に最後に出力した値(hlfo)
	int		tempo_48;			//	現在のテンポ(clock=48 tの値)
	int		tempo_48_push;		//	現在のテンポ(同上/保存用)
	int		_fm_voldown;		//	FM voldown 数値 (保存用)
	int		_ssg_voldown;		//	PSG voldown 数値 (保存用)
	int		_pcm_voldown;		//	PCM voldown 数値 (保存用)
	int		_rhythm_voldown;	//	RHYTHM voldown 数値 (保存用)
	int		_pcm86_vol;			//	PCM86の音量をSPBに合わせるか? (保存用)
	int		rshot_bd;			//	リズム音源 shot inc flag (BD)
	int		rshot_sd;			//	リズム音源 shot inc flag (SD)
	int		rshot_sym;			//	リズム音源 shot inc flag (CYM)
	int		rshot_hh;			//	リズム音源 shot inc flag (HH)
	int		rshot_tom;			//	リズム音源 shot inc flag (TOM)
	int		rshot_rim;			//	リズム音源 shot inc flag (RIM)
	int		rdump_bd;			//	リズム音源 dump inc flag (BD)
	int		rdump_sd;			//	リズム音源 dump inc flag (SD)
	int		rdump_sym;			//	リズム音源 dump inc flag (CYM)
	int		rdump_hh;			//	リズム音源 dump inc flag (HH)
	int		rdump_tom;			//	リズム音源 dump inc flag (TOM)
	int		rdump_rim;			//	リズム音源 dump inc flag (RIM)
	int		ch3mode;			//	ch3 Mode
	int		ppz_voldown;		//	PPZ8 voldown 数値
	int		_ppz_voldown;		//	PPZ8 voldown 数値 (保存用)
	int		TimerAflag;			//	TimerA割り込み中？フラグ（＠不要？）
	int		TimerBflag;			//	TimerB割り込み中？フラグ（＠不要？）

	// for PMDWin
	int		rate;				//	PCM 出力周波数(11k, 22k, 44k, 55k)
	int		ppzrate;			//	PPZ 出力周波数
	bool	fmcalc55k;							// FM で 55kHz 合成をするか？
	bool	ppz8ip;								//	PPZ8 で補完するか
	bool	ppsip;								//	PPS  で補完するか
	bool	p86ip;								//	P86  で補完するか
	bool	use_p86;							//	P86  を使用しているか
	int		fadeout2_speed;						//	fadeout(高音質)speed(>0で fadeout)
	char	mus_filename[PATH_MAX];			//	曲のFILE名バッファ
	char	ppcfilename[PATH_MAX];				//	PPC のFILE名バッファ
	char	pcmdir[MAX_PCMDIR+1][PATH_MAX];	//	PCM 検索ディレクトリ
} OPEN_WORK;


//=============================================================================
//	COM 風 interface class
//=============================================================================
class IPMDWIN : public IFMPMD {
	virtual void setppsuse(bool value) = 0;
	virtual void setrhythmwithssgeffect(bool value) = 0;
	virtual void setpmd86pcmmode(bool value) = 0;
	virtual bool getpmd86pcmmode(void) = 0;
	virtual void setppsinterpolation(bool ip) = 0;
	virtual void setp86interpolation(bool ip) = 0;
	virtual int maskon(int ch) = 0;
	virtual int maskoff(int ch) = 0;
	virtual void setfmvoldown(int voldown) = 0;
	virtual void setssgvoldown(int voldown) = 0;
	virtual void setrhythmvoldown(int voldown) = 0;
	virtual void setadpcmvoldown(int voldown) = 0;
	virtual void setppzvoldown(int voldown) = 0;
	virtual int getfmvoldown(void) = 0;
	virtual int getfmvoldown2(void) = 0;
	virtual int getssgvoldown(void) = 0;
	virtual int getssgvoldown2(void) = 0;
	virtual int getrhythmvoldown(void) = 0;
	virtual int getrhythmvoldown2(void) = 0;
	virtual int getadpcmvoldown(void) = 0;
	virtual int getadpcmvoldown2(void) = 0;
	virtual int getppzvoldown(void) = 0;
	virtual int getppzvoldown2(void) = 0;
	virtual char* getmemo(char *dest, uchar *musdata, int size, int al) = 0;
	virtual char* getmemo2(char *dest, uchar *musdata, int size, int al) = 0;
	virtual char* getmemo3(char *dest, uchar *musdata, int size, int al) = 0;
	virtual int	fgetmemo(char *dest, char *filename, int al) = 0;
	virtual int	fgetmemo2(char *dest, char *filename, int al) = 0;
	virtual int	fgetmemo3(char *dest, char *filename, int al) = 0;
	virtual char* getppcfilename(char *dest) = 0;
	virtual char* getppsfilename(char *dest) = 0;
	virtual char* getp86filename(char *dest) = 0;
	virtual int ppc_load(char *filename) = 0;
	virtual int pps_load(char *filename) = 0;
	virtual int p86_load(char *filename) = 0;
	virtual int ppz_load(char *filename, int bufnum) = 0;
	virtual OPEN_WORK* getopenwork(void) = 0;
	virtual QQ* getpartwork(int ch) = 0;
};


//=============================================================================
//	PMDWin class
//=============================================================================

class PMDWIN : public IPMDWIN
{
public:
	PMDWIN();
	~PMDWIN();

	// IPCMMUSICDRIVER
	bool init(char *path);
	int music_load(char *filename);
	int music_load2(uchar *musdata, int size);
	char* getmusicfilename(char *dest);
	void music_start(void);
	void music_stop(void);
	int getloopcount(void);
	bool getlength(char *filename, int *length, int *loop);
	int getpos(void);
	void setpos(int pos);
	void getpcmdata(short *buf, int nsamples);
	
	// IFMPMD
	bool loadrhythmsample(char *path);
	bool setpcmdir(char **path);
	void setpcmrate(int rate);
	void setppzrate(int rate);
	void setfmcalc55k(bool flag);
	void setppzinterpolation(bool ip);
	void setfmwait(int nsec);
	void setssgwait(int nsec);
	void setrhythmwait(int nsec);
	void setadpcmwait(int nsec);
	void fadeout(int speed);
	void fadeout2(int speed);
	void setpos2(int pos);
	int getpos2(void);
	char* getpcmfilename(char *dest);
	char* getppzfilename(char *dest, int bufnum);
	bool getlength2(char *filename, int *length, int *loop);
	
	// IPMDWIN
	void setppsuse(bool value);
	void setrhythmwithssgeffect(bool value);
	void setpmd86pcmmode(bool value);
	bool getpmd86pcmmode(void);
	void setppsinterpolation(bool ip);
	void setp86interpolation(bool ip);
	int maskon(int ch);
	int maskoff(int ch);
	void setfmvoldown(int voldown);
	void setssgvoldown(int voldown);
	void setrhythmvoldown(int voldown);
	void setadpcmvoldown(int voldown);
	void setppzvoldown(int voldown);
	int getfmvoldown(void);
	int getfmvoldown2(void);
	int getssgvoldown(void);
	int getssgvoldown2(void);
	int getrhythmvoldown(void);
	int getrhythmvoldown2(void);
	int getadpcmvoldown(void);
	int getadpcmvoldown2(void);
	int getppzvoldown(void);
	int getppzvoldown2(void);
	char* getmemo(char *dest, uchar *musdata, int size, int al);
	char* getmemo2(char *dest, uchar *musdata, int size, int al);
	char* getmemo3(char *dest, uchar *musdata, int size, int al);
	int	fgetmemo(char *dest, char *filename, int al);
	int	fgetmemo2(char *dest, char *filename, int al);
	int	fgetmemo3(char *dest, char *filename, int al);
	char* getppcfilename(char *dest);
	char* getppsfilename(char *dest);
	char* getp86filename(char *dest);
	int ppc_load(char *filename);
	int pps_load(char *filename);
	int p86_load(char *filename);
	int ppz_load(char *filename, int bufnum);
	OPEN_WORK* getopenwork(void);
	QQ* getpartwork(int ch);
	
private:
	FM::OPNAW	opna;
	PPZ8		ppz8;
	PPSDRV		ppsdrv;
	P86DRV		p86drv;
	
	OPEN_WORK open_work;
	QQ FMPart[NumOfFMPart], SSGPart[NumOfSSGPart], ADPCMPart, RhythmPart;
	QQ ExtPart[NumOfExtPart], DummyPart, EffPart, PPZ8Part[NumOfPPZ8Part];
	
	PMDWORK pmdwork;
	EFFWORK effwork;
	
	Stereo16bit	wavbuf2[nbufsample];
	StereoSample	wavbuf[nbufsample];
	StereoSample	wavbuf_conv[nbufsample];
	
	char	*pos2;						// buf に余っているサンプルの先頭位置
	int		us2;						// buf に余っているサンプル数
	int64_t	upos;						// 演奏開始からの時間(μs)
	int64_t	fpos;						// fadeout2 開始時間
	
	uchar mdataarea[mdata_def*1024];
	uchar vdataarea[voice_def*1024];		//@不要？
	uchar edataarea[effect_def*1024];		//@不要？
	PCMEnds pcmends;

protected:
	int uRefCount;		// 参照カウンタ
	short read_short(void *value);
	ushort read_word(void *value);
	long read_long(void *value);
	int  read_char(void *value);

	void opnint_start(void);
	void data_init(void);
	void opn_init(void);
	void mstop(void);
	void mstop_f(void);
	void silence(void);
	void mstart(void);
	void mstart_f(void);
	void play_init(void);
	void setint(void);
	void calc_tb_tempo(void);
	void calc_tempo_tb(void);
	void settempo_b(void);
	void FM_Timer_main(void);
	void TimerA_main(void);
	void TimerB_main(void);
	void mmain(void);
	void syousetu_count(void);
	void fmmain(QQ *qq);
	void psgmain(QQ *qq);
	void rhythmmain(QQ *qq);
	void adpcmmain(QQ *qq);
	void pcm86main(QQ *qq);
	void ppz8main(QQ *qq);
	uchar *rhythmon(QQ *qq, uchar *bx, int al, int *result);
	void effgo(QQ *qq, int al);
	void eff_on2(QQ *qq, int al);
	void eff_main(QQ *qq, int al);
	void effplay(void);
	void efffor(const int *si);
	void effend(void);
	void effsweep(void);
	uchar *pdrswitch(QQ *qq, uchar *si);
	char* _getmemo(char *dest, uchar *musdata, int size, int al);
	char* _getmemo2(char *dest, uchar *musdata, int size, int al);
	char* _getmemo3(char *dest, uchar *musdata, int size, int al);
	int	_fgetmemo(char *dest, char *filename, int al);
	int	_fgetmemo2(char *dest, char *filename, int al);
	int	_fgetmemo3(char *dest, char *filename, int al);

	int silence_fmpart(QQ *qq);
	void keyoff(QQ *qq);
	void kof1(QQ *qq);
	void keyoffp(QQ *qq);
	void keyoffm(QQ *qq);
	void keyoff8(QQ *qq);
	void keyoffz(QQ *qq);
	int ssgdrum_check(QQ *qq, int al);
	uchar *commands(QQ *qq, uchar *si);
	uchar *commandsp(QQ *qq, uchar *si);
	uchar *commandsr(QQ *qq, uchar *si);
	uchar *commandsm(QQ *qq, uchar *si);
	uchar *commands8(QQ *qq, uchar *si);
	uchar *commandsz(QQ *qq, uchar *si);
	uchar *special_0c0h(QQ *qq, uchar *si, uchar al);
	uchar *_vd_fm(QQ *qq, uchar *si);
	uchar *_vd_ssg(QQ *qq, uchar *si);
	uchar *_vd_pcm(QQ *qq, uchar *si);
	uchar *_vd_rhythm(QQ *qq, uchar *si);
	uchar *_vd_ppz(QQ *qq, uchar *si);
	uchar *comt(uchar *si);
	uchar *comat(QQ *qq, uchar *si);
	uchar *comatm(QQ *qq, uchar *si);
	uchar *comat8(QQ *qq, uchar *si);
	uchar *comatz(QQ *qq, uchar *si);
	uchar *comstloop(QQ *qq, uchar *si);
	uchar *comedloop(QQ *qq, uchar *si);
	uchar *comexloop(QQ *qq, uchar *si);
	uchar *extend_psgenvset(QQ *qq, uchar *si);
	void lfoinit(QQ *qq, int al);
	void lfoinitp(QQ *qq, int al);
	uchar *lfoset(QQ *qq, uchar *si);
	uchar *psgenvset(QQ *qq, uchar *si);
	uchar *rhykey(uchar *si);
	uchar *rhyvs(uchar *si);
	uchar *rpnset(uchar *si);
	uchar *rmsvs(uchar *si);
	uchar *rmsvs_sft(uchar *si);
	uchar *rhyvs_sft(uchar *si);

	uchar *vol_one_up_psg(QQ *qq, uchar *si);
	uchar *vol_one_up_pcm(QQ *qq, uchar *si);
	uchar *vol_one_down(QQ *qq, uchar *si);
	uchar *portap(QQ *qq, uchar *si);
	uchar *portam(QQ *qq, uchar *si);
	uchar *portaz(QQ *qq, uchar *si);
	uchar *psgnoise_move(uchar *si);
	uchar *mdepth_count(QQ *qq, uchar *si);
	uchar *toneadr_calc(QQ *qq, int dl);
	void neiroset(QQ *qq, int dl);

	int oshift(QQ *qq, int al);
	int oshiftp(QQ *qq, int al);
	void fnumset(QQ *qq, int al);
	void fnumsetp(QQ *qq, int al);
	void fnumsetm(QQ *qq, int al);
	void fnumset8(QQ *qq, int al);
	void fnumsetz(QQ *qq, int al);
	uchar *panset(QQ *qq, uchar *si);
	uchar *panset_ex(QQ *qq, uchar *si);
	uchar *pansetm(QQ *qq, uchar *si);
	uchar *panset8(QQ *qq, uchar *si);
	uchar *pansetz(QQ *qq, uchar *si);
	uchar *pansetz_ex(QQ *qq, uchar *si);
	void panset_main(QQ *qq, int al);
	uchar calc_panout(QQ *qq);
	uchar *calc_q(QQ *qq, uchar *si);
	void fm_block_calc(int *cx, int *ax);
	int ch3_setting(QQ *qq);
	void cm_clear(int *ah, int *al);
	void ch3mode_set(QQ *qq);
	void ch3_special(QQ *qq, int ax, int cx);
	void volset(QQ *qq);
	void volsetp(QQ *qq);
	void volsetm(QQ *qq);
	void volset8(QQ *qq);
	void volsetz(QQ *qq);
	void otodasi(QQ *qq);
	void otodasip(QQ *qq);
	void otodasim(QQ *qq);
	void otodasi8(QQ *qq);
	void otodasiz(QQ *qq);
	void keyon(QQ *qq);
	void keyonp(QQ *qq);
	void keyonm(QQ *qq);
	void keyon8(QQ *qq);
	void keyonz(QQ *qq);
	int lfo(QQ *qq);
	int lfop(QQ *qq);
	uchar *lfoswitch(QQ *qq, uchar *si);
	void lfoinit_main(QQ *qq);
	void lfo_change(QQ *qq);
	void lfo_exit(QQ *qq);
	void lfin1(QQ *qq);
	void lfo_main(QQ *qq);
	void fmlfo_sub(QQ *qq, int al, int bl, uchar *vol_tbl);
	void volset_slot(int dh, int dl, int al);
	void porta_calc(QQ *qq);
	int soft_env(QQ *qq);
	int soft_env_main(QQ *qq);
	int soft_env_sub(QQ *qq);
	int ext_ssgenv_main(QQ *qq);
	void esm_sub(QQ *qq, int ah);
	void psgmsk(void);
	int get07(void);
	void md_inc(QQ *qq);
	uchar *pcmrepeat_set(QQ *qq, uchar * si);
	uchar *pcmrepeat_set8(QQ *qq, uchar * si);
	uchar *ppzrepeat_set(QQ *qq, uchar * si);
	uchar *pansetm_ex(QQ *qq, uchar * si);
	uchar *panset8_ex(QQ *qq, uchar * si);
	uchar *pcm_mml_part_mask(QQ *qq, uchar * si);
	uchar *pcm_mml_part_mask8(QQ *qq, uchar * si);
	uchar *ppz_mml_part_mask(QQ *qq, uchar * si);
	void pcmstore(ushort pcmstart, ushort pcmstop, uchar *buf);
	void pcmread(ushort pcmstart, ushort pcmstop, uchar *buf);

	uchar *hlfo_set(QQ *qq, uchar *si);
	uchar *vol_one_up_fm(QQ *qq, uchar *si);
	uchar *porta(QQ *qq, uchar *si);
	uchar *slotmask_set(QQ *qq, uchar *si);
	uchar *slotdetune_set(QQ *qq, uchar *si);
	uchar *slotdetune_set2(QQ *qq, uchar *si);
	void fm3_partinit(QQ *qq, uchar *ax);
	uchar *fm3_extpartset(QQ *qq, uchar *si);
	uchar *ppz_extpartset(QQ *qq, uchar *si);
	uchar *volmask_set(QQ *qq, uchar *si);
	uchar *fm_mml_part_mask(QQ *qq, uchar *si);
	uchar *ssg_mml_part_mask(QQ *qq, uchar *si);
	uchar *rhythm_mml_part_mask(QQ *qq, uchar *si);
	uchar *_lfoswitch(QQ *qq, uchar *si);
	uchar *_volmask_set(QQ *qq, uchar *si);
	uchar *tl_set(QQ *qq, uchar *si);
	uchar *fb_set(QQ *qq, uchar *si);
	uchar *fm_efct_set(QQ *qq, uchar *si);
	uchar *ssg_efct_set(QQ *qq, uchar *si);
	void fout(void);
	void neiro_reset(QQ *qq);
	int music_load3(uchar *musdata, int size, char *current_dir);
	int ppc_load2(char *filename);
	int ppc_load3(uchar *pcmdata, int size);
	char *search_pcm(char *dest, const char *filename, const char *current_dir);
	void swap(int *a, int *b);
};


//============================================================================
//	DLL Export Functions
//============================================================================

#ifdef __cplusplus
extern "C" {
#endif

int getversion(void);
int getinterfaceversion(void);
bool pmdwininit(char *path);
bool loadrhythmsample(char *path);
bool setpcmdir(char **path);
void setpcmrate(int rate);
void setppzrate(int rate);
void setppsuse(bool value);
void setrhythmwithssgeffect(bool value);
void setpmd86pcmmode(bool value);
bool getpmd86pcmmode(void);
int music_load(char *filename);
int music_load2(uchar *musdata, int size);
void music_start(void);
void music_stop(void);
void fadeout(int speed);
void fadeout2(int speed);
void getpcmdata(short *buf, int nsamples);
void setfmcalc55k(bool flag);
void setppsinterpolation(bool ip);
void setp86interpolation(bool ip);
void setppzinterpolation(bool ip);
char * getmemo(char *dest, uchar *musdata, int size, int al);
char * getmemo2(char *dest, uchar *musdata, int size, int al);
char * getmemo3(char *dest, uchar *musdata, int size, int al);
int fgetmemo(char *dest, char *filename, int al);
int fgetmemo2(char *dest, char *filename, int al);
int fgetmemo3(char *dest, char *filename, int al);
char * getmusicfilename(char *dest);
char * getpcmfilename(char *dest);
char * getppcfilename(char *dest);
char * getppsfilename(char *dest);
char * getp86filename(char *dest);
char * getppzfilename(char *dest, int bufnum);
int ppc_load(char *filename);
int pps_load(char *filename);
int p86_load(char *filename);
int ppz_load(char *filename, int bufnum);
int maskon(int ch);
int maskoff(int ch);
void setfmvoldown(int voldown);
void setssgvoldown(int voldown);
void setrhythmvoldown(int voldown);
void setadpcmvoldown(int voldown);
void setppzvoldown(int voldown);
int getfmvoldown(void);
int getfmvoldown2(void);
int getssgvoldown(void);
int getssgvoldown2(void);
int getrhythmvoldown(void);
int getrhythmvoldown2(void);
int getadpcmvoldown(void);
int getadpcmvoldown2(void);
int getppzvoldown(void);
int getppzvoldown2(void);
void setpos(int pos);
void setpos2(int pos);
int getpos(void);
int getpos2(void);
bool getlength(char *filename, int *length, int *loop);
bool getlength2(char *filename, int *length, int *loop);
int getloopcount(void);
void setfmwait(int nsec);
void setssgwait(int nsec);
void setrhythmwait(int nsec);
void setadpcmwait(int nsec);
OPEN_WORK * getopenwork(void);
QQ * getpartwork(int ch);

#ifdef __cplusplus
}
#endif


#endif // PMDWIN_H
