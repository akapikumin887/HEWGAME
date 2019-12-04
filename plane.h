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

#ifndef PLANE_H_
#define PLANE_H_

class Plane
{
public:
	VERTEX_3D *planev;          // 頂点情報
	TextureIndex texture_index; // テクスチャ
	D3DXVECTOR3 size;           // サイズ
	D3DXVECTOR3 size_nor;       // 単位サイズ
	D3DXVECTOR3 pos;            // 位置
	D3DXVECTOR3 rot;            // 回転
	D3DXVECTOR3 scl;            // 大きさ
	bool revolution;            // 公転フラグ
	D3DXVECTOR3 revRadius;      // 公転半径
	D3DXVECTOR3 revSpd;         // 公転速度

	Plane(); // Planeの頂点情報取得（コンストラクタ）
	~Plane(); // Planeの頂点情報のリリース（デストラクタ）

	void Draw_Plane(); // Planeの描画
	void Set_Plane(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, // Planeのテクスチャ・位置・回転・大きさの設定
		D3DXVECTOR3 sz, D3DXVECTOR3 szn, // Planeのサイズ・単位サイズの設定
		bool Revolution = false, D3DXVECTOR3 RevRadius = D3DXVECTOR3(0, 0, 0), D3DXVECTOR3 RevSpd = D3DXVECTOR3(0, 0, 0)); // Planeの公転フラグ・公転半径・公転速度の設定
	float Rotation_Correction(float r); // Planeの回転の補正
};

void Plane_Initialize(); // Planeの初期化
void Plane_Finalize(); // Planeの終了処理
void Plane_Update(); // Planeの更新
void Plane_Draw(); // Planeの描画
#endif