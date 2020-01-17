#include "tool_functions.h"

// ��]�p�x�̕␳
float Rotation_Correction(float r)
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

// �_�Ɠ_�̋����̎擾
float Get_Length(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{
	return sqrtf(powf(p1.x - p2.x, 2.0f) + powf(p1.y - p2.y, 2.0f) + powf(p1.z - p2.z, 2.0f));
}

// �p�x�̒��������ɂ��Target�ɂȂ锻��
float Angle_To_Target(float r, float ri, float t, float deviation)
{
	if (fabsf(r - ri - t) <= 0.1f + deviation)
	{
		return t;
	}
	else
	{
		return r - ri;
	}
}

// �ː��������������W�̎擾
D3DXVECTOR3 RayHit(D3DXVECTOR3 p, D3DXVECTOR2 r, float l)
{
	D3DXVECTOR3 posHit;

	posHit.x = p.x + l / cosf(D3DXToRadian(r.y)) * sinf(D3DXToRadian(r.y));
	posHit.y = p.y + l / cosf(D3DXToRadian(r.x)) * sinf(D3DXToRadian(r.x));
	posHit.z = p.z;

	return posHit;
}

// �w��̓_�Ɠ��������ɂ���_�̍��W�̎擾
D3DXVECTOR3 Get_PointXYZ_OnLine(D3DXVECTOR3 p, D3DXVECTOR3 r, float l)
{
	D3DXVECTOR3 pos;

	pos.y = p.y - l * sinf(D3DXToRadian(r.x));
	pos.x = p.x + l * sinf(D3DXToRadian(r.y)) * cosf(D3DXToRadian(r.x));
	pos.z = p.z + l * cosf(D3DXToRadian(r.y)) * cosf(D3DXToRadian(r.x));

	return pos;
}