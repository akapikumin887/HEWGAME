#include "gravility.h"
#include "mode.h"

D3DXVECTOR3 Gravility::g;

// Gravility‚Ì‰Šú‰»
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

// Gravility‚ÌI—¹ˆ—
void Gravility::Finalize()
{

}

// Gravility‚ÌXV
void Gravility::Update()
{

}

// Gravility‚Ì•`‰æ
void Gravility::Draw()
{

}