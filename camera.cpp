#include "camera.h"
#include "input.h"
#include "debug_font.h"

static Camera* camera;

// �v���W�F�N�V�������J�����s��
static D3DXMATRIX g_mtxView; // �r���[�}�g���b�N�X
static D3DXMATRIX g_mtxProjection; // �v���W�F�N�V�����}�g���b�N�X

// Camera�̏�����
void Camera_Initialize()
{
	camera = new Camera;
}

// Camera�̏I������
void Camera_Finalize()
{
	delete camera;
}

// Camera�̍X�V
void Camera_Update()
{
	// Camera�̃��Z�b�g
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		camera->CameraReset();
	}

	// Camera�����_�̑���
	// X����]�i�㉺��]�j
	if (Keyboard_IsPress(DIK_UP) && camera->rotAt.x < ROT_X_LIMIT)
	{
		camera->rotAt.x += CAMERAATROT;
	}
	if (Keyboard_IsPress(DIK_DOWN) && camera->rotAt.x > -ROT_X_LIMIT)
	{
		camera->rotAt.x -= CAMERAATROT;
	}
	// Y����]�i���E��]�j
	if (Keyboard_IsPress(DIK_LEFT))
	{
		camera->rotAt.y -= CAMERAATROT;
		camera->rotAt.y = camera->Rotation_Correction(camera->rotAt.y);
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{
		camera->rotAt.y += CAMERAATROT;
		camera->rotAt.y = camera->Rotation_Correction(camera->rotAt.y);
	}

	// Camera�ʒu�̑���
	// Z���ړ��i�O��j�ESPACE��������Ă����Y���ړ��i�㉺�j
	if (Keyboard_IsPress(DIK_W))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera->posEye.y += CAMERA_MOVE_SPEED;
		}
		else
		{
			camera->posEye.x += sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
			camera->posEye.z += cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	if (Keyboard_IsPress(DIK_S))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera->posEye.y -= CAMERA_MOVE_SPEED;
		}
		else
		{
			camera->posEye.x -= sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
			camera->posEye.z -= cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	// X���ړ��i���E�j
	if (Keyboard_IsPress(DIK_A))
	{
		camera->posEye.x -= cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		camera->posEye.z += sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		camera->posEye.x += cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		camera->posEye.z -= sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
	}

	// Camera�����_�̍X�V
	camera->posAt.y = camera->posEye.y + camera->lenEyeToAt * sinf(D3DXToRadian(camera->rotAt.x));
	camera->posAt.x = camera->posEye.x + camera->lenEyeToAt * sinf(D3DXToRadian(camera->rotAt.y)) * cosf(D3DXToRadian(camera->rotAt.x));
	camera->posAt.z = camera->posEye.z + camera->lenEyeToAt * cosf(D3DXToRadian(camera->rotAt.y)) * cosf(D3DXToRadian(camera->rotAt.x));
}

// Camera�̕`��
void Camera_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �r���[�s��̍쐬
	D3DXMatrixLookAtLH(&g_mtxView,
		&camera->posEye, // �J�����̈ʒu
		&camera->posAt,  // �J�����̒����_
		&camera->vecUp); // �J�����̏����

	// �r���[�s��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// �v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,  // �r���[���ʂ̎���p
		VIEW_ASPECT, // �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z, // �r���[���ʂ�NearZ�l
		VIEW_FAR_Z); // �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����s��̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", camera->posEye.x, camera->posEye.y, camera->posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", camera->posAt.x, camera->posAt.y, camera->posAt.z);
	DebugFont_Draw(2, 62, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", camera->rotAt.x, camera->rotAt.y, camera->rotAt.z);
}

// Camera�̏������i�R���X�g���N�^�j
Camera::Camera()
{
	rotAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	lenEyeToAt = EYETOATLEN;
}

// Camera�̃��Z�b�g
void Camera::CameraReset()
{
	rotAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
}

// ��]�p�x�̕␳
float Camera::Rotation_Correction(float r)
{
	if (r > 360.0f)
	{
		r -= 360.0f;
	}
	if (r < 0.0f)
	{
		r += 360.0f;
	}
	return r;
}