#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"

// 円判定を行う
bool HitCheck(float x1, float y1, float radius1, float x2, float y2, float radius2);
bool HitCheck3D(D3DXVECTOR3 p1, float radius1, D3DXVECTOR3 p2, float radius2);