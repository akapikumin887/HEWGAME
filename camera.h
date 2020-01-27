#pragma once
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

// �v���W�F�N�V�����p�p�����[�^
#define VIEW_ANGLE (D3DXToRadian(45.0f)) // �r���[���ʂ̎���p
#define VIEW_ASPECT ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) // �r���[���ʂ̃A�X�y�N�g��
#define VIEW_NEAR_Z (1.0f) // �r���[���ʂ�NearZ�l
#define VIEW_FAR_Z (1000.0f) // �r���[���ʂ�FarZ�l

#define CAMERAEYE_X 1.0f
#define CAMERAEYE_Y 1.0f
#define CAMERAEYE_Z -27.2f
#define CAMERAAT_X 0.0f
#define CAMERAAT_Y 0.0f
#define CAMERAAT_Z 0.0f
#define EYETOATLEN sqrtf(powf(posEye.x - posAt.x, 2.0f) + powf(posEye.y - posAt.y, 2.0f) + powf(posEye.z - posAt.z, 2.0f))
#define CAMERAEYEROT 0.125f
#define CAMERAATROT 1.0f
#define ROT_X_LIMIT (90.0f - CAMERAATROT)
#define CAMERA_MOVE_SPEED 0.1f
#define CAMERA_ZOOM_SPEED 0.005f
#define ROT_RESET_FRAME 10

class Camera
{
public:
	D3DXMATRIX mtxView;       // �r���[�}�g���b�N�X
	D3DXMATRIX mtxProjection; // �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 posEye;       // Camera�̎��_
	D3DXVECTOR3 posEyeDef;    // Camera�̎��_Default
	D3DXVECTOR3 posAt;        // Camera�̒����_
	D3DXVECTOR3 posAtDef;     // Camera�̒����_Default
	D3DXVECTOR3 vecUp;        // Camera�̏����
	D3DXVECTOR3 direction;    // Camera�̌���
	float       lenEyeToAt;   // Camera�ʒu�ƒ����_�̋���
	bool        bZoom;        // Zoom�t���O
	
	void Draw(); // Camera�̕`��

	// Camera�Y�[������
	void Zoom_Foward(float zi);   // Camera�̃Y�[���O�i
	void Zoom_Backward(float zi); // Camera�̃Y�[�����
};

// �t�@�[�X�g�p�[�\��Camera
class CameraFP :public Camera
{
public:
	D3DXVECTOR3 rotEye;    // CameraEye�̉�]
	D3DXVECTOR3 rotEyeDef; // CameraEye�̉�]Default
	bool bSetValue;        // ���Z�b�g�l�Z�b�g�t���O
	float rot_reset_x;     // Camera��]�̃��Z�b�g�l
	float rot_reset_y;     // Camera��]�̃��Z�b�g�l

	void Initialize(D3DXVECTOR3 peye, D3DXVECTOR3 pat); // Camera�̏�����
	void Finalize();       // Camera�̏I������
	void Update();         // Camera�̍X�V

	void Camera_Reset();   // Camera�̃��Z�b�g
	void CameraEye_Move(); // CameraEye�̈ړ�
	void CameraEye_Rot();  // CameraEye�̉�]�i���]�j
	void CameraAt_Move();  // CameraAt�̈ړ�

	// ���Z�b�g����
	void Set_Rot_Reset_Value(); // Camera��]�̃��Z�b�g�l�̃Z�b�g
	void Rot_Reset();           // �J������]�̃��Z�b�g
};

class CameraTP :public Camera
{
public:
	D3DXVECTOR3 rotAt;    // CameraAt�̉�]
	D3DXVECTOR3 rotAtDef; // CameraAt�̉�]Default

	void Initialize(D3DXVECTOR3 p, D3DXVECTOR3 pc); // Camera�̏�����
	void Finalize();            // Camera�̏I������
	void Update(D3DXVECTOR3 p); // Camera�̍X�V

	void CameraAt_Move(D3DXVECTOR3 p); // CameraAt�̈ړ�
	void CameraAt_Rot();               // CameraAt�̉�]�i���]�j
	void CameraEye_Move();             // CameraEye�̈ړ�
};