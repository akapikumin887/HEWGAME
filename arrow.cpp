#include "arrow.h"
#include "input.h"
#include "debug_font.h"
<<<<<<< HEAD
#include "player.h"
#include "target.h"
#include "score.h"

static Arrow arrow[ARROW_MAX];
int Arrow::cnt = 0;
static Target *target = GetTarget();
=======

Arrow arrow[ARROW_MAX];
int Arrow::cnt = 0;
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83

// ARROW�̏�����
void Arrow_Initialize()
{

}

// ARROW�̏I������
void Arrow_Finalize()
{

}

// ARROW�̍X�V
void Arrow_Update()
{
<<<<<<< HEAD
	// �V�[���J��
	if (Arrow::cnt == 5)
	{
		SetScene(SCENE_RESULT);
	}
	
=======
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
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
<<<<<<< HEAD
			{   
				// �������Ă��Ȃ�
				if (!arrow[i].hit)
=======
			{
				// ���x�X�V
				arrow[i].move.x = ARROW_SPEED * arrow[i].direction.x * arrow[i].charge;
				arrow[i].move.y = ARROW_SPEED * arrow[i].direction.y * arrow[i].charge;

				// �ʒu�X�V
				arrow[i].pos.x += arrow[i].move.x;
				arrow[i].pos.y += arrow[i].move.y;

				// ��ʊO�`�F�b�N
				if (arrow[i].pos.x >= SCREEN_WIDTH || arrow[i].pos.x <= 0 || arrow[i].pos.y >= SCREEN_HEIGHT || arrow[i].pos.y <= 0)
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
				{
					// ���x�X�V
					arrow[i].move.x = ARROW_SPEED * arrow[i].direction.x * arrow[i].charge;
					arrow[i].move.y = ARROW_SPEED * arrow[i].direction.y * arrow[i].charge;

					// ��̔��̈ʒu�X�V
					arrow[i].posTail.x += arrow[i].move.x;
					arrow[i].posTail.y += arrow[i].move.y;

					// ��̐擪�̈ʒu�X�V
					arrow[i].Arrow_Head_Pos();

					// ��ʊO�`�F�b�N
					if (arrow[i].posTail.x >= SCREEN_WIDTH || arrow[i].posTail.x <= 0 || arrow[i].posTail.y >= SCREEN_HEIGHT || arrow[i].posTail.y <= 0)
					{
						arrow[i].bUse = false;
						arrow[i].beShotted = false;
					}

					// �����蔻��
					for (int j = 0; j < 3; j++)
					{
						// �c����
						if (arrow[i].posHead.y <= target[j].pos.y + target[j].th / 2)
						{
							// ������
							if (arrow[i].posHead.x >= target[j].pos.x - target[j].tw / 2 && arrow[i].posHead.x <= target[j].pos.x + target[j].tw / 2)
							{
								// ����
								arrow[i].hit = true;
								if (j == 0)
								{
									Add_Score(5);
								}
								else
								{
									Add_Score(1);
								}
							}
						}
					}
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
		// �X�v���C�g�`��
		if (arrow[i].bUse) // �g�p���Ȃ珈��
		{
			// �e�N�X�`���̃Z�b�g
			pDevice->SetTexture(0, Texture_GetTexture(arrow[i].TextureIndex));

			// �u�����h�ݒ�
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

			Sprite_SetColor(arrow[i].color); // �F�̃Z�b�g

			// �X�v���C�g�`��
			Sprite_Draw_Rotation_Arrow(arrow[i].TextureIndex,
				arrow[i].posTail.x, arrow[i].posTail.y,
				arrow[i].tx, arrow[i].ty,
				arrow[i].tw, arrow[i].th,
				arrow[i].degree.z);
		}
	}
	Arrow::Print();
}

// ARROW�̍쐬
void Create_Arrow()
{
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (!arrow[i].bUse && Arrow::cnt < ARROW_MAX)
		{
			arrow[i].bUse = true;
			arrow[i].posTail.x = ARROW_X;
			arrow[i].posTail.y = ARROW_Y;
			arrow[i].Arrow_Direction_Normalize();
			Arrow::cnt++;
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
	hit = false;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_ARROW01;
	tx = Texture_GetWidth(TextureIndex);
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
	charge = 0;
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
	direction.x = player->pos.x - posTail.x;
	direction.y = player->pos.y - posTail.y;

	// �����x�N�g���̐��K��
	len = sqrtf(direction.x * direction.x + direction.y * direction.y);
	direction.x = direction.x / len;
	direction.y = direction.y / len;
	degree.z = atan2f(direction.y, direction.x);
}

<<<<<<< HEAD
void Arrow::Arrow_Head_Pos()
{
	posHead.x = posTail.x + Texture_GetWidth(TextureIndex) * direction.x;
	posHead.y = posTail.y + Texture_GetWidth(TextureIndex) * direction.y;
}

=======
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
void Arrow::Print()
{
	DebugFont_Draw(2, 32, "�c��̖{��: %d", ARROW_MAX - Arrow::cnt);
}