#include "mode.h"
#include "input.h"
#include "debug_font.h"
#include "player.h"

static Mode mode_bg;
static Mode mode_easy;
static Mode mode_hard;
ModeIndex ModeIdx = MODE_NONE;

// Mode�̏�����
void Mode_Initialize()
{
	mode_bg.TextureIndex = TEXTURE_INDEX_MODE_BG;
	mode_bg.tw = Texture_GetWidth(mode_bg.TextureIndex);
	mode_bg.th = Texture_GetHeight(mode_bg.TextureIndex);
	mode_bg.pos.x = SCREEN_WIDTH * 0.5f;
	mode_bg.pos.y = SCREEN_HEIGHT * 0.5f;

	mode_easy.TextureIndex = TEXTURE_INDEX_MODE_EASY;
	mode_easy.tw = Texture_GetWidth(mode_easy.TextureIndex);
	mode_easy.th = Texture_GetHeight(mode_easy.TextureIndex);
	mode_easy.pos.x = SCREEN_WIDTH * 0.5f;
	mode_easy.pos.y = SCREEN_HEIGHT * 0.5f - Texture_GetHeight(mode_easy.TextureIndex) * 1.5f;

	mode_hard.TextureIndex = TEXTURE_INDEX_MODE_HARD;
	mode_hard.tw = Texture_GetWidth(mode_hard.TextureIndex);
	mode_hard.th = Texture_GetHeight(mode_hard.TextureIndex);
	mode_hard.pos.x = SCREEN_WIDTH * 0.5f;
	mode_hard.pos.y = SCREEN_HEIGHT * 0.5f + Texture_GetHeight(mode_hard.TextureIndex) * 1.5f;

	Player_Initialize();
}

// Mode�̏I������
void Mode_Finalize()
{
	Player_Finalize();
}

// Mode�̍X�V
void Mode_Update()
{
	Player *player = GetPlayer();

	Player_Update();

	// ������
	if ((player->pos.x + player->tw * 0.25f >= mode_easy.pos.x - mode_easy.tw * 0.5f) && 
		(player->pos.x - player->tw * 0.25f <= mode_easy.pos.x + mode_easy.tw * 0.5f))
	{
		// �c����ieasy�j
		if ((player->pos.y + player->th * 0.25f >= mode_easy.pos.y - mode_easy.th * 0.5f) &&
			(player->pos.y - player->th * 0.25f <= mode_easy.pos.y + mode_easy.th * 0.5f))
		{
			ModeIdx = MODE_EASY;
		}
		// �c����ihard�j
		else if ((player->pos.y + player->th * 0.25f >= mode_hard.pos.y - mode_hard.th * 0.5f) &&
			(player->pos.y - player->th * 0.25f <= mode_hard.pos.y + mode_hard.th * 0.5f))
		{
			ModeIdx = MODE_HARD;
		}
		else
		{
			ModeIdx = MODE_NONE;
		}
	}
	else
	{
		ModeIdx = MODE_NONE;
	}

	if (Keyboard_IsTrigger(DIK_RETURN) && ModeIdx != MODE_NONE)
	{
		SetScene(SCENE_GAME);
	}
}

// Mode�̕`��
void Mode_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// �e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(mode_bg.TextureIndex));
	pDevice->SetTexture(0, Texture_GetTexture(mode_easy.TextureIndex));
	pDevice->SetTexture(0, Texture_GetTexture(mode_hard.TextureIndex));

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
	if (mode_bg.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(mode_bg.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(mode_bg.TextureIndex,
			mode_bg.pos.x, mode_bg.pos.y,
			mode_bg.tx, mode_bg.ty,
			mode_bg.tw, mode_bg.th);
	}
	if (mode_easy.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(mode_easy.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(mode_easy.TextureIndex,
			mode_easy.pos.x, mode_easy.pos.y,
			mode_easy.tx, mode_easy.ty,
			mode_easy.tw, mode_easy.th);
	}
	if (mode_hard.bUse) // �g�p���Ȃ珈��
	{
		Sprite_SetColor(mode_hard.color); // �F�̃Z�b�g
		// �X�v���C�g�`��
		Sprite_Draw(mode_hard.TextureIndex,
			mode_hard.pos.x, mode_hard.pos.y,
			mode_hard.tx, mode_hard.ty,
			mode_hard.tw, mode_hard.th);
	}
	Player_Draw();
	DebugFont_Draw(2, 32, "tw: %d th: %d", mode_bg.tw, mode_bg.th);
	DebugFont_Draw(2, 62, "tw: %d th: %d", mode_easy.tw, mode_easy.th);
	DebugFont_Draw(2, 92, "tw: %d th: %d", mode_hard.tw, mode_hard.th);
	DebugFont_Draw(2, 122, "Mode: %d", ModeIdx);
}

// Mode���̎擾
ModeIndex GetMode()
{
	return ModeIdx;
}

Mode::Mode()
{
	bUse = true;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	tx = 0;
	ty = 0;
}

Mode::~Mode()
{

}