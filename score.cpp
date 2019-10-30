#include "score.h"
#include "debug_font.h"

int score;

// Scoreの初期化
void Score_Initialize()
{
	score = 0;
}

// Scoreの終了処理
void Score_Finalize()
{
	score = 0;
}

// Scoreの更新
void Score_Update()
{

}

// Scoreの描画
void Score_Draw()
{
	DebugFont_Draw(2, 62, "Score: %d", score);
}

// Scoreの加算
void Add_Score(int s)
{
	score += s;
}

// スコアの取得
int* GetScore()
{
	return &score;
}