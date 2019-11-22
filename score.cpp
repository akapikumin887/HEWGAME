#include "score.h"
#include "debug_font.h"
#include"input.h"
#include"sprite.h"
#include"texture.h"

int score;
int g_Score;
static Score score1;
// Score�̏�����
void Score_Initialize()
{
	score = 0000;
}

// Score�̏I������
void Score_Finalize()
{
	score = 0000;
}

// Score�̍X�V
void Score_Update()
{
	
}

// Score�̕`��
void Score_Draw()
{
	
	

	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(score1.TextureIndex));
	// �u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

		Sprite_SetColor(score1.color); // �F�̃Z�b�g
		// �X�v���C�g

		g_Score = score;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*4, score1.pos.y = 24,
			score1.tx = g_Score*64, score1.ty = 0,
			64, 64);
		g_Score = score / 10;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*3, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);
		g_Score = score / 100;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*2, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);
		g_Score = score / 1000;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);






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