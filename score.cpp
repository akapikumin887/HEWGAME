#include "score.h"
#include "debug_font.h"

static int score;

// Score�̏�����
void Score_Initialize()
{
	score = 0;
}

// Score�̏I������
void Score_Finalize()
{

}

// Score�̍X�V
void Score_Update()
{

}

// Score�̕`��
void Score_Draw()
{
	DebugFont_Draw(2, 62, "Score: %d", score);
}

// Score�̉��Z
void Add_Score(int s)
{
	score += s;
}