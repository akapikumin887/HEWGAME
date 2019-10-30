#include "score.h"
#include "debug_font.h"

static int score;

// Score‚Ì‰Šú‰»
void Score_Initialize()
{
	score = 0;
}

// Score‚ÌI—¹ˆ—
void Score_Finalize()
{

}

// Score‚ÌXV
void Score_Update()
{

}

// Score‚Ì•`‰æ
void Score_Draw()
{
	DebugFont_Draw(2, 62, "Score: %d", score);
}

// Score‚Ì‰ÁZ
void Add_Score(int s)
{
	score += s;
}