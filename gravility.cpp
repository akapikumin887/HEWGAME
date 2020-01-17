#include "gravility.h"

D3DXVECTOR3 Gravility::g;

// Gravility‚Ì‰Šú‰»
void Gravility::Initialize()
{
	g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
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