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

#ifndef UI_H_
#define UI_H_

#define SCORE_DIGIT_MAX 3
#define TIME_DIGIT_MAX 5
#define DIGIT_MAX 5

class UI
{
public:
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	D3DXVECTOR3 pos;
	int tx, ty;
	int tw, th;
};

class Number :public UI
{
public:
	int n;
	int n_tmp;

	Number();
};

class Alphabet :public UI
{
public:
	int len;

	Alphabet();
};

void UI_Initialize(); // UIの初期化
void UI_Finalize(); // UIの終了処理
void UI_Update(); // UIの更新
void UI_Draw(); // UIの描画
#endif