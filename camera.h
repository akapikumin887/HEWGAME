#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef CAMERA_H_
#define CAMERA_H_

// �v���W�F�N�V�����p�p�����[�^
#define VIEW_ANGLE (D3DXToRadian(45.0f)) // �r���[���ʂ̎���p
#define VIEW_ASPECT ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) // �r���[���ʂ̃A�X�y�N�g��
#define VIEW_NEAR_Z (1.0f) // �r���[���ʂ�NearZ�l
#define VIEW_FAR_Z (1000.0f) // �r���[���ʂ�FarZ�l

#define CAMERAEYE_X 0.0f
#define CAMERAEYE_Y 4.5f
#define CAMERAEYE_Z -10.0f
#define CAMERAAT_X 0.0f
#define CAMERAAT_Y 3.5f
#define CAMERAAT_Z 1.5f
#define EYETOATLEN sqrtf(powf(CAMERAEYE_X - CAMERAAT_X, 2.0f) + powf(CAMERAEYE_Y - CAMERAAT_Y, 2.0f) + powf(CAMERAEYE_Z - CAMERAAT_Z, 2.0f))
#define CAMERAATROT 1.0f
#define ROT_X_LIMIT (90.0f - CAMERAATROT)
#define CAMERA_MOVE_SPEED 0.5f

class Camera
{
public:
	D3DXVECTOR3 posEye; // Camera�̎��_
	D3DXVECTOR3 posAt; // Camera�̒����_
	D3DXVECTOR3 vecUp; // Camera�̏����
	D3DXVECTOR3 rotAt; // CameraEye�̉�]
	float lenEyeToAt; // Camera�ʒu�ƒ����_�̋���

	Camera(); // Camera�̏������i�R���X�g���N�^�j
	void CameraReset(); // Camera�̃��Z�b�g
	float Rotation_Correction(float r); // ��]�p�x�̕␳
};

void Camera_Initialize(); // Camera�̏�����
void Camera_Finalize(); // Camera�̏I������
void Camera_Update(); // Camera�̍X�V
void Camera_Draw(); // Camera�̕`��
#endif