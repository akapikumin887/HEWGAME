#include "camera.h"
#include "input.h"
#include "tool_functions.h"

// �����o�[�֐��̒�`
// Camera�̕`��
void Camera::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �r���[�s��̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&posEye, // �J�����̈ʒu
		&posAt,  // �J�����̒����_
		&vecUp); // �J�����̏����

	// �r���[�s��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// �v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,  // �r���[���ʂ̎���p
		VIEW_ASPECT, // �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z, // �r���[���ʂ�NearZ�l
		VIEW_FAR_Z); // �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����s��̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

// Camera�̃Y�[���O�i
void Camera::Zoom_Foward(float zi)
{
	posEye.z += zi;	
}

// Camera�̃Y�[�����
void Camera::Zoom_Backward(float zi)
{
	posEye.z -= zi;
}

// �t�@�[�X�g�p�[�\��Camera
// Camera�̏�����
void CameraFP::Initialize(D3DXVECTOR3 peye, D3DXVECTOR3 pat)
{
	posEye = posEyeDef = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = posAtDef = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	direction = posAt - posEye; // Camera�̌����̌v�Z
	D3DXVec3Normalize(&direction, &direction); // Camera�̌����̐��K��
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotEyeDef = rotEye = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Camera�̉�]
	bSetValue = false;
}

// Camera�̏I������
void CameraFP::Finalize()
{
	posEye = posEyeDef;
	posAt = posAtDef;
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotEye = rotEyeDef;
	bSetValue = false;
}

// Camera�̍X�V
void CameraFP::Update()
{
	// Camera�̃��Z�b�g
	Camera_Reset();

	if (!bZoom)
	{
		// CameraEye�̉�]�i���]�j
		CameraEye_Rot();
	}

	// CameraEye�̈ړ�
	CameraEye_Move();

	// CameraAt�̈ړ�
	CameraAt_Move();
}

// Camera�̃��Z�b�g
void CameraFP::Camera_Reset()
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		Finalize();
	}
}

// CameraEye�̈ړ�
void CameraFP::CameraEye_Move()
{
	// Z���ړ��i�O��j
	// SPACE�������Ă���ꍇ�AY���ړ��i�㉺�j
	if (GetKeyboardPress(DIK_W))
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			posEye.y += CAMERA_MOVE_SPEED;
		}
		else
		{
			posEye.x += sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
			posEye.z += cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		}
	}

	if (GetKeyboardPress(DIK_S))
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			posEye.y -= CAMERA_MOVE_SPEED;
		}
		else
		{
			posEye.x -= sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
			posEye.z -= cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		}
	}
	// X���ړ��i���E�j
	if (GetKeyboardPress(DIK_A))
	{
		posEye.x -= cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		posEye.z += sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
	}
	if (GetKeyboardPress(DIK_D))
	{
		posEye.x += cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		posEye.z -= sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
	}

	// �Y�[���i�O��j
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
}

// CameraEye�̉�]�i���]�j
void CameraFP::CameraEye_Rot()
{
	// �}�E�X����
	rotEye.x -= GetMouseY() * CAMERAEYEROT;
	if (rotEye.x > ROT_X_LIMIT)
	{
		rotEye.x = ROT_X_LIMIT;
	}
	if (rotEye.x < -ROT_X_LIMIT)
	{
		rotEye.x = -ROT_X_LIMIT;
	}
	rotEye.y += GetMouseX() * CAMERAEYEROT;

	// �L�[�{�[�h����
	// X����]�i�㉺��]�j
	if (GetKeyboardPress(DIK_UP) && rotEye.x < ROT_X_LIMIT)
	{
		rotEye.x += CAMERAATROT;
	}
	if (GetKeyboardPress(DIK_DOWN) && rotEye.x > -ROT_X_LIMIT)
	{
		rotEye.x -= CAMERAATROT;
	}
	// Y����]�i���E��]�j
	if (GetKeyboardPress(DIK_LEFT))
	{
		rotEye.y -= CAMERAATROT;
		rotEye.y = Rotation_Correction(rotEye.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		rotEye.y += CAMERAATROT;
		rotEye.y = Rotation_Correction(rotEye.y);
	}
}

// CameraAt�̈ړ�
void CameraFP::CameraAt_Move()
{
	posAt.y = posEye.y + lenEyeToAt * sinf(D3DXToRadian(rotEye.x));
	posAt.x = posEye.x + lenEyeToAt * sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
	posAt.z = posEye.z + lenEyeToAt * cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
}

// Camera��]�̃��Z�b�g�l�̃Z�b�g
void CameraFP::Set_Rot_Reset_Value()
{
	if (!bSetValue)
	{
		rot_reset_x = (rotEye.x - rotEyeDef.x) / ROT_RESET_FRAME;
		rot_reset_y = (rotEye.y - rotEyeDef.y) / ROT_RESET_FRAME;
		bSetValue = true;
	}
}

// Camera��]�̃��Z�b�g
void CameraFP::Rot_Reset()
{
	rotEye.x = Angle_To_Target(rotEye.x, rot_reset_x, rotEyeDef.x);
	rotEye.y = Angle_To_Target(rotEye.y, rot_reset_y, rotEyeDef.y);
}

// �T�[�h�p�[�\��Camera
// Camera�̏�����
void CameraTP::Initialize(D3DXVECTOR3 p, D3DXVECTOR3 pc)
{
	posEye = posEyeDef = D3DXVECTOR3(p.x + pc.x, p.y + pc.y, p.z + pc.z);
	posAt = posAtDef = D3DXVECTOR3(p.x, p.y, p.z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	direction = posAt - posEye; // Camera�̌����̌v�Z
	D3DXVec3Normalize(&direction, &direction); // Camera�̌����̐��K��
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotAt = rotAtDef = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Camera�̉�]
}

// Camera�̏I������
void CameraTP::Finalize()
{
	posEye = posEyeDef;
	posAt = posAtDef;
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotAt = rotAtDef;
}

// Camera�̍X�V
void CameraTP::Update(D3DXVECTOR3 p)
{
	// CameraAt�̈ړ�
	CameraAt_Move(p);

	// CameraAt�̉�]�i���]�j
	CameraAt_Rot();

	// CameraEye�̈ړ�
	CameraEye_Move();
}

// CameraAt�̈ړ�
void CameraTP::CameraAt_Move(D3DXVECTOR3 p)
{
	posAt = p;
}

// CameraAt�̉�]�i���]�j
void CameraTP::CameraAt_Rot()
{
	// X����]�i�㉺��]�j
	if (GetKeyboardPress(DIK_UP) && rotAt.x > -ROT_X_LIMIT)
	{
		rotAt.x -= CAMERAATROT;
	}
	if (GetKeyboardPress(DIK_DOWN) && rotAt.x < ROT_X_LIMIT)
	{
		rotAt.x += CAMERAATROT;
	}
	// Y����]�i���E��]�j
	if (GetKeyboardPress(DIK_LEFT))
	{
		rotAt.y -= CAMERAATROT;
		rotAt.y = Rotation_Correction(rotAt.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		rotAt.y += CAMERAATROT;
		rotAt.y = Rotation_Correction(rotAt.y);
	}
}

// CameraEye�̈ړ�
void CameraTP::CameraEye_Move()
{
	posEye.y = posAt.y - lenEyeToAt * sinf(D3DXToRadian(rotAt.x));
	posEye.x = posAt.x - lenEyeToAt * sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
	posEye.z = posAt.z - lenEyeToAt * cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
}