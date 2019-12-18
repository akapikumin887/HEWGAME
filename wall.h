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

#ifndef WALL_H_
#define WALL_H_

#define WALLMOVE 1.0f

class Wall
{
public:
	VERTEX_3D *wallv;            // 頂点情報
	TextureIndex texture_index; // テクスチャ
	D3DXVECTOR3 pos;            // 位置
	D3DXVECTOR3 rot;            // 回転
	D3DXVECTOR3 scl;            // 大きさ
	bool revolution;            // 公転フラグ
	D3DXVECTOR3 revRadius;      // 公転半径
	D3DXVECTOR3 revSpd;         // 公転速度

	Wall(); // Wallの頂点情報取得（コンストラクタ）
	~Wall(); // Wallの頂点情報のリリース（デストラクタ）

	void Draw_Wall(); // Wallの描画
	void Set_Wall(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Wallのテクスチャ・位置・回転・大きさの設定
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Wallの公転フラグ・公転半径・公転速度の設定
	float Rotation_Correction(float r); // Wallの回転の補正
};

void Wall_Initialize(); // Wallの初期化
void Wall_Finalize(); // Wallの終了処理
void Wall_Update(); // Wallの更新
void Wall_Draw(); // Wallの描画
#endif