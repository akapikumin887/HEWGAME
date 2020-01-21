#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <string>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#define SCORE_DIGIT_MAX 3
#define TIME_DIGIT_MAX 4
#define DIGIT_MAX 5

using std::string;

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
		int t_x = 0, int t_y = 0,                        // t_x, t_yは切り取り開始位置(デフォルト値は0: テクスチャ左上から切り取る)
		int t_w = 1, int t_h = 1,                        // t_w, t_hは切り取り単位(デフォルト値は1: テクスチャの右下まで切り取る)
		D3DCOLOR c = D3DCOLOR_RGBA(255, 255, 255, 255)); // 色の設定
	virtual void Finalize();           // UIの終了処理
	virtual void Update();             // UIの更新
	virtual void Draw(float r = 0.0f); // UIの描画
};

class Number :public UI
{
public:
	int digit_i; // 数字の位(整数部)
	int digit_d; // 数字の位(小数部)

	// Numberの初期化
	virtual void Initialize(TextureIndex tex_idx,        // TextureIndexの設定
		D3DXVECTOR2 s, D3DXVECTOR2 p,                    // サイズ・最後尾の位置の設定
		int t_x = 0, int t_y = 0,                        // t_x, t_yは切り取り開始位置(デフォルト値は0: テクスチャ左上から切り取る)
		int t_w = 1, int t_h = 1,                        // t_w, t_hは切り取り単位(デフォルト値は1: テクスチャの右下まで切り取る)
		int di = 1, int dd = 0,                          // 数字の位（整数部・小数部)
		D3DCOLOR c = D3DCOLOR_RGBA(255, 255, 255, 255)); // 色の設定
	void Draw_Integer(double n, int di, int dd = 0, float r = 0.0f); // Numberの描画
};

class Alphabet :public UI
{
public:
	string word; // 文字列

	// Alphabetの初期化
	void Initialize(TextureIndex tex_idx,                // TextureIndexの設定
		string w,                                        // 文字列
		D3DXVECTOR2 s, D3DXVECTOR2 p,                    // サイズ・最後尾の位置の設定
		int t_x = 0, int t_y = 0,                        // t_x, t_yは切り取り開始位置(デフォルト値は0: テクスチャ左上から切り取る)
		int t_w = 1, int t_h = 1,                        // t_w, t_hは切り取り単位(デフォルト値は1: テクスチャの右下まで切り取る)
		int blank = 0,                                   // 最後尾に入れる空白数
		D3DCOLOR c = D3DCOLOR_RGBA(255, 255, 255, 255)); // 色の設定
	void Draw(float r = 0.0f); // Alphabetの描画
};