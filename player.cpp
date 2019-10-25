#include "player.h"
#include "input.h"
#include "debug_font.h"
#include "sound.h"
#include "arrow.h"

Player *player;

// Player�̏�����
void Player_Initialize()
{
	player = new Player;
}

// Player�̏I������
void Player_Finalize()
{
	delete player;
}

// Player�̍X�V
void Player_Update()
{
	// �}�E�X���̎擾
	DIMOUSESTATE *MouseState = GetMouseState();
	player->pos.x += MouseState->lX;
	player->pos.y += MouseState->lY;

	//��ʊO����
	if ((player->pos.x - (float)player->tw / 2) <= 0.0f)
	{
		player->pos.x = (float)player->tw / 2;
	}
	if ((player->pos.x + (float)player->tw / 2) >= SCREEN_WIDTH)
	{
		player->pos.x = (float)(SCREEN_WIDTH - player->tw / 2);
	}
	if ((player->pos.y - (float)player->th / 2) <= 0.0f)
	{
		player->pos.y = (float)player->th / 2;
	}
	if ((player->pos.y + (float)player->th / 2) >= SCREEN_HEIGHT)
	{
		player->pos.y = (float)(SCREEN_HEIGHT - player->th / 2);
	}

	// �`���[�W
	if (Keyboard_IsPress(DIK_Z))
	{	// SPACE�������Ă���ԃ`���[�W����
		if (player->charge_span < CHARGE_SPAN)
		{
			player->charge_span++;
		}
		if (Keyboard_IsPress(DIK_X))
		{
			Arrow *arrow = GetArrow();
			for (int i = 0; i < ARROW_MAX; i++)
			{
				if (arrow[i].bUse && !arrow[i].beShotted)
				{
					arrow[i].beShotted = true;
				}
			}
			PlaySound(SOUND_LABEL_SE_SHOT00);
			player->charge_span = 0;
		}
	}
	else
	{
		if (player->charge_span > 0)
		{
			player->charge_span--;
		}
	}
}

// Player�̕`��
void Player_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	//�e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(player->TextureIndex));

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
	if (player->bUse)//�g�p���Ȃ珈��
	{
		Sprite_SetColor(player->color);//�F�̃Z�b�g
		//�X�v���C�g�`��
		Sprite_Draw(player->TextureIndex,
			player->pos.x, player->pos.y,
			player->tx, player->ty,
			player->tw, player->th);
		DebugFont_Draw(2, 2, "x: %.2lf y: %.2lf charge: %d", player->pos.x, player->pos.y, player->charge_span);
	}
}

// �v���C���[���̎擾
Player* GetPlayer()
{
	return player;
}

Player::Player()
{
	bUse = true; // �\���̎g�p��
	isShot = false;
	pos.x = SCREEN_WIDTH * 0.5f; 
	pos.y = SCREEN_HEIGHT * 0.5f;
	degree.x = 270.0f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_PLAYER;
	tx = Texture_GetWidth(TextureIndex);
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