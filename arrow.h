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
#define ARROW_X SCREEN_WIDTH * 0.5f
#define ARROW_Y SCREEN_HEIGHT * 0.9f
#define ARROW_SPEED 0.5f

class Arrow
{
public:
	bool bUse;                 // 使用中フラグ
	bool beShotted;            // 発射フラグ
	bool hit;                  // 命中フラグ
	D3DXVECTOR3 posTail;       // 矢の尾の座標
	D3DXVECTOR3 posHead;       // 矢の先頭の座標
	D3DXVECTOR3 direction;     // 矢の向き
	D3DXVECTOR3 move;          // 矢の移動量
	D3DXVECTOR3 degree;        // 発射角度
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ
	int charge;                // 矢に与えられた力
	static int cnt;            // 矢の数のカウンタ
	static int shot_cnt;       // 発射カウント

	Arrow();
	~Arrow();
	void Arrow_Direction_Normalize(); // 向きベクトルの取得と正規化
	void Arrow_Head_Pos(); // 矢の先頭座標の取得
	static void Print(); // 残りの本数の表示
};

void Arrow_Initialize(); // ARROWの初期化
void Arrow_Finalize(); // ARROWの終了処理
void Arrow_Update(); // ARROWの更新
void Arrow_Draw(); // ARROWの描画
void Create_Arrow(); // ARROWの生成
Arrow* GetArrow(); // ARROWの情報取得
#endif