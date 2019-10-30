#include "wind.h"
#include "debug_font.h"
#include "mode.h"

static Wind wind; 
static ModeIndex ModeIdx;

// Windの初期化
void Wind_Initialize()
{
	ModeIdx = GetMode();
	if (ModeIdx == MODE_EASY)
	{
		wind.WindSpeed.x = 0;
	}
	else
	{
		wind.WindSpeed.x = 2.0f;
	}

	//	wind.WindSpeed.x = 3.0f * (rand() % 10);
	//	// 左
	//	if (rand() % 2 == 0)
	//	{
	//		wind.WindSpeed.x *= -1;
	//		wind.wright = false;
	//	}
	//	// 右
	//	else
	//	{
	//		wind.wright = true;
	//	}
	//}
}

// Windの終了処理
void Wind_Finalize()
{

}

// Windの更新
void Wind_Update()
{

}

// Windの描画
void Wind_Draw()
{
	if (ModeIdx != MODE_EASY)
	{
		DebugFont_Draw(2, 92, "風向: 右");
		DebugFont_Draw(2, 122, "WindSpeedX: %.2lf", wind.WindSpeed.x);
	}
}

// Windの情報の取得
Wind* GetWind()
{
	return &wind;
}

Wind::Wind()
{

}

Wind::~Wind()
{
	
}