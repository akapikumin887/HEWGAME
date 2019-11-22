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

#ifndef AIMING_H_
#define AIMING_H_

#define AIMING_X 0.0f
#define AIMING_Y 0.0f
#define AIMING_Z 5.0f
#define AIMING_MAG_Z 6.0f
#define CHARGE_SPAN 50
#define TIME_COUNT_MAX 10.0f

class Aiming
{
public:
	VERTEX_3D *aimingv;         // 頂点情報
	Time *timec;                // 時間情報
	D3DXVECTOR3 pos;            // 位置
	D3DXVECTOR3 rot;            // 回転
	D3DXVECTOR3 scl;            // 大きさ
	TextureIndex texture_index; // テクスチャ
	bool revolution;            // 公転フラグ
	D3DXVECTOR3 revRadius;      // 公転半径
	D3DXVECTOR3 revSpd;         // 公転速度
	bool isAiming;              // 照準フラグ
	bool prepare;               // 構えフラグ
	bool isShot;                // 発射フラグ
	int charge_span;            // チャージ間隔

	Aiming(); // Aimingの頂点情報取得（コンストラクタ）
	~Aiming(); // Aimingの頂点情報のリリース（デストラクタ）

	void Draw_Aiming(); // Aimingの描画
	void Set_Aiming(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Aimingのテクスチャ・位置・向き（回転）・大きさの設定
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Aimingの公転フラグ・公転半径・公転速度の設定
	float Rotation_Correction(float r); // Aimingの回転の補正
	void TimeCount_On(); // 時間をカウント
};

void Aiming_Initialize(); // Aimingの初期化
void Aiming_Finalize(); // Aimingの終了処理
void Aiming_Update(); // Aimingの更新
void Aiming_Draw(); // Aimingの描画
Aiming* Get_Aiming(); // Aiming情報の取得
#endif