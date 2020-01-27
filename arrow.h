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
#include "xmodel.h"
#include "gravility.h"
#include "wind.h"
#include "ui.h"
#include "score.h"

#define ARROWXMODEL "asset/xmodel/bow.x"

#define ARROW_SIZE_X 0.025f
#define ARROW_SIZE_Y 0.025f
#define ARROW_SIZE_Z 5.0f

#define ARROW_POS_X 0.0f
#define ARROW_POS_Y 0.0f
#define ARROW_POS_Z -20.0f
#define ARROW_POS_Z_MAX 1000.0f

#define ARROW_ROT_X 0.0f
#define ARROW_ROT_Y 0.0f
#define ARROW_ROT_Z 0.0f

#define ARROW_MAX 5
#define ARROW_MOVE_SPEED 8.0f

enum ArrowState {
	ARROW_STATE_NONE,
	ARROW_STATE_WAIT_ZOOM,
	ARROW_STATE_PREPARE,
	ARROW_STATE_FLYING,
	ARROW_STATE_HIT_TARGET,
	ARROW_STATE_HIT_PLANE,
	ARROW_STATE_OVER_FLYING,
};

class BowXModel
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	XModel       *xmodel;       // XModel

	D3DXVECTOR3 posAiming; // 発射時のAimingの位置情報
	D3DXVECTOR3 direction; // 向き
	D3DXVECTOR3 move;      // 移動量

	static ArrowState state; // BowXModelの状態

	BowXModel();  // BowXModelの初期化（コンストラクタ）
	~BowXModel(); // BowXModelの終了処理（デストラクタ）

	void Initialize(const char *filepass, TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r); // BowXModelの初期化
	void Finalize();   // BowXModelの終了処理
	void Update();     // BowXModelの更新
	void Draw();       // BowXModelの描画

	void Aiming_Direction(); // BowXModelの向き
};

class Arrow
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	Cube         *cube;         // Cube

	D3DXVECTOR3  posOld;        // 旧位置
	D3DXVECTOR3  posAiming;     // 発射時のAimingの位置情報
	D3DXVECTOR3  posHead;       // 先頭位置
	D3DXVECTOR3  posHit;        // 的での刺し位置
	D3DXVECTOR3  direction;     // Arrowの向き
	D3DXVECTOR3  move;          // Arrowの移動量

	ArrowState   state;         // Arrowの状態

	Gravility    *gravility;    // 重力
	Wind         *wind;         // 風

	bool         bUse;          // 使用フラグ
	bool         Display;       // 表示フラグ

	float        targetLen;     // 矢の先端位置と的の中心位置との距離
	int          score;         // Score

	static float speed;         // 速度

	Arrow();  // Arrowの初期化（コンストラクタ）
	~Arrow(); // Arrowの終了処理（デストラクタ）

	void Initialize(); // Arrowの初期化
	void Finalize(); // Arrowの終了処理
	void Update();   // Arrowの更新
	void Draw();     // Arrowの描画

	void Aiming_Direction(); // Arrowの発射時の向き
	void Move_Direction();   // Arrowの移動時の向き
	void Movement();  // Arrowの移動
	void HitCheck();  // 当たり判定
	int  Get_Score(); // Scoreの取得
};

class ArrowManager
{
public:
	static Arrow *arrows; // Arrowの配列
	static int   num;     // Arrowの数
	static int   cnt;     // 矢の残数
	static int   score_t; // Score

	static void   Initialize(int n); // 全てのArrowの初期化
	static void   Finalize();        // 全てのArrowの終了処理
	static void   Update();          // 全てのArrowの更新
	static void   Draw();            // 全てのArrowの描画
	static void   Add_Arrow();       // Arrowの生成
	static Arrow* Get_Arrow();       // Arrow情報の取得
};