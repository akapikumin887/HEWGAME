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

#ifndef Result_H_
#define Result_H_

class Result
{
public:
	bool bUse;                 // 使用中フラグ
	D3DXVECTOR3	pos;	       //表示座標
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ

	Result();
	~Result();
};

void Result_Initialize(); // Resultの初期化
void Result_Finalize(); // Resultの終了処理
void Result_Update(); // Resultの更新
void Result_Draw(); // Resultの描画
#endif