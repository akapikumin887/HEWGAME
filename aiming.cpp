#include "aiming.h"
#include "debug_font.h"
#include "input.h"
#include "tool_functions.h"
#include "game.h"

double Aiming2D::timeCnt;

// Aiming2D
// Aiming2D�̏������i�R���X�g���N�^�j
Aiming2D::Aiming2D()
{
	timer = NULL;
	num = new Number;
	alpha = new Alphabet;
}

// Aiming2D�̏I�������i�f�X�g���N�^�j
Aiming2D::~Aiming2D()
{
	if (timer != NULL)
	{
		delete timer;
		timer = NULL;
	}
	delete num;
	delete alpha;
}

// Aiming2D�̏�����
void Aiming2D::Initialize()
{
	texture_index = TEXTURE_INDEX_AIMING;
	pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(texture_index);
	th = Texture_GetHeight(texture_index);

	state = AIMING_STATE_FREE;

	// Number�̏�����
	num->Initialize(TEXTURE_INDEX_NUMBER, D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(SCREEN_WIDTH - (TIME_DIGIT_MAX - 1) * 32.0f, 64.0f), 0, 0, 10, 1, TIME_DIGIT_MAX / 2, TIME_DIGIT_MAX / 2);
	// Alphabet�̏�����
	alpha->Initialize(TEXTURE_INDEX_ALPHABET, "TIME", D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(num->pos.x - num->digit_i * num->tw, 64.0f), 0, 0, 13, 2);
}

// Aiming2D�̏I������
void Aiming2D::Finalize()
{
	if (timer != NULL)
	{
		delete timer;
		timer = NULL;
	}
}

// Aiming2D�̍X�V
void Aiming2D::Update()
{	
	switch (state)
	{
	case AIMING_STATE_FREE:
		Free();
		break;
	case AIMING_STATE_PREPARE:
		Prepare();
		break;
	case AIMING_STATE_ZOOM_FORWARD:
		Zoom_Forward(zoomStart);
		break;
	case AIMING_STATE_ZOOM_BACKWARD_PREPARE:
		Zoom_Backward(zoomStart);
		break;
	case AIMING_STATE_ZOOM_BACKWARD_SHOT:
		Zoom_Backward(zoomStart, ZOOM_FORWARD + ZOOM_CHECK, ZOOM_INCREASING * 2);
		break;
	case AIMING_STATE_CHECK_ARROW:
		Zoom_Forward(zoomStart, ZOOM_CHECK, ZOOM_INCREASING * 2);
		break;
	default:
		break;
	}
}

// Aiming2D�̕`��
void Aiming2D::Draw()
{
	// �\����Ԓ�
	if (state == AIMING_STATE_PREPARE || state == AIMING_STATE_ZOOM_FORWARD)
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

		Sprite_SetColor_2D(color); // �F�̃Z�b�g

		Sprite_Draw_2D(texture_index, pos.x, pos.y, tx, ty, tw, th);
	}
	DebugFont_Draw(2, 92, "posHit  x: %.2lf  y: %.2lf  z: %.2lf", posHit.x, posHit.y, posHit.z);
	DebugFont_Draw(2, 152, "timeCnt  x: %.2lf", timeCnt);
	Draw_Timer();
}

// Timer�̕\��
void Aiming2D::Draw_Timer()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �^�C�}�[���ԕ\��
	// �J�E���g�_�E���^�C�}�[������ꍇ
	if (timer != NULL)
	{
		num->Draw(timeCnt, num->digit_i, num->digit_d);
		
	}
	// �J�E���g�_�E���^�C�}�[���Ȃ��ꍇ
	else
	{
		num->Draw(0, num->digit_i, num->digit_d);
	}
	alpha->Draw();
}

// �t���[���
void Aiming2D::Free()
{
	if (GetKeyboardTrigger(DIK_LSHIFT) && ArrowManager::cnt < ARROW_MAX)
	{
		ArrowManager::Add_Arrow();
		state = AIMING_STATE_ZOOM_FORWARD;
		zoomStart = Get_Game_CameraFP()->posEye.z;
	}
}

// �\�����
void Aiming2D::Prepare()
{
	// Aiming2D����̎ː��������������W�̎擾
	// �K�v�̏��̎擾
	CameraFP *cameraFP = Get_Game_CameraFP();
	Target *target = Get_Game_Target();

	// Camera����o�����ː��������������W�̎擾
	posHit = RayHit(
		D3DXVECTOR3(cameraFP->posEye.x, cameraFP->posEye.y, target->pos.z), // ������Ԃ̎��A�ː��̂������������W
		D3DXVECTOR2(cameraFP->rotEye.x, cameraFP->rotEye.y), // �ː��̉�]
		target->pos.z - cameraFP->posEye.z); // ������Ԃ̎��ACamera����Target�ւ̋���

	if (timer == NULL)
	{
		timer = new Timer; // �J�E���g�_�E���^�C�}�[�C���X�^���X�̐���
		timer->SystemTimer_Initialize();
		timer->SystemTimer_Start(); // �^�C���X�^�[�g
	}

	// �J�E���g�_�E�����X�V
	timeCnt = TIME_COUNT_MAX - timer->SystemTimer_GetTime();
	
	// ���Ԑ����𒴂���
	if (timeCnt < 0.0f)
	{
		Arrow *arrows = ArrowManager::arrows;
		for (int i = 0; i < ArrowManager::num; i++)
		{
			// ���ˏ�����
			if (arrows[i].state == ARROW_STATE_PREPARE)
			{
				// ��\���ɂ���
				arrows[i].state = ARROW_STATE_NONE;
				arrows[i].Display = false;
				break;
			}
		}
		//aiming.charge_span = 0;
		state = AIMING_STATE_ZOOM_BACKWARD_PREPARE;
		zoomStart = cameraFP->posEye.z;
		delete timer; // �J�E���g�_�E���^�C�}�[�C���X�^���X�̍폜
		timer = NULL;
		return;
	}
	// ���Ԑ�����
	else
	{
		if (GetKeyboardPress(DIK_Z))
		{
			state = AIMING_STATE_NONE;

			Arrow *arrows = ArrowManager::arrows;
			for (int i = 0; i < ArrowManager::num; i++)
			{
				// ���ˏ�����
				if (arrows[i].state == ARROW_STATE_PREPARE)
				{
					// ��\���ɂ���
					arrows[i].state = ARROW_STATE_FLYING;
					break;
				}
			}
			delete timer;
			timer = NULL;
		}
	}
}

// �O�iZoom�҂�
void Aiming2D::Zoom_Forward(float start, float zm, float zi)
{
	CameraFP *cameraFP = Get_Game_CameraFP();
	
	cameraFP->bZoom = true;

	if (cameraFP->rotEye.x != cameraFP->rotEyeDef.x || cameraFP->rotEye.y != cameraFP->rotEyeDef.y)
	{
		cameraFP->Set_Rot_Reset_Value();
		cameraFP->Rot_Reset();
	}
	else
	{
		// Camera��]�̃��Z�b�g���I�������A���Z�b�g�l�Z�b�g�t���O���I�t��
		cameraFP->bSetValue = false;

		if (cameraFP->posEye.z < start + zm)
		{
			cameraFP->Zoom_Foward(zi);
		}
		else
		{
			if (state == AIMING_STATE_ZOOM_FORWARD)
			{
				// �Y�[�����I�������AZoom�t���O���I�t��
				cameraFP->bZoom = false;
				state = AIMING_STATE_PREPARE;

				Arrow *arrows = ArrowManager::arrows;
				for (int i = 0; i < ArrowManager::num; i++)
				{
					if (arrows[i].state == ARROW_STATE_WAIT_ZOOM)
					{
						arrows[i].state = ARROW_STATE_PREPARE;
						arrows[i].Display = true;
					}
				}
			}
			else if (state == AIMING_STATE_CHECK_ARROW && GetKeyboardTrigger(DIK_C))
			{
				state = AIMING_STATE_ZOOM_BACKWARD_SHOT;
				zoomStart = cameraFP->posEye.z;
			}
		}
	}
}

// ���Zoom�҂�
void Aiming2D::Zoom_Backward(float start, float zm, float zi)
{
	CameraFP *cameraFP = Get_Game_CameraFP();

	cameraFP->bZoom = true;

	if (cameraFP->rotEye.x != cameraFP->rotEyeDef.x || cameraFP->rotEye.y != cameraFP->rotEyeDef.y)
	{
		cameraFP->Set_Rot_Reset_Value();
		cameraFP->Rot_Reset();
	}
	else
	{
		// Camera��]�̃��Z�b�g���I�������A���Z�b�g�l�Z�b�g�t���O���I�t��
		cameraFP->bSetValue = false;

		if (cameraFP->posEye.z > start - zm)
		{
			cameraFP->Zoom_Backward(zi);
		}
		else
		{
			// �Y�[�����I�������AZoom�t���O���I�t��
			cameraFP->bZoom = false;
			state = AIMING_STATE_FREE;
		}
	}
}

// Aiming3D
// Aiming3D�̏������i�R���X�g���N�^�j
Aiming3D::Aiming3D()
{
	face = new Face;
}

// Aiming3D�̏I�������i�f�X�g���N�^�j
Aiming3D::~Aiming3D()
{
	delete face;
}

// Aiming3D�̏�����
void Aiming3D::Initialize()
{
	texture_index = TEXTURE_INDEX_AIMING;
	pos = Pos_With_Camera();
	rot = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	face->CreateFace(D3DXVECTOR3(0.1f, 0.0f, 0.1f));
	prepare = false;
}

// Aiming3D�̏I������
void Aiming3D::Finalize()
{

}

// Aiming3D�̍X�V
void Aiming3D::Update()
{
	// Aiming3D����̎ː��������������W�̎擾
	// �K�v�̏��̎擾
	CameraFP *cameraFP = Get_Game_CameraFP();
	Target *target = Get_Game_Target();

	// Camera�ɂ��Aiming�̈ʒu�X�V
	pos = Pos_With_Camera();

	// Camera����o�����ː��������������W�̎擾
	posHit = RayHit(
		D3DXVECTOR3(cameraFP->posEye.x, cameraFP->posEye.y, target->pos.z), // ������Ԃ̎��A�ː��̂������������W
		D3DXVECTOR2(cameraFP->rotEye.x, cameraFP->rotEye.y), // �ː��̉�]
		target->pos.z - cameraFP->posEye.z); // ������Ԃ̎��ACamera����Target�ւ̋���

	// �\�����
	if (GetKeyboardTrigger(DIK_LSHIFT) && !prepare && ArrowManager::cnt < 5)
	{
		ArrowManager::Add_Arrow();
		prepare = true;
		
	}
}

// Aiming3D�̕`��
void Aiming3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl; // �X�P�[�����O�s��
	D3DXMATRIX mtxRot; // ��]�s��
	D3DXMATRIX mtxTrs; // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, 1, 1, 1);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y + rot_Camera.y), D3DXToRadian(rot.x + rot_Camera.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	face->Draw(texture_index);

	DebugFont_Draw(2, 92, "posHit  x: %.2lf  y: %.2lf  z: %.2lf", posHit.x, posHit.y, posHit.z);
}

// Camera�ɂ��ʒu
D3DXVECTOR3 Aiming3D::Pos_With_Camera()
{
	rot_Camera = Get_Game_CameraFP()->rotEye;
	rot_Camera.x *= -1;
	return Get_PointXYZ_OnLine(Get_Game_CameraFP()->posEye, rot_Camera, 2.0f);
}