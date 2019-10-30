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

#ifndef TITLE_H_
#define TITLE_H_

class Title
{
public:
	bool bUse;                 // 使用中フラグ
	D3DXVECTOR3 pos;           // 表示座標
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ

	Title();
	~Title();
};

void Title_Initialize(); // Titleの初期化
void Title_Finalize(); // Titleの終了処理
void Title_Update(); // Titleの更新
void Title_Draw(); // Titleの描画
#endif