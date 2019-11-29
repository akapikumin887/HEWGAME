#include "score.h"
#include "debug_font.h"
#include"input.h"

static Number score;
static Score alphabet;

// Score�̏�����
void Score_Initialize()
{	
	// Alphabet�̏�����
	strncpy(alphabet.sa, "SCORE", 8);
	alphabet.len = strlen(alphabet.sa);
}

// Score�̏I������
void Score_Finalize()
{
	score.n = 0;
}

// Score�̍X�V
void Score_Update()
{
	
}

// Score�̕`��
void Score_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(score.TextureIndex));
	// �u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	Sprite_SetColor_2D(score.color); // �F�̃Z�b�g
	
	// �X�v���C�g

	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.n_tmp = score.n / (int)powf((float)10, (float)i);
		score.n_tmp = score.n_tmp % 10;
		Sprite_Draw_2D(score.TextureIndex,
			SCREEN_WIDTH - score.pos.x * i * 2 - score.pos.x, score.pos.y,
			score.tx + score.n_tmp * score.tw, score.ty,
			score.tw, score.th);
	}
	
	for (int i = 0; i < alphabet.len; i++)
	{
		Sprite_Draw_2D(alphabet.TextureIndex,
			SCREEN_WIDTH - score.pos.x * DIGIT_MAX * 2 - alphabet.pos.x * (alphabet.len - i) * 2, alphabet.pos.y,
			alphabet.tx + ((alphabet.sa[i] - 65) % 13) * alphabet.tw, alphabet.ty + (alphabet.sa[i] - 65) / 13 * alphabet.th,
			alphabet.tw, alphabet.th);
	}
}

// Score�̉��Z
void Add_Score(int s)
{
	score.n += s;
}

// �X�R�A�̎擾
Number* Get_Score()
{
	return &score;
}