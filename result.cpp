#include "result.h"
#include "debug_font.h"
#include "input.h"
#include "score.h"

int *score;

// Result‚Ì‰Šú‰»
void Result_Initialize()
{
	score = GetScore();
}

// BG‚ÌI—¹ˆ—
void Result_Finalize()
{
	Score_Finalize();
}

// BG‚ÌXV
void Result_Update()
{
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		SetScene(SCENE_TITLE);
	}
}

// BG‚Ì•`‰æ
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