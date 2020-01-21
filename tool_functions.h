#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"

float Rotation_Correction(float r); // ��]�p�x�̕␳
float Get_Length(D3DXVECTOR3 p1, D3DXVECTOR3 p2); // �_�Ɠ_�̋����̎擾
float Angle_To_Target(float r, float ri, float t, float deviation = 0.0f); // �p�x�̒��������ɂ��Target�ɂȂ锻��
D3DXVECTOR3 RayHit(D3DXVECTOR3 p, D3DXVECTOR2 r, float l); // �ː��������������W�̎擾
D3DXVECTOR3 Get_PointXYZ_OnLine(D3DXVECTOR3 p, D3DXVECTOR3 r, float l); // �w��̓_�Ɠ��������ɂ���_�̍��W�̎擾