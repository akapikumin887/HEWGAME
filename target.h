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

#ifndef TARGET_H_
#define TARGET_H_

#define TARGET_X AIMING_X
#define TARGET_Y 3.5f
#define TARGET_Z AIMING_Z
#define TARGET_MAG_Z AIMING_MAG_Z

class Target
{
public:
	VERTEX_3D *targetv;         // 頂点情報
	D3DXVECTOR3 pos;            // 位置
	D3DXVECTOR3 rot;            // 回転
	D3DXVECTOR3 scl;            // 大きさ
	TextureIndex texture_index; // テクスチャ
	bool revolution;            // 公転フラグ
	D3DXVECTOR3 revRadius;      // 公転半径
	D3DXVECTOR3 revSpd;         // 公転速度

	Target(); // Targetの頂点情報取得（コンストラクタ）
	~Target(); // Targetの頂点情報のリリース（デストラクタ）

	void Draw_Target(); // Targetの描画
	void Set_Target(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Targetのテクスチャ・位置・回転・大きさの設定
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Targetの公転フラグ・公転半径・公転速度の設定
	float Rotation_Correction(float r); // Targetの回転の補正
};

void Target_Initialize(); // Targetの初期化
void Target_Finalize(); // Targetの終了処理
void Target_Update(); // Targetの更新
void Target_Draw(); // Targetの描画
Target* Get_Target(); // Target情報の取得
#endif