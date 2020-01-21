#include "hitcheck.h"

//====================================================
// ‰~‚Ì“–‚½‚è”»’è‚ðs‚¤
// ‚ ‚½‚è: true     ‚Í‚¸‚ê: false
//====================================================
bool HitCheck(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
	float  len;
	float  radius;
	float x, y;

	// ‚¨ŒÝ‚¢‚Ì”¼Œa‚Ì‡Œv
	radius = radius1 + radius2;
	radius *= radius;

	// ‚¨ŒÝ‚¢‚Ì’†S“¯Žm‚Ì‹——£
	x = x2 - x1;
	y = y2 - y1;
	len = x * x + y * y;

	// ”»’è
	if (radius > len)
	{
		return true; // ‚ ‚½‚è
	}

	return false; // ‚Í‚¸‚ê
}

bool HitCheck3D(D3DXVECTOR3 p1, float radius1, D3DXVECTOR3 p2, float radius2)
{
	float len;
	float radius;
	D3DXVECTOR3 p;

	// ‚¨ŒÝ‚¢‚Ì”¼Œa‚Ì‡Œv
	radius = radius1 + radius2;
	radius *= radius;

	// ‚¨ŒÝ‚¢‚Ì’†S“¯Žm‚Ì‹——£
	p = p2 - p1;
	len = p.x * p.x + p.y * p.y + p.z * p.z;

	// ”»’è
	if (radius > len)
	{
		return true; // ‚ ‚½‚è
	}

	return false; // ‚Í‚¸‚ê
}