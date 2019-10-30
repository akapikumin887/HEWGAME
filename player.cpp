#include "player.h"
#include "input.h"
#include "debug_font.h"
#include "sound.h"
#include "arrow.h"
#include "target.h"

static Player player;
static Target *target;
static SCENE g_Scene;

// Player�̏�����
void Player_Initialize()
{
	target = GetTarget();
	player.pos.x = SCREEN_WIDTH * 0.5f;
	player.pos.y = SCREEN_HEIGHT * 0.5f;
}

// Player�̏I������
void Player_Finalize()
{
	player.prepare = false;
}

// Player�̍X�V
void Player_Update()
{
	// �}�E�X���̎擾
	DIMOUSESTATE *MouseState = GetMouseState();
	player.pos.x += MouseState->lX;
	player.pos.y += MouseState->lY;

	// Scene���̎擾
	g_Scene = GetScene();

	//��ʊO����
	if ((player.pos.x - (float)player.tw / 2) <= 0.0f)
	{
		player.pos.x = (float)player.tw / 2;
	}
	if ((player.pos.x + (float)player.tw / 2) >= SCREEN_WIDTH)
	{
		player.pos.x = (float)(SCREEN_WIDTH - player.tw / 2);
	}
	if (g_Scene == SCENE_GAME)
	{
		if ((player.pos.y - (float)player.th / 2) <= target->th * 4.0f)
		{
			player.pos.y = (float)player.th / 2 + target->th * 4.0f;
		}
	}
	else
	{
		if ((player.pos.y - (float)player.th / 2) <= 0.0f)
		{
			player.pos.y = (float)player.th / 2;
		}
	}
	if ((player.pos.y + (float)player.th / 2) >= SCREEN_HEIGHT)
	{
		player.pos.y = (float)(SCREEN_HEIGHT - player.th / 2);
	}

	// �\���i��̐����j
	if (Keyboard_IsTrigger(DIK_SPACE) && !player.prepare)
	{
		Create_Arrow();
		player.prepare = true;
	}

	// �\����Ԓ�
	if (player.prepare)
	{
		// �|�������i�`���[�W�j
		if (Keyboard_IsPress(DIK_Z))
		{	// SPACE�������Ă���ԃ`���[�W����
			if (player.charge_span < CHARGE_SPAN)
			{
				player.charge_span++;
			}
		}
		else
		{
			if (player.charge_span > 0)
			{
				player.charge_span--;
			}
		}

		// ����
		if (Keyboard_IsTrigger(DIK_X) && player.charge_span > 0)
		{
			Arrow *arrow = GetArrow();
			for (int i = 0; i < ARROW_MAX; i++)
			{
				if (arrow[i].bUse && !arrow[i].beShotted)
				{
					arrow[i].beShotted = true;
					arrow[i].charge = player.charge_span;
					PlaySound(SOUND_LABEL_SE_SHOT00);
				}
			}
			player.charge_span = 0;
			player.prepare = false;
		}
	}
}

// Player�̕`��
void Player_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(player.TextureIndex));

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
	if (player.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(player.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(player.TextureIndex,
			player.pos.x, player.pos.y,
			player.tx, player.ty,
			player.tw, player.th);
		DebugFont_Draw(2, 2, "x: %.2lf y: %.2lf charge: %d", player.pos.x, player.pos.y, player.charge_span);
	}
}

// �v���C���[���̎擾
Player* GetPlayer()
{
	return &player;
}

Player::Player()
{
	bUse = true; // �\���̎g�p��
	isShot = false;
	prepare = false;
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