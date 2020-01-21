#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"

float Rotation_Correction(float r); // 回転角度の補正
float Get_Length(D3DXVECTOR3 p1, D3DXVECTOR3 p2); // 点と点の距離の取得
float Angle_To_Target(float r, float ri, float t, float deviation = 0.0f); // 角度の逓増逓減によるTargetになる判定
D3DXVECTOR3 RayHit(D3DXVECTOR3 p, D3DXVECTOR2 r, float l); // 射線が当たった座標の取得
D3DXVECTOR3 Get_PointXYZ_OnLine(D3DXVECTOR3 p, D3DXVECTOR3 r, float l); // 指定の点と同じ直線にある点の座標の取得