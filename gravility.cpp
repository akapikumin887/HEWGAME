#include "gravility.h"

D3DXVECTOR3 Gravility::g;

// Gravilityの初期化
void Gravility::Initialize()
{
	g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
}

// Gravilityの終了処理
void Gravility::Finalize()
{

}

// Gravilityの更新
void Gravility::Update()
{

}

// Gravilityの描画
void Gravility::Draw()
{

}