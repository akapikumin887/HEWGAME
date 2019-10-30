#include "result.h"
#include "debug_font.h"
#include "input.h"
#include "score.h"

int *score;

// Resultの初期化
void Result_Initialize()
{
	score = GetScore();
}

// BGの終了処理
void Result_Finalize()
{
	Score_Finalize();
}

// BGの更新
void Result_Update()
{
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		SetScene(SCENE_TITLE);
	}
}

// BGの描画
void Result_Draw()
{
	DebugFont_Draw(SCREEN_WIDTH * 2 / 5 + 60, SCREEN_HEIGHT / 2, "Score: %d", *score);
}

Result::Result()
{

}

Result::~Result()
{

}