#include "camera.h"
#include "input.h"
#include "debug_font.h"
#include "cube.h"
#include "target.h"
#include "aiming.h"

static Camera camera;

// Camera�̏�����
void Camera_Initialize()
{
	camera.Initialize();
}

// Camera�̏I������
void Camera_Finalize()
{
	camera.Finalize();
}

// Camera�̍X�V
void Camera_Update()
{
	camera.Update();
}

// Camera�̕`��
void Camera_Draw()
{
	camera.Draw();

	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", camera.posEye.x, camera.posEye.y, camera.posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", camera.posAt.x, camera.posAt.y, camera.posAt.z);
	DebugFont_Draw(2, 62, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", camera.rotEye.x, camera.rotEye.y, camera.rotEye.z);
	//DebugFont_Draw(640, 32, "CameraView: Xx: %.02lf Xy: %.02lf Xz: %.02lf", camera.mtxView._11, camera.mtxView._12, camera.mtxView._13);
	//DebugFont_Draw(640, 62, "CameraView: Yx: %.02lf Yy :%.02lf Yz: %.02lf", camera.mtxView._21, camera.mtxView._22, camera.mtxView._23);
	//DebugFont_Draw(640, 92, "CameraView: Zx: %.02lf Zy :%.02lf Zz: %.02lf", camera.mtxView._31, camera.mtxView._32, camera.mtxView._33);
	//DebugFont_Draw(640, 122, "CameraView: Tx: %.02lf Ty: %.02lf Tz: %.02lf", camera.mtxView._41, camera.mtxView._42, camera.mtxView._43);
}

Camera* Get_Camera()
{
	return &camera;
}

// �����o�[�֐��̒�`

// Camera�̏������i�R���X�g���N�^�j
Camera::Camera()
{
	
}

// Camera�̏�����
void Camera::Initialize()
{
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	//posEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	direction = posAt - posEye; // Camera�̌����̌v�Z
	D3DXVec3Normalize(&direction, &direction); // Camera�̌����̐��K��
	rotEye = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Camera�̉�]
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	lenEyeToAt = EYETOATLEN;
	bZoom_Forward = false;
	bZoom_Back = false;
	zoom_cnt = 0.0f;
}

// Camera�̏I������
void Camera::Finalize()
{

}

// Camera�̍X�V
void Camera::Update()
{
	// �t�@�[�X�g�p�[�\��
	// Camera�̃��Z�b�g
	Camera_Reset();

	//// CameraEye�̉�]�i���]�j
	CameraEye_Rot();

	// CameraEye�̈ړ�
	CameraEye_Move();

	// CameraAt�̈ړ�
	CameraAt_Move();

	// �T�[�h�p�[�\��

}

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

	DebugFont_Draw(2, 2, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", rotAt.x, rotAt.y, rotAt.z);
}

// Camera�̃��Z�b�g
void Camera::Camera_Reset()
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		Initialize();
	}
}

// CameraEye�̈ړ�
void Camera::CameraEye_Move()
{
	// Y���ړ��i�㉺�j
	if (GetKeyboardPress(DIK_W))
	{
		posEye.y += CAMERA_MOVE_SPEED;
	}

	if (GetKeyboardPress(DIK_S))
	{
		posEye.y -= CAMERA_MOVE_SPEED;
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

	// Camera�Y�[���i�O��j
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
}

// CameraEye�̉�]�i���]�j
void Camera::CameraEye_Rot()
{
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
void Camera::CameraAt_Move()
{
	posAt.y = posEye.y + lenEyeToAt * sinf(D3DXToRadian(rotEye.x));
	posAt.x = posEye.x + lenEyeToAt * sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
	posAt.z = posEye.z + lenEyeToAt * cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
}

// Camera�̃Y�[���O�i
void Camera::Camera_Zoom_Forward(float zm, float zi)
{
	Aiming *aiming;
	aiming = Get_Aiming();

	// �\�����̃Y�[��
	if (aiming->prepare && zoom_cnt < zm)
	{
		Reset_Zoom();
		zoom_cnt += zi;
		Set_Zoom();
	}
	// ��I�ɂ������Ƃ��̃Y�[��
	else if (bZoom_Forward)
	{
		Reset_Zoom();
		zoom_cnt += zi;
		if (zoom_cnt > zm)
		{
			zoom_cnt = zm;
		}
		Set_Zoom();
		if (GetKeyboardTrigger(DIK_SPACE))
		{
			bZoom_Forward = false;
			bZoom_Back = true;
			Cube::bFlying = false;
		}
	}
}

// Camera�̃Y�[�����
void Camera::Camera_Zoom_Back(float zm, float zi)
{
	if (bZoom_Back)
	{
		Reset_Zoom();
		zoom_cnt -= zi;
		if (zoom_cnt <= zm)
		{
			zoom_cnt = zm;
			bZoom_Back = false;
		}
		Set_Zoom();
	}
}

// CameraEye�̃Z�b�g
void Camera::Set_Zoom()
{
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
}

// CameraEye�̃��Z�b�g
void Camera::Reset_Zoom()
{
	posEye.y -= sinf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.x -= sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.z -= cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
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

void Camera::Initialize(D3DXVECTOR3 p)
{
	posEye = D3DXVECTOR3(p.x, p.y + 5.0f, p.z - 50.0f);
	posAt = D3DXVECTOR3(p.x, p.y, p.z);
	direction = posAt - posEye; // Camera�̌����̌v�Z
	lenEyeToAt = sqrtf(powf(direction.x, 2.0f) + powf(direction.y, 2.0f) + powf(direction.z, 2.0f));
	D3DXVec3Normalize(&direction, &direction); // Camera�̌����̐��K��
	rotAt = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // �����ɂ��Camera�̉�]
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	bZoom_Forward = false;
	bZoom_Back = false;
	zoom_cnt = 0.0f;
}

void Camera::Finalize(D3DXVECTOR3 p)
{
	Initialize(p);
}

void Camera::Update(D3DXVECTOR3 p)
{

}

void Camera::CameraAt_Rot()
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

	posEye.y = posAt.y - lenEyeToAt * sinf(D3DXToRadian(rotAt.x));
	posEye.x = posAt.x - lenEyeToAt * sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
	posEye.z = posAt.z - lenEyeToAt * cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
}