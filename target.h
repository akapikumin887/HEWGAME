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

#define TARGET_X 0.0f
#define TARGET_Y 0.0f
#define TARGET_Z 23.0f
#define TARGET_MAG_Z 6.0f
#define TARGET_SIZE 5.0f

class Target
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	Face         *face;         // Face
	float        circle[11];    // 輪の数

	Target();  // Targetの初期化（コンストラクタ）
	~Target(); // Targetの終了処理（デストラクタ）

	void Initialize(TextureIndex tex_idx = TEXTURE_INDEX_TARGET, D3DXVECTOR3 p = D3DXVECTOR3(TARGET_X, TARGET_Y, TARGET_Z), D3DXVECTOR3 r = D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3 sz = D3DXVECTOR3(TARGET_SIZE, 0.0f, TARGET_SIZE)); // Targetの初期化
	void Finalize(); // Targetの終了処理
	void Update();   // Targetの更新
	void Draw();     // Targetの描画
};