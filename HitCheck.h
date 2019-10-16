#include<windows.h>
#include<math.h>	//sin cosを使うのに必要
#include"common.h"

#ifndef _HIT_CHECK_H
#define _HIT_CHECK_H

// 円判定を行う
bool HitCheck(float x1, float y1, float radius1, float x2, float y2, float radius2);
#endif