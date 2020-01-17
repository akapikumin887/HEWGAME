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
	strncpy(alpha->word, "SCORE", 8);
	alpha->len = strlen(alpha->word);

	for (int i = 0; i < 5; i++)
	{
		s[i] = 0;
	}
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
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(num->color); // �F�̃Z�b�g

	// �X�R�A�\��
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		int tmp;
		// 1�̈ʂ���n�܂�
		tmp = num->n / (int)powf((float)10, (float)i);
		tmp = tmp % 10; // 1�̈ʂ̒l�����o��

		// ���o�����l��\��
		Sprite_Draw_2D(num->texture_index,
			SCREEN_WIDTH - num->pos.x * i * 2 - num->pos.x, num->pos.y,
			num->tx + tmp * num->tw, num->ty,
			num->tw, num->th);
	}
	
	Sprite_SetColor_2D(alpha->color); // �F�̃Z�b�g

	// �A���t�@�x�b�g�̕\��
	for (int i = 0; i < alpha->len; i++)
	{
		Sprite_Draw_2D(alpha->texture_index,
			SCREEN_WIDTH - num->pos.x * DIGIT_MAX * 2 - alpha->pos.x * (alpha->len - i) * 2, alpha->pos.y,
			alpha->tx + ((alpha->word[i] - 65) % 13) * alpha->tw, alpha->ty + (alpha->word[i] - 65) / 13 * alpha->th,
			alpha->tw, alpha->th);
	}
}

// Score�̉��Z
void Score::Add_Score(int s)
{
	num->n += s;
}