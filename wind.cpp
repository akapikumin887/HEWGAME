#include "wind.h"
#include "debug_font.h"
#include "cube.h"

static Wind wind;
static int cnt = CUBE_MAX;

// Windの初期化
void Wind_Initialize()
{
	wind.speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f) / WIND_FORCE_MAG;
}

// Windの終了処理
void Wind_Finalize()
{

}

// Windの更新
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

// Windの描画
void Wind_Draw()
{
	DebugFont_Draw(640, 62, "wind: %.01lf", wind.speed.x * WIND_FORCE_MAG);
}

Wind* Get_Wind()
{
	return &wind;
}