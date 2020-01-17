#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"
#include "ui.h"

#ifndef AIMING_H_
#define AIMING_H_

#define TIME_COUNT_MAX 15.0f

enum AimingState {
	AIMING_STATE_NONE,
	AIMING_STATE_FREE,
	AIMING_STATE_PREPARE,
	AIMING_STATE_ZOOM_FORWARD,
	AIMING_STATE_ZOOM_BACKWARD,
	AIMING_STATE_CHECK_ARROW,
};

class Aiming2D
{
public:
	TextureIndex  texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR2   pos;           // ���݈ʒu
	D3DCOLOR      color;	     // �J���[
	int           tx, ty;
	int           tw, th;

	D3DXVECTOR3   posHit; // Camera����o�����ː��������������W
	Number        *num;   // ���ԕ\��
	Alphabet      *alpha; // �����\��
	Timer         *timer; // Timer
	float         zoomStart; // Zoom�J�n�ʒu

	AimingState   state;   // Aiming�̏��

	static double timeCnt; // �J�E���g�_�E��

	Aiming2D();  // Aiming2D�̏������i�R���X�g���N�^�j
	~Aiming2D(); // Aiming2D�̏I�������i�f�X�g���N�^�j

	void Initialize(); // Aiming2D�̏�����
	void Finalize();   // Aiming2D�̏I������
	void Update();     // Aiming2D�̍X�V
	void Draw();       // Aiming2D�̕`��
	void Draw_Timer(); // Timer�̕\��

	void Free();          // �t���[���
	void Prepare();       // �\�����
	void Zoom_Forward(float start, float zm = ZOOM_MAX, float zi = ZOOM_INCREASING);  // �O�iZoom�҂�
	void Zoom_Backward(float start, float zm = ZOOM_MAX, float zi = ZOOM_INCREASING); // ���Zoom�҂�
};

class Aiming3D
{
public:
	TextureIndex texture_index; // �e�N�X�`���C���f�b�N�X
	D3DXVECTOR3  pos;           // ���݈ʒu
	D3DXVECTOR3  rot;           // ��]
	D3DXVECTOR3  rot_Camera;    // �J������]�̕␳
	D3DXMATRIX   mtxWorld;      // ���[���h�}�g���b�N�X
	Face         *face;         // Face

	D3DXVECTOR3  posHit; // Camera����o�����ː��������������W

	bool         prepare; // �\���t���O

	Aiming3D();  // Aiming3D�̏������i�R���X�g���N�^�j
	~Aiming3D(); // Aiming3D�̏I�������i�f�X�g���N�^�j

	void Initialize(); // Aiming3D�̏�����
	void Finalize();   // Aiming3D�̏I������
	void Update();     // Aiming3D�̍X�V
	void Draw();       // Aiming3D�̕`��

	D3DXVECTOR3 Pos_With_Camera(); // Camera�ɂ��ʒu
};
#endif