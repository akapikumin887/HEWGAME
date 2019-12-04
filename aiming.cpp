#include "aiming.h"
#include "input.h"
#include "debug_font.h"
#include "camera.h"
#include "cube.h"

static Aiming aiming;
static Number time;
static AimingTime alphabet;
static Camera *camera;
static double timeCnt;

// Aiming�̏�����
void Aiming_Initialize()
{
	aiming.Set_Aiming(TEXTURE_INDEX_AIMING, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// Alphabet�̏�����
	strncpy(alphabet.ta, "TIME", 8);
	alphabet.len = strlen(alphabet.ta);
	camera = Get_Camera();
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
	if (Keyboard_IsTrigger(DIK_LSHIFT) && !aiming.prepare && !camera->bZoom_Ready && !camera->bZoom_Back)
	{
		Add_Cube(); // ��̐���
		aiming.prepare = true;
		aiming.time_cnt = new Timer; // �J�E���g�_�E���^�C�}�[�C���X�^���X�̐���
		aiming.time_cnt->SystemTimer_Start(); // �^�C���X�^�[�g
	}

	// �\����Ԓ�
	if (aiming.prepare)
	{
		// Camera�Y�[���O�i
		camera->Camera_Zoom_Forward();

		// �J�E���g�_�E�����X�V
		timeCnt = TIME_COUNT_MAX - aiming.time_cnt->SystemTimer_GetTime();

		// ���Ԑ����𒴂���
		if (timeCnt < 0.0f)
		{
			Cube *cube = Get_Cube(); // Cube���̎擾
			for (int i = 0; i < CUBE_MAX; i++)
			{
				// �g�p���ŁA���˂���Ă��Ȃ��A���Ԑ����Ŕ��˂���Ă��Ȃ��ł͂Ȃ���
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					// ���Ԑ����Ŗ����˂�����A�t���O��true��
					cube[i].nShotted = true;
					break;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false; // �\������
			camera->bZoom_Back = true; // Camera�Y�[����ރt���O
			delete aiming.time_cnt; // �J�E���g�_�E���^�C�}�[�C���X�^���X�̍폜
			aiming.time_cnt = NULL;
			return;
		}

		// �Y�[�����I���܂Ń`���[�W�͂ł��Ȃ��i�`���[�W�͂ł��Ȃ�����A���˂��ł��Ȃ��j
		if (camera->zoom_cnt >= ZOOM_MAX)
		{
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
		}

		// ����
		if (Keyboard_IsTrigger(DIK_X) && aiming.charge_span > 0)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				// �g�p���ŁA���˂���Ă��Ȃ��A���Ԑ����Ŕ��˂���Ă��Ȃ��ł͂Ȃ���𔭎�
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					cube[i].bShotted = true; // ���˂��ꂽ���Ƃɂ���
					//cube[i].charge = aiming.charge_span;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false; // �\������
			delete aiming.time_cnt; // �J�E���g�_�E���^�C�}�[�C���X�^���X�̍폜
			aiming.time_cnt = NULL;
		}
	}

	// Camera�Y�[�����
	camera->Camera_Zoom_Back();
}

// Aiming�̕`��
void Aiming_Draw()
{
	aiming.Draw_Aiming();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �u�����h�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	Sprite_SetColor_2D(time.color); // �F�̃Z�b�g

	// �^�C�}�[���ԕ\��
	// �J�E���g�_�E���^�C�}�[������ꍇ
	if (aiming.time_cnt != NULL)
	{
		for (int i = 0; i < TIME_DIGIT_MAX - 1; i++)
		{
			// �����_�ȍ~2������n�܂�ii��0�̎��A�J�E���g�_�E����10��-2��Ŋ�������A100�{�ɂȂ�j
			time.n_tmp = (int)(timeCnt / pow((double)10, (double)i - 2));
			time.n_tmp = time.n_tmp % 10; // 1�̈ʂ̒l�����o��

			// ���o�����l��\��
			// �����_�ȍ~2����\���o������A1�}�X�����āA�����_�ȑO��2����\������
			Sprite_Draw_2D(time.TextureIndex,
				SCREEN_WIDTH - time.pos.x * (i < TIME_DIGIT_MAX / 2 ? i : i + 1) * 2 - time.pos.x, time.pos.y * 4,
				time.tx + time.n_tmp * time.tw, time.ty,
				time.tw, time.th);
		}
	}
	// �J�E���g�_�E���^�C�}�[���Ȃ��ꍇ
	else
	{
		for (int i = 0; i < TIME_DIGIT_MAX - 1; i++)
		{
			// �S��0�ŕ\��
			Sprite_Draw_2D(time.TextureIndex,
				SCREEN_WIDTH - time.pos.x * (i < TIME_DIGIT_MAX / 2 ? i : i + 1) * 2 - time.pos.x, time.pos.y * 4,
				time.tx, time.ty,
				time.tw, time.th);
		}
	}

	Sprite_SetColor_2D(alphabet.color); // �F�̃Z�b�g

	// �A���t�@�x�b�g�̕\��
	for (int i = 0; i < alphabet.len; i++)
	{
		Sprite_Draw_2D(alphabet.TextureIndex,
			SCREEN_WIDTH - time.pos.x * DIGIT_MAX * 2 - alphabet.pos.x * (alphabet.len - i) * 2, alphabet.pos.y * 4,
			alphabet.tx + ((alphabet.ta[i] - 65) % 13) * alphabet.tw, alphabet.ty + (alphabet.ta[i] - 65) / 13 * alphabet.th,
			alphabet.tw, alphabet.th);
	}
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
	time_cnt = NULL;
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