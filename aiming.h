#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "ui.h"

#ifndef AIMING_H_
#define AIMING_H_

#define TIME_COUNT_MAX 15.0f

enum AimingState {
	AIMING_STATE_NONE,
	AIMING_STATE_FREE,
	AIMING_STATE_PREPARE,
	AIMING_STATE_ZOOM_FORWARD,
	AIMING_STATE_ZOOM_BACKWARD,
	AIMING_STATE_CHECK_ARROW,
};

class Aiming2D
{
public:
	TextureIndex  texture_index; // テクスチャインデックス
	D3DXVECTOR2   pos;           // 現在位置
	D3DCOLOR      color;	     // カラー
	int           tx, ty;
	int           tw, th;

	D3DXVECTOR3   posHit; // Cameraから出される射線が当たった座標
	Number        *num;   // 時間表示
	Alphabet      *alpha; // 文字表示
	Timer         *timer; // Timer
	float         zoomStart; // Zoom開始位置

	AimingState   state;   // Aimingの状態

	static double timeCnt; // カウントダウン

	Aiming2D();  // Aiming2Dの初期化（コンストラクタ）
	~Aiming2D(); // Aiming2Dの終了処理（デストラクタ）

	void Initialize(); // Aiming2Dの初期化
	void Finalize();   // Aiming2Dの終了処理
	void Update();     // Aiming2Dの更新
	void Draw();       // Aiming2Dの描画
	void Draw_Timer(); // Timerの表示

	void Free();          // フリー状態
	void Prepare();       // 構え状態
	void Zoom_Forward(float start, float zm = ZOOM_MAX, float zi = ZOOM_INCREASING);  // 前進Zoom待ち
	void Zoom_Backward(float start, float zm = ZOOM_MAX, float zi = ZOOM_INCREASING); // 後退Zoom待ち
};

class Aiming3D
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXVECTOR3  rot_Camera;    // カメラ回転の補正
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	Face         *face;         // Face

	D3DXVECTOR3  posHit; // Cameraから出される射線が当たった座標

	bool         prepare; // 構えフラグ

	Aiming3D();  // Aiming3Dの初期化（コンストラクタ）
	~Aiming3D(); // Aiming3Dの終了処理（デストラクタ）

	void Initialize(); // Aiming3Dの初期化
	void Finalize();   // Aiming3Dの終了処理
	void Update();     // Aiming3Dの更新
	void Draw();       // Aiming3Dの描画

	D3DXVECTOR3 Pos_With_Camera(); // Cameraによる位置
};
#endif