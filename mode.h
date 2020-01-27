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

#define MODE_OBJECT_MAX 5

enum MODE {
	MODE_NONE,
	MODE_EASY,
	MODE_NORMAL,
	MODE_HARD,
};

class Mode
{
public:
	TextureIndex texture_index; // テクスチャインデックス
	D3DXVECTOR3  pos;           // 現在位置
	D3DXVECTOR3  rot;           // 回転
	D3DXMATRIX   mtxWorld;      // ワールドマトリックス
	Face         *face;         // Face

	Mode();  // Modeの初期化（コンストラクタ）
	~Mode(); // Modeの終了処理（デストラクタ）

	void Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz); // Modeの初期化
	void Finalize(); // Modeの終了処理
	void Update();   // Modeの更新
	void Draw();     // Modeの描画
};

void Mode_Initialize();
void Mode_Finalize();
void Mode_Update();
void Mode_Draw();
MODE Get_Mode();
void Set_Mode(MODE m);