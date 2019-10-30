#include "target.h"

static Target target[3];

// Target�̏�����
void Target_Initialize()
{
	target[1].tw = target[2].tw = target[0].tw * 2;
	target[1].pos.x = target[0].pos.x - (target[0].tw + target[1].tw) * 0.5f;
	target[2].pos.x = target[0].pos.x + (target[0].tw + target[2].tw) * 0.5f;
	target[1].color = D3DCOLOR_RGBA(240, 240, 0, 255); // �F��K���ɍ��
	target[2].color = D3DCOLOR_RGBA(240, 240, 0, 255); // �F��K���ɍ��
}

// Target�̏I������
void Target_Finalize()
{

}

// Target�̍X�V
void Target_Update()
{

}

// Target�̕`��
void Target_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	for (int i = 0; i < 3; i++)
	{ // �e�N�X�`���̃Z�b�g
		pDevice->SetTexture(0, Texture_GetTexture(target[i].TextureIndex));
	}

	// �u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	for (int i = 0; i < 3; i++)
	{
		if (target[i].bUse)
		{
			Sprite_SetColor(target[i].color); // �F�̃Z�b�g
		    // �X�v���C�g�`��
			Sprite_Draw(target[i].TextureIndex,
				target[i].pos.x, target[i].pos.y,
				target[i].tx, target[i].ty,
				target[i].tw, target[i].th);
		}
	}
}

// Target�̎擾
Target* GetTarget()
{
	return target;
}

Target::Target()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = 10.0f;
	color = D3DCOLOR_RGBA(0, 240, 0, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_MAX;
	tx = 0;
	ty = 0;
	tw = 30;
	th = 20;
}

Target::~Target()
{

}