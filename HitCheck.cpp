#include "hitcheck.h"

//====================================================
// �~�̓����蔻����s��
// ������: true     �͂���: false
//====================================================
bool HitCheck(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
	float  len;
	float  radius;
	float x, y;

	// ���݂��̔��a�̍��v
	radius = radius1 + radius2;
	radius *= radius;

	// ���݂��̒��S���m�̋���
	x = x2 - x1;
	y = y2 - y1;
	len = x * x + y * y;

	// ����
	if (radius > len)
	{
		return true; // ������
	}

	return false; // �͂���
}

bool HitCheck3D(D3DXVECTOR3 p1, float radius1, D3DXVECTOR3 p2, float radius2)
{
	float len;
	float radius;
	D3DXVECTOR3 p;

	// ���݂��̔��a�̍��v
	radius = radius1 + radius2;
	radius *= radius;

	// ���݂��̒��S���m�̋���
	p = p2 - p1;
	len = p.x * p.x + p.y * p.y + p.z * p.z;

	// ����
	if (radius > len)
	{
		return true; // ������
	}

	return false; // �͂���
}