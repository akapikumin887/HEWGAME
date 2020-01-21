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

class Plane :public FaceEX
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	FaceEX       *faceEX;       // FaceEX

	Plane();  // Planeの初期化（コンストラクタ）
	~Plane(); // Planeの終了処理（デストラクタ）

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz, D3DXVECTOR3 szn); // Planeの初期化
	void Finalize(); // Planeの終了処理
	void Update();   // Planeの更新
	void Draw();     // Planeの描画
};