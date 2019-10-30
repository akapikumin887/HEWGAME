#include "title.h"
#include "input.h"

static Title title;

// Title�̏�����
void Title_Initialize()
{

}

// Title�̏I������
void Title_Finalize()
{

}

// Title�̍X�V
void Title_Update()
{
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		SetScene(SCENE_MODE);
	}
}

// Title�̕`��
void Title_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(title.TextureIndex));

	// �u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �X�v���C�g�`��
	if (title.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(title.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(title.TextureIndex,
			title.pos.x, title.pos.y,
			title.tx, title.ty,
			title.tw, title.th);
	}
}

Title::Title()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_TITLE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Title::~Title()
{

}