//====================================================
// サウンド処理 [sound.h]
//====================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include <Windows.h>

//====================================================
// サウンドファイル
//====================================================
typedef enum
{
	//SOUND_LABEL_BGM000 = 0,		    // BGM0
	SOUND_LABEL_SE_SHOT00,		    // 弓発射音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//====================================================
// プロトタイプ宣言
//====================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
#endif