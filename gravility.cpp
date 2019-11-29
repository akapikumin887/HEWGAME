#include "gravility.h"

static Gravility gravility;

// Gravility‚Ì‰Šú‰»
void Gravility_Initialize()
{
	gravility.g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
}

// Gravility‚ÌI—¹ˆ—
void Gravility_Finalize()
{

}

// Gravility‚ÌXV
void Gravility_Update()
{

}

// Gravility‚Ì•`‰æ
void Gravility_Draw()
{

}

// Gravilityî•ñ‚Ìæ“¾
Gravility* Get_Gravility()
{
	return &gravility;
}