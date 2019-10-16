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