#include "hitcheck.h"

//====================================================
// 円の当たり判定を行う
// あたり: true     はずれ: false
//====================================================
bool HitCheck(float x1, float y1, float radius1, float x2, float y2, float radius2)
{
	float  len;
	float  radius;
	float x, y;

	// お互いの半径の合計
	radius = radius1 + radius2;
	radius *= radius;

	// お互いの中心同士の距離
	x = x2 - x1;
	y = y2 - y1;
	len = x * x + y * y;

	// 判定
	if (radius > len)
	{
		return true; // あたり
	}

	return false; // はずれ
}