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