#include "gravility.h"

static Gravility gravility;

// Gravilityの初期化
void Gravility_Initialize()
{
	gravility.g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
}

// Gravilityの終了処理
void Gravility_Finalize()
{

}

// Gravilityの更新
void Gravility_Update()
{

}

// Gravilityの描画
void Gravility_Draw()
{

}

// Gravility情報の取得
Gravility* Get_Gravility()
{
	return &gravility;
}