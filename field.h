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

#define FIELD_MAX 10

class Field
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	Face         *face;         // Face

	Field();  // Fieldの初期化（コンストラクタ）
	~Field(); // Fieldの終了処理（デストラクタ）

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz); // Fieldの初期化
	void Finalize(); // Fieldの終了処理
	void Update();   // Fieldの更新
	void Draw();     // Fieldの描画
};