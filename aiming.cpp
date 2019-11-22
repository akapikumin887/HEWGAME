#include "aiming.h"
#include "input.h"
#include "debug_font.h"
#include "cube.h"

static Aiming aiming;
static double timeCnt;

// Aiming�̏�����
void Aiming_Initialize()
{
	aiming.Set_Aiming(TEXTURE_INDEX_AIMING, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

// Aiming�̏I������
void Aiming_Finalize()
{
	aiming.prepare = false;
}

// Aiming�̍X�V
void Aiming_Update()
{
	// �}�E�X���̎擾
	DIMOUSESTATE *MouseState = GetMouseState();

	// Aiming�̑���
	aiming.pos.x += MouseState->lX * 0.05f;
	aiming.pos.y -= MouseState->lY * 0.05f;

	// �\���i��̐����j
	if (Keyboard_IsTrigger(DIK_LSHIFT) && !aiming.prepare)
	{
		Add_Cube();
		aiming.prepare = true;
		aiming.timec = new Time;
		aiming.timec->SystemTimer_Start();
	}

	// �\����Ԓ�
	if (aiming.prepare)
	{
		// �J�E���g�_�E�����X�V
		timeCnt = TIME_COUNT_MAX - aiming.timec->SystemTimer_GetTime();

		// ���Ԑ����𒴂���
		if (timeCnt < 0.0f)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					cube[i].nShotted = true;
					break;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false;
			delete aiming.timec;
			return;
		}

		// �|�������i�`���[�W�j
		if (Keyboard_IsPress(DIK_Z))
		{	// SPACE�������Ă���ԃ`���[�W����
			if (aiming.charge_span < CHARGE_SPAN)
			{
				aiming.charge_span++;
			}
		}
		else
		{
			if (aiming.charge_span > 0)
			{
				aiming.charge_span--;
			}
		}

		// ����
		if (Keyboard_IsTrigger(DIK_X) && aiming.charge_span > 0)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				if (cube[i].bUse && !cube[i].bShotted)
				{
					cube[i].bShotted = true;
					//cube[i].charge = aiming.charge_span;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false;
			delete aiming.timec;
		}
	}
}

// Aiming�̕`��
void Aiming_Draw()
{
	aiming.Draw_Aiming();
	DebugFont_Draw(2, 92, "Aiming  x: %.2lf  y: %.2lf  z: %.2lf", aiming.pos.x, aiming.pos.y, aiming.pos.z);
	DebugFont_Draw(640, 32, "AimingTime: %.02lf", timeCnt);
}

// Aiming���̎擾
Aiming* Get_Aiming()
{
	return &aiming;
}

// Aiming�̒��_���擾�i�R���X�g���N�^�j
Aiming::Aiming()
{
	aimingv = new VERTEX_3D;
	isAiming = false;
	prepare = false;
}

// Aiming�̒��_���̃����[�X�i�f�X�g���N�^�j
Aiming::~Aiming()
{
	delete aimingv;
}

// Aiming�̕`��
void Aiming::Draw_Aiming()
{
	aimingv->Sprite_Draw_Face(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
}

// Aiming�̃Z�b�g
void Aiming::Set_Aiming(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Aiming�̉�]�̕␳
float Aiming::Rotation_Correction(float r)
{
	return aimingv->Rotation_Correction(r);
}