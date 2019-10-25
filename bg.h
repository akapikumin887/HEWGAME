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

#ifndef BG_H_
#define BG_H_

class BG
{
public:
	bool bUse;                 // 使用中フラグ
	D3DXVECTOR3 pos;           // 表示座標
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ

	BG();
	~BG();
};

void BG_Initialize(); // BGの初期化
void BG_Finalize(); // BGの終了処理
void BG_Update(); // BGの更新
void BG_Draw(); // BGの描画
#endif
