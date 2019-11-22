#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef CAMERA_H_
#define CAMERA_H_

// プロジェクション用パラメータ
#define VIEW_ANGLE (D3DXToRadian(45.0f)) // ビュー平面の視野角
#define VIEW_ASPECT ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) // ビュー平面のアスペクト比
#define VIEW_NEAR_Z (1.0f) // ビュー平面のNearZ値
#define VIEW_FAR_Z (1000.0f) // ビュー平面のFarZ値

#define CAMERAEYE_X 0.0f
#define CAMERAEYE_Y 4.5f
#define CAMERAEYE_Z -10.0f
#define CAMERAAT_X 0.0f
#define CAMERAAT_Y 3.5f
#define CAMERAAT_Z 1.5f
#define EYETOATLEN sqrtf(powf(CAMERAEYE_X - CAMERAAT_X, 2.0f) + powf(CAMERAEYE_Y - CAMERAAT_Y, 2.0f) + powf(CAMERAEYE_Z - CAMERAAT_Z, 2.0f))
#define CAMERAATROT 1.0f
#define ROT_X_LIMIT (90.0f - CAMERAATROT)
#define CAMERA_MOVE_SPEED 0.5f

class Camera
{
public:
	D3DXVECTOR3 posEye; // Cameraの視点
	D3DXVECTOR3 posAt; // Cameraの注視点
	D3DXVECTOR3 vecUp; // Cameraの上方向
	D3DXVECTOR3 rotAt; // CameraEyeの回転
	float lenEyeToAt; // Camera位置と注視点の距離

	Camera(); // Cameraの初期化（コンストラクタ）
	void CameraReset(); // Cameraのリセット
	float Rotation_Correction(float r); // 回転角度の補正
};

void Camera_Initialize(); // Cameraの初期化
void Camera_Finalize(); // Cameraの終了処理
void Camera_Update(); // Cameraの更新
void Camera_Draw(); // Cameraの描画
#endif