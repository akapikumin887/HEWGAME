#include "score.h"
#include "debug_font.h"

// Score�̏������i�R���X�g���N�^�j
Score::Score()
{
	num = new Number;
	alpha = new Alphabet;
}

// Score�̏I�������i�f�X�g���N�^�j
Score::~Score()
{
	delete num;
	delete alpha;
}

// Score�̏�����
void Score::Initialize()
{	
	// Alphabet�̏�����
	//strncpy(alpha->word, "SCORE", 8);
	//alpha->len = strlen(alpha->word);
}

// Score�̏I������
void Score::Finalize()
{
	
}

// Score�̍X�V
void Score::Update()
{
	
}

// Score�̕`��
void Score::Draw()
{
	//num->Draw(SCORE_DIGIT_MAX, ArrowManager::score_t);
}

// Score�̉��Z
void Score::Add_Score(int s)
{
	//num->n += s;
}