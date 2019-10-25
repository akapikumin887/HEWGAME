#include "arrow.h"
#include "player.h"
#include "input.h"

Arrow arrow[ARROW_MAX];

// ARROW�̏�����
void Arrow_Initialize()
{
	Create_Arrow();
}

// ARROW�̏I������
void Arrow_Finalize()
{

}

// ARROW�̍X�V
void Arrow_Update()
{
	if (Keyboard_IsPress(DIK_SPACE))
	{
		Create_Arrow();
	}
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (arrow[i].bUse)
		{
			// ���˂���Ă��Ȃ�
			if (!arrow[i].beShotted)
			{
				// �����x�N�g���̎擾�Ɛ��K��
				arrow[i].Arrow_Direction_Normalize();
			}
			// ���˂��ꂽ��
			else
			{
				// �v���C���[���̎擾
				Player *player = GetPlayer();

				// ���x�X�V
				arrow[i].speed.x = ARROW_SPEED * arrow[i].direction.x * player->charge_span;
				arrow[i].speed.y = ARROW_SPEED * arrow[i].direction.y * player->charge_span;

				// �ʒu�X�V
				arrow[i].pos.x += arrow[i].speed.x;
				arrow[i].pos.y += arrow[i].speed.y;

				// ��ʊO�`�F�b�N
				if (arrow[i].pos.x >= SCREEN_WIDTH || arrow[i].pos.x <= 0 || arrow[i].pos.y >= SCREEN_HEIGHT || arrow[i].pos.y <= 0)
				{
					arrow[i].bUse = false;
					arrow[i].beShotted = false;
				}
			}
		}
	}
}

// ARROW�̕`��
void Arrow_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	for (int i = 0; i < ARROW_MAX; i++)
	{
		//�X�v���C�g�`��
		if (arrow[i].bUse)//�g�p���Ȃ珈��
		{
			//�e�N�X�`���̃Z�b�g
			pDevice->SetTexture(0, Texture_GetTexture(arrow[i].TextureIndex));

			//�u�����h�ݒ�
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

			Sprite_SetColor(arrow[i].color);//�F�̃Z�b�g

			//�X�v���C�g�`��
			Sprite_Draw_Rotation_Arrow(arrow[i].TextureIndex,
				arrow[i].pos.x, arrow[i].pos.y,
				arrow[i].tx, arrow[i].ty,
				arrow[i].tw, arrow[i].th,
				atan2f(arrow[i].direction.y, arrow[i].direction.x));
		}
	}
}

// ARROW�̍쐬
void Create_Arrow()
{
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (!arrow[i].bUse)
		{
			arrow[i].bUse = true;
			arrow[i].pos.x = ARROW_X;
			arrow[i].pos.y = ARROW_Y;
			arrow[i].Arrow_Direction_Normalize();
			break;
		}
	}
}

// ARROW�̏��擾
Arrow* GetArrow()
{
	return arrow;
}

Arrow::Arrow()
{
	bUse = false;
	beShotted = false;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_ARROW01;
	tx = Texture_GetWidth(TextureIndex) / 8;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex) / 2;
	th = Texture_GetHeight(TextureIndex);
}

Arrow::~Arrow()
{
	
}

void Arrow::Arrow_Direction_Normalize()
{
	// �v���C���[���̎擾
	Player *player = GetPlayer();
	float len;

	// ��̔��˃x�N�g���X�V
	direction.x = player->pos.x - pos.x;
	direction.y = player->pos.y - pos.y;

	// �����x�N�g���̐��K��
	len = sqrtf(direction.x * direction.x + direction.y * direction.y);
	direction.x = direction.x / len;
	direction.y = direction.y / len;
}