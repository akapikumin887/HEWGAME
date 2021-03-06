#include "gravility.h"
#include "mode.h"

D3DXVECTOR3 Gravility::g;

// Gravilityの初期化
void Gravility::Initialize()
{
	if (Get_Mode() != MODE_EASY)
	{
		g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
	}
	else
	{
		g = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
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