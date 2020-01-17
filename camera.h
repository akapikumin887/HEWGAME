#pragma once
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
#define CAMERAEYE_Y 4.0f
#define CAMERAEYE_Z -15.0f
#define CAMERAAT_X 0.0f
#define CAMERAAT_Y 2.5f
#define CAMERAAT_Z 1.5f
#define EYETOATLEN sqrtf(powf(posEye.x - posAt.x, 2.0f) + powf(posEye.y - posAt.y, 2.0f) + powf(posEye.z - posAt.z, 2.0f))
#define CAMERAEYEROT 0.125f
#define CAMERAATROT 1.0f
#define ROT_X_LIMIT (90.0f - CAMERAATROT)
#define CAMERA_MOVE_SPEED 0.1f
#define CAMERA_ZOOM_SPEED 0.005f
#define ZOOM_MAX 6.0f
#define ZOOM_INCREASING 0.25f
#define ROT_RESET_FRAME 10

class Camera
{
public:
	D3DXMATRIX mtxView;       // ビューマトリックス
	D3DXMATRIX mtxProjection; // プロジェクションマトリックス
	D3DXVECTOR3 posEye;       // Cameraの視点
	D3DXVECTOR3 posAt;        // Cameraの注視点
	D3DXVECTOR3 vecUp;        // Cameraの上方向
	D3DXVECTOR3 rotEye;       // CameraEyeの回転
	D3DXVECTOR3 rotAt;        // CameraAtの回転
	D3DXVECTOR3 direction;    // Cameraの向き
	float lenEyeToAt;         // Camera位置と注視点の距離
	bool bZoom;               // Zoomフラグ
	bool bSetValue;           // リセット値セットフラグ
	float rot_reset_x;        // Camera回転のリセット値
	float rot_reset_y;        // Camera回転のリセット値

	D3DXVECTOR3 rotEye_init;  // 初期状態のCameraEyeの回転

	Camera(); // Cameraの初期化（コンストラクタ）
	
	void Draw(); // Cameraの描画

	// リセット処理
	void Set_Rot_Reset_Value(); // Camera回転のリセット値のセット
	void Rot_Reset(); // カメラ回転のリセット

	// Cameraズーム処理
	void Zoom_Foward(float zi = ZOOM_INCREASING);   // Cameraのズーム前進
	void Zoom_Backward(float zi = ZOOM_INCREASING); // Cameraのズーム後退

};

// ファーストパーソンCamera
class CameraFP :public Camera
{
public:
	void Initialize();     // Cameraの初期化
	void Finalize();       // Cameraの終了処理
	void Update();         // Cameraの更新
	void Camera_Reset();   // Cameraのリセット
	void CameraEye_Move(); // CameraEyeの移動
	void CameraEye_Rot();  // CameraEyeの回転（自転）
	void CameraAt_Move();  // CameraAtの移動
};

class CameraTP :public Camera
{
public:
	void Initialize(D3DXVECTOR3 p); // Cameraの初期化
	void Finalize(D3DXVECTOR3 p); // Cameraの終了処理
	void Update(D3DXVECTOR3 p); // Cameraの更新
	void CameraAt_Move(D3DXVECTOR3 p); // CameraAtの移動
	void CameraAt_Rot(); // CameraAtの回転（自転）
	void CameraEye_Move(); // CameraEyeの移動
};
#endif