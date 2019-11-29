#include "wind.h"
#include "debug_font.h"
#include "cube.h"

static Wind wind;
static int cnt = CUBE_MAX;

// Wind‚Ì‰Šú‰»
void Wind_Initialize()
{
	wind.speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f) / WIND_FORCE_MAG;
}

// Wind‚ÌI—¹ˆ—
void Wind_Finalize()
{

}

// Wind‚ÌXV
void Wind_Update()
{
	Cube *cube = Get_Cube();
	if (cnt != Cube::cnt)
	{
		wind.speed = D3DXVECTOR3(((rand() % 16) * 1.0f + 5.0f) / 2, 0.0f, 0.0f) / WIND_FORCE_MAG;
		if (rand() % 2 == 0)
		{
			wind.speed *= -1.0f;
		}
	}
	cnt = Cube::cnt;
}

// Wind‚Ì•`‰æ
void Wind_Draw()
{
	DebugFont_Draw(640, 62, "wind: %.01lf", wind.speed.x * WIND_FORCE_MAG);
}

Wind* Get_Wind()
{
	return &wind;
}