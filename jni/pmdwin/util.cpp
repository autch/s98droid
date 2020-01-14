//=============================================================================
//			Utility Functions
//				Programmed by C60
//=============================================================================

#include	"util.h"
#include 	<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int ismbblead(uint8_t c)
{
	return (c >= 0x81 && c <= 0x9f) || (c >= 0xe0 && c <= 0xfc);
}


//=============================================================================
//	ファイル名で示されたファイルのサイズを取得する
//=============================================================================
int64_t GetFileSize_s(char *filename)
{
	int ret;
	struct stat st;


	ret = stat(filename, &st);
	if(ret < 0)
		return -1;

	return st.st_size;
}


//=============================================================================
//	TAB を SPACE に変換(tabcolumn カラム毎 / ESCシーケンス不可)
//=============================================================================
char *tab2spc(char *dest, const char *src, int tabcolumn)
{
	int		column = 0;
	char	*dest2;

	dest2 = dest;
	while(*src != '\0') {
		if(*src == '\t') {							// TAB
			src++;
			do {
				*dest++ = ' ';
			}while(++column % tabcolumn);
		} else if(*src == 0x0d || *src == 0x0a) {	// 改行
			column = 0;
			*dest++ = *src++;
		} else {
			*dest++ = *src++;
			column++;
		}
	}
	*dest = '\0';
	return dest2;
}


//=============================================================================
//	エスケープシーケンスの除去
//	カーソル移動系エスケープシーケンスには非対応
//=============================================================================
char *delesc(char *dest, const char *src)
{
	char	*dest2;
	uchar	*src2;
	uchar	*src3;
	size_t	i;
	
	if((src2 = src3 = (uchar *)malloc(strlen(src) + 32)) == NULL) {
		return NULL;
	};

	strcpy((char *)src2, src);

	// 31バイト連続 '\0' にする(8 以上なら OK なはずだけど，念のため）
	for(i = strlen((char *)src2); i < strlen((char *)src2) + 31; i++) {
		src2[i] = '\0';
	}

	dest2 = dest;

	while(*src2 != '\0') {
		if(*src2 == 0x1b) {		// エスケープシーケンス
			if(*(src2 + 1) == '[') {
				src2 += 2;
				while(*src2 && (toupper(*src2) < 'A' || toupper(*src2) > 'Z')) {
					if(ismbblead(*src2)) {
						src2 += 2;
						continue;
					}
					src2++;
				}
				src2++;
			} else if(*(src2 + 1) == '=') {
				src2 += 4;
			} else if(*(src2 + 1) == ')' || *(src2 + 1) == '!') {
				src2 += 3;
			} else {
				src2 += 2;
			}
		} else {
			*dest++ = *src2++;		
		}
	}

	free(src3);
	*dest = '\0';
	return dest2;
}
