#include "score.h"
#include "debug_font.h"

int score;

// Score�̏�����
void Score_Initialize()
{
	score = 0;
}

// Score�̏I������
void Score_Finalize()
{
	score = 0;
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

// �X�R�A�̎擾
int* GetScore()
{
	return &score;
}