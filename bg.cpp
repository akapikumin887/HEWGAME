#include "bg.h"
#include "debug_font.h"

static BG bg01;

// BG�̏�����
void BG_Initialize()
{
	
}

// BG�̏I������
void BG_Finalize()
{
	
}

// BG�̍X�V
void BG_Update()
{

}

// BG�̕`��
void BG_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_PLAYER));

	//�u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �X�v���C�g�`��
	if (bg01.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(bg01.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(bg01.TextureIndex,
			bg01.pos.x, bg01.pos.y,
			bg01.tx, bg01.ty,
			bg01.tw, bg01.th);
	}
}

BG::BG()
{
	bUse = true; // �\���̎g�p��
	pos.x = SCREEN_WIDTH / 2;
	pos.y = SCREEN_HEIGHT / 2;
	color = D3DCOLOR_RGBA(200, 200, 200, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_BG01;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

BG::~BG()
{
	bUse = false;
}