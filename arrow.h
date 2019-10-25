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

#ifndef ARROW_H_
#define ARROW_H_

#define ARROW_MAX 5
#define ARROW_X SCREEN_WIDTH *0.5f
#define ARROW_Y SCREEN_HEIGHT * 0.75f
#define ARROW_SPEED 0.5f

class Arrow
{
public:
	bool bUse;                 // 使用中フラグ
	bool beShotted;            // 発射フラグ
	D3DXVECTOR3 pos;           // 表示座標
	D3DXVECTOR3 direction;     // 矢の向き
	D3DXVECTOR3 speed;         // 矢の速度
	D3DXVECTOR3 degree;        // 発射角度
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ

	Arrow();
	~Arrow();
	void Arrow_Direction_Normalize(); // 向きベクトルの取得と正規化
};

void Arrow_Initialize(); // ARROWの初期化
void Arrow_Finalize(); // ARROWの終了処理
void Arrow_Update(); // ARROWの更新
void Arrow_Draw(); // ARROWの描画
void Create_Arrow(); // ARROWの作成
Arrow* GetArrow(); // ARROWの情報取得
#endif