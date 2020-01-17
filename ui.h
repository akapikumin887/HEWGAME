#pragma once
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
	TextureIndex texture_index; // テクスチャID
	D3DXVECTOR2  size;          // ポリゴンのサイズ
	D3DXVECTOR2  pos;           // 現在位置
	int          tx, ty;
	int          tw, th;
	D3DCOLOR     color;	        // カラー

	// UIの初期化
	virtual void Initialize(TextureIndex tex_idx,        // TextureIndexの設定
		D3DXVECTOR2 s, D3DXVECTOR2 p,                    // サイズ・位置の設定
		int t_x = 0, int t_y = 0,                        // t_x, t_y切り取り開始位置(デフォルト値は0: テクスチャ左上から切り取る)
		int t_w = 1, int t_h = 1,                        // t_w, t_h切り取り単位(デフォルト値は1: テクスチャの右下まで切り取る)
		D3DCOLOR c = D3DCOLOR_RGBA(255, 255, 255, 255)); // 色の設定
	virtual void Finalize();   // UIの終了処理
	virtual void Update();     // UIの更新
	virtual void Draw();       // UIの描画
};

class Number :public UI
{
public:
	int n;

	Number();
};

class Alphabet :public UI
{
public:
	int  len;
	char word[8];

	Alphabet();
};
#endif