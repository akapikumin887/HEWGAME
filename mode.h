#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef MODE_H_
#define MODE_H_

typedef enum {
	MODE_NONE,
	MODE_EASY,
	MODE_HARD
}ModeIndex;

class Mode
{
public:
	bool bUse;                 // 使用中フラグ
	D3DXVECTOR3 pos;           // 表示座標
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ

	Mode();
	~Mode();
};

void Mode_Initialize(); // Modeの初期化
void Mode_Finalize(); // Modeの終了処理
void Mode_Update(); // Modeの更新
void Mode_Draw(); // Modeの描画
ModeIndex GetMode(); // Mode情報の取得
#endif