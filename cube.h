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

#ifndef CUBE_H_
#define CUBE_H_

#define CUBE_MAX 5
#define MOVE_SPEED 10.0f
#define CUBE_X AIMING_X
#define CUBE_Y 3.5f
#define CUBE_Z AIMING_Z
#define CUBE_MAG_Z 3.5f

class Cube
{
public:
	bool bUse;                  // 使用フラグ
	bool bShotted;              // 発射フラグ
	bool nShotted;              // 時間制限で未発射フラグ
	bool bHit;                  // 命中フラグ
	VERTEX_3D *cubev;           // 頂点情報
	TextureIndex texture_index; // テクスチャ
	D3DXVECTOR3 pos;            // 現在位置
	D3DXVECTOR3 posOld;         // 旧位置
	D3DXVECTOR3 posHead;        // 先頭位置
	D3DXVECTOR3 posHit;         // 的での刺し位置
	D3DXVECTOR3 posAiming;      // 発射時のAimingの位置情報
	D3DXVECTOR3 rot;            // 回転
	D3DXVECTOR3 scl;            // 大きさ
	bool revolution;            // 公転フラグ
	D3DXVECTOR3 revRadius;      // 公転半径
	D3DXVECTOR3 revSpd;         // 公転速度
	D3DXVECTOR3 direction;      // Cubeの向き
	D3DXVECTOR3 move;           // Cubeの移動量
	static int cnt;             // 矢の残数
	float targetLen;            // 矢の先端位置と的の中心位置との距離

	Cube(); // Cubeの頂点情報取得（コンストラクタ）
	~Cube(); // Cubeの頂点情報のリリース（デストラクタ）

	void Draw_Cube(); // Cubeの描画
	void Set_Cube(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Cubeのテクスチャ・位置・回転・大きさの設定
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Cubeの公転フラグ・公転半径・公転速度の設定
	void Cube_Aiming_Direction(); // Cubeの発射時の向き
	//void Cube_Move_Direction(D3DXVECTOR3 wind,float g = GRAVILITY / GRAVILITY_FORCE_MAG); // Cubeの発射時の向き
	void Cube_Move_Direction(); // Cubeの発射時の向き
	float Rotation_Correction(float r); // Cubeの回転の補正
	float Get_Length(D3DXVECTOR3 l); // 長さの計算
	void Score(); // Score判定
};

void Cube_Initialize(); // Cubeの初期化
void Cube_Finalize(); // Cubeの終了処理
void Cube_Update(); // Cubeの更新
void Cube_Draw(); // Cubeの描画
void Add_Cube(); // Cubeの追加
Cube* Get_Cube(); // Cube情報の取得
#endif