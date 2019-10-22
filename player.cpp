#include "player.h"
#include "input.h"
#include "debug_font.h"

Player *pplayer;
static DIMOUSESTATE *pMouseState;

// Player�̏�����
void Player_Initialize()
{
	pplayer = new Player;
}

// Player�̏I������
void Player_Finalize()
{
	delete pplayer;
}

// Player�̍X�V
void Player_Update()
{
	pMouseState = GetMouseState();

	// Player�̈ړ�
	pplayer->pos.x += pMouseState->lX;
	pplayer->pos.y += pMouseState->lY;

	// ��ʊO����
	if ((pplayer->pos.x - pplayer->tw / 2) <= 0.0f)
	{
		pplayer->pos.x = pplayer->tw / 2;
	}
	if ((pplayer->pos.x + pplayer->tw / 2) >= SCREEN_WIDTH)
	{
		pplayer->pos.x = SCREEN_WIDTH - pplayer->tw / 2;
	}
	if ((pplayer->pos.y - pplayer->th / 2) <= 0.0f)
	{
		pplayer->pos.y = pplayer->th / 2;
	}
	if ((pplayer->pos.y + pplayer->th / 2) >= SCREEN_HEIGHT)
	{
		pplayer->pos.y = SCREEN_HEIGHT - pplayer->th / 2;
	}
}

// Player�̕`��
void Player_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	//�e�N�X�`���̃Z�b�g
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

	//�X�v���C�g�`��
	if (pplayer->bUse)//�g�p���Ȃ珈��
	{
		Sprite_SetColor(pplayer->color);//�F�̃Z�b�g
		//�X�v���C�g�`��
		Sprite_Draw(pplayer->TextureIndex,
			pplayer->pos.x, pplayer->pos.y,
			pplayer->tx, pplayer->ty,
			pplayer->tw, pplayer->th);
		DebugFont_Draw(2, 2, "x: %.2lf y: %.2lf", pplayer->pos.x, pplayer->pos.y);
	}
}

Player::Player()
{
	bUse = true; // �\���̎g�p��
	pos.x = SCREEN_WIDTH / 2; 
	pos.y = SCREEN_HEIGHT / 2;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_PLAYER;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
	power = 0;
	charge_span = 0;
}

Player::~Player()
{
	bUse = false;
}