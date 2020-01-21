#include "arrow.h"
#include "debug_font.h"
#include "input.h"
#include "tool_functions.h"
#include "game.h"

float Arrow::speed = ARROW_MOVE_SPEED;
Arrow* ArrowManager::arrows;
int ArrowManager::num;
int ArrowManager::cnt;
int ArrowManager::score_t;

// ArrowXModel
// ArrowXModel�̏������i�R���X�g���N�^�j
ArrowXModel::ArrowXModel()
{
	xmodel = new XModel;
}

// ArrowXModel�̏I�������i�f�X�g���N�^�j
ArrowXModel::~ArrowXModel()
{
	delete xmodel;
}

// ArrowXModel�̏�����
void ArrowXModel::Initialize()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	xmodel->XModel_Initialize(ARROWXMODEL);
}

// ArrowXModel�̏I������
void ArrowXModel::Finalize()
{
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	xmodel->XModel_Finalize();
}

// ArrowXModel�̍X�V
void ArrowXModel::Update()
{

}

// ArrowXModel�̕`��
void ArrowXModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ���[���h�}�g���b�N�X��ݒ�

	xmodel->XModel_Draw();
}

// Arrow�|���S��
// Arrow�̏������i�R���X�g���N�^�j
Arrow::Arrow()
{
	cube = new Cube;
	gravility = new Gravility;
	wind = new Wind;
}

// Arrow�̏I�������i�f�X�g���N�^�j
Arrow::~Arrow()
{
	delete cube;
	delete gravility;
	delete wind;
}

void Arrow::Initialize()
{
	texture_index = TEXTURE_INDEX_MAX;
	pos = D3DXVECTOR3(ARROW_POS_X, ARROW_POS_Y, ARROW_POS_Z);
	rot = D3DXVECTOR3(ARROW_ROT_X, ARROW_ROT_Y, ARROW_ROT_Z);
	cube->CreateCube(D3DXVECTOR3(ARROW_SIZE_X, ARROW_SIZE_Y, ARROW_SIZE_Z));

	bUse = false;
	Display = false;
	state = ARROW_STATE_NONE;
	score = 0;
	targetLen = 0.0f;

	posHit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posAiming = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Arrow::Finalize()
{

}

void Arrow::Update()
{
	switch (state)
	{
	case ARROW_STATE_PREPARE:
		Aiming_Direction();
		break;
	case ARROW_STATE_FLYING:
		Movement();
		HitCheck();
		break;
	case ARROW_STATE_HIT_TARGET:
		break;
	case ARROW_STATE_HIT_PLANE:
		break;
	case ARROW_STATE_OVER_FLYING:
		Movement();
		break;
	default:
		break;
	}
}

void Arrow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);
	
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ���[���h�}�g���b�N�X��ݒ�

	cube->Draw(texture_index);
}

// Arrow�̔��ˎ��̌���
void Arrow::Aiming_Direction()
{
	Aiming2D *aiming = Get_Game_Aiming();
	posAiming = aiming->posHit;
	direction = posAiming - pos;
	D3DXVec3Normalize(&direction, &direction); // Arrow�̌����̐��K��
	rot = D3DXVECTOR3(-D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Arrow�̉�]
}

// Arrow�̈ړ����̌���
void Arrow::Move_Direction()
{
	direction = pos - posOld; // Cube�̌����̌v�Z
	D3DXVec3Normalize(&direction, &direction); // Cube�̌����̐��K��
	rot = D3DXVECTOR3(-D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Arrow�̉�]
}

// Arrow�̈ړ�
void Arrow::Movement()
{
	// �ړ��ʂ̍X�V
	move = direction * Arrow::speed + Wind::speed + Gravility::g;

	// �����X�V�̂��߁A���̈ʒu�����L�^
	posOld = pos;

	// Arrow�̈ʒu�X�V
	pos += move;

	// �����X�V
	Move_Direction();

	if (pos.z > ARROW_POS_Z_MAX)
	{
		state = ARROW_STATE_NONE;
		Display = false;
	}
}

// �����蔻��
void Arrow::HitCheck()
{
	// Arrow�̐擪�ʒu�̍X�V
	posHead = Get_PointXYZ_OnLine(pos, rot, ARROW_SIZE_Z / 2);

	// �擪�ʒu��Plane�𒴂����ꍇ
	if (posHead.y < Get_Game_Plane()->pos.y)
	{
		// �\����Ԃ̉���
		Aiming2D *aiming = Get_Game_Aiming();
		CameraFP *cameraFP = Get_Game_CameraFP();

		aiming->zoomStart = cameraFP->posEye.z;
		aiming->state = AIMING_STATE_CHECK_ARROW;

		Target *target = Get_Game_Target();
		targetLen = Get_Length(posHead, target->pos);
		state = ARROW_STATE_HIT_PLANE;
		return;
	}

	// �擪�ʒu��Target�𒴂����ꍇ
	if (posHead.z > posAiming.z)
	{
		// Arrow�̈ʒu�Ǝh���ʒu�Ƃ̋���
		float lenRatio = (posAiming.z - pos.z) / (ARROW_SIZE_Z * cosf(D3DXToRadian(rot.y)) * cosf(D3DXToRadian(-rot.x)));

		// �h���ʒu�̍��W�̎擾
		posHit = Get_PointXYZ_OnLine(pos, rot, ARROW_SIZE_Z * lenRatio);

		// �����蔻��
		Target *target = Get_Game_Target();
		targetLen = Get_Length(posHit, target->pos);
		score = Get_Score();

		CameraFP *cameraFP = Get_Game_CameraFP();

		// �\����Ԃ̉���
		Aiming2D *aiming = Get_Game_Aiming();
		aiming->zoomStart = cameraFP->posEye.z;
		aiming->state = AIMING_STATE_CHECK_ARROW;

		// �͂���
		if (targetLen > TARGET_SIZE / 2)
		{
			// �����Ĉړ�
			state = ARROW_STATE_OVER_FLYING;
		}
		// ������
		else
		{
			// �擪�ʒu�Ǝh���ʒu�̍��������AArrow�̈ʒu�𒲐�
			pos -= (posHead - posHit);

			state = ARROW_STATE_HIT_TARGET;
		}
	}
}

// Score�̎擾
int Arrow::Get_Score()
{
	Target *target = Get_Game_Target();
	if (targetLen <= target->circle[0])
	{
		return 100;
	}
	else if (targetLen > target->circle[0] && targetLen <= target->circle[1])
	{
		return 75;
	}
	else if (targetLen > target->circle[1] && targetLen <= target->circle[2])
	{
		return 50;
	}
	else if (targetLen > target->circle[2] && targetLen <= target->circle[3])
	{
		return 25;
	}
	else if (targetLen > target->circle[3] && targetLen <= target->circle[4])
	{
		return 20;
	}
	else if (targetLen > target->circle[4] && targetLen <= target->circle[5])
	{
		return 15;
	}
	else if (targetLen > target->circle[5] && targetLen <= target->circle[6])
	{
		return 10;
	}
	else if (targetLen > target->circle[6] && targetLen <= target->circle[7])
	{
		return 5;
	}
	else if (targetLen > target->circle[7] && targetLen <= target->circle[8])
	{
		return 3;
	}
	else if (targetLen > target->circle[8] && targetLen <= target->circle[9])
	{
		return 2;
	}
	else if (targetLen > target->circle[9] && targetLen <= target->circle[10])
	{
		return 1;
	}
	return 0;
}

// ArrowManager
// �S�Ă�Arrow�̏�����
void ArrowManager::Initialize(int n)
{
	// Arrow�z��̍쐬
	arrows = new Arrow[n];

	for (int i = 0; i < n; i++)
	{
		arrows[i].Initialize();
	}

	// Arrow���ۑ�
	num = n;

	cnt = 0;

	score_t = 0;
}

// �S�Ă�Arrow�̏I������
void ArrowManager::Finalize()
{
	if (arrows != NULL)
	{
		delete[] arrows;
		arrows = NULL;
	}
}

// �S�Ă�Arrow�̍X�V
void ArrowManager::Update()
{
	// �X�s�[�h����
	if (GetKeyboardTrigger(DIK_J))
	{
		Arrow::speed -= 1.0f;
	}

	if (GetKeyboardTrigger(DIK_K))
	{
		Arrow::speed += 1.0f;
	}

	ArrowManager::score_t = 0;
	
	for (int i = 0; i < num; i++)
	{
		arrows[i].Update();
		ArrowManager::score_t += arrows[i].score;
	}
}

// �S�Ă�Arrow�̕`��
void ArrowManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	for (int i = 0; i < num; i++)
	{
		if (arrows[i].Display)
		{
			arrows[i].Draw();
		}
		DebugFont_Draw(640, 32 + i * 30, "targetlen: %.02lf", arrows[i].targetLen);
		DebugFont_Draw(640, 182, "posHit  x: %.2lf  y: %.2lf  z: %.2lf", arrows[0].posHit.x, arrows[0].posHit.y, arrows[0].posHit.z);
		DebugFont_Draw(640, 212, "posAiming  x: %.2lf  y: %.2lf  z: %.2lf", arrows[0].posAiming.x, arrows[0].posAiming.y, arrows[0].posAiming.z);
	}

	DebugFont_Draw(2, 122, "��̎c��: %d", num - cnt);
	DebugFont_Draw(2, 182, "wind: %.01lf", Wind::speed.x * WIND_FORCE_MAG);
	DebugFont_Draw(2, 212, "speed: %.01lf", Arrow::speed);
}

// Arrow�̐���
void ArrowManager::Add_Arrow()
{
	for (int i = 0; i < num; i++)
	{
		if (!arrows[i].bUse)
		{
			Wind::Initialize();
			arrows[i].bUse = true;
			arrows[i].state = ARROW_STATE_WAIT_ZOOM;
			//arrows[i].state = ARROW_STATE_PREPARE;			
			cnt++;
			break;
		}
	}
}

// Arrow���̎擾
Arrow* ArrowManager::Get_Arrow()
{
	if (arrows != NULL)
	{
		return arrows;
	}
	return NULL;
}