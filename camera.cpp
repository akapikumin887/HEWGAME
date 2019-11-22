#include "camera.h"
#include "input.h"
#include "debug_font.h"

static Camera* camera;

// プロジェクション＆カメラ行列
static D3DXMATRIX g_mtxView; // ビューマトリックス
static D3DXMATRIX g_mtxProjection; // プロジェクションマトリックス

// Cameraの初期化
void Camera_Initialize()
{
	camera = new Camera;
}

// Cameraの終了処理
void Camera_Finalize()
{
	delete camera;
}

// Cameraの更新
void Camera_Update()
{
	// Cameraのリセット
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		camera->CameraReset();
	}

	// Camera注視点の操作
	// X軸回転（上下回転）
	if (Keyboard_IsPress(DIK_UP) && camera->rotAt.x < ROT_X_LIMIT)
	{
		camera->rotAt.x += CAMERAATROT;
	}
	if (Keyboard_IsPress(DIK_DOWN) && camera->rotAt.x > -ROT_X_LIMIT)
	{
		camera->rotAt.x -= CAMERAATROT;
	}
	// Y軸回転（左右回転）
	if (Keyboard_IsPress(DIK_LEFT))
	{
		camera->rotAt.y -= CAMERAATROT;
		camera->rotAt.y = camera->Rotation_Correction(camera->rotAt.y);
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{
		camera->rotAt.y += CAMERAATROT;
		camera->rotAt.y = camera->Rotation_Correction(camera->rotAt.y);
	}

	// Camera位置の操作
	// Z軸移動（前後）・SPACEが押されている間Y軸移動（上下）
	if (Keyboard_IsPress(DIK_W))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera->posEye.y += CAMERA_MOVE_SPEED;
		}
		else
		{
			camera->posEye.x += sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
			camera->posEye.z += cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	if (Keyboard_IsPress(DIK_S))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera->posEye.y -= CAMERA_MOVE_SPEED;
		}
		else
		{
			camera->posEye.x -= sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
			camera->posEye.z -= cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	// X軸移動（左右）
	if (Keyboard_IsPress(DIK_A))
	{
		camera->posEye.x -= cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		camera->posEye.z += sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		camera->posEye.x += cosf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
		camera->posEye.z -= sinf(D3DXToRadian(camera->rotAt.y)) * CAMERA_MOVE_SPEED;
	}

	// Camera注視点の更新
	camera->posAt.y = camera->posEye.y + camera->lenEyeToAt * sinf(D3DXToRadian(camera->rotAt.x));
	camera->posAt.x = camera->posEye.x + camera->lenEyeToAt * sinf(D3DXToRadian(camera->rotAt.y)) * cosf(D3DXToRadian(camera->rotAt.x));
	camera->posAt.z = camera->posEye.z + camera->lenEyeToAt * cosf(D3DXToRadian(camera->rotAt.y)) * cosf(D3DXToRadian(camera->rotAt.x));
}

// Cameraの描画
void Camera_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ビュー行列の作成
	D3DXMatrixLookAtLH(&g_mtxView,
		&camera->posEye, // カメラの位置
		&camera->posAt,  // カメラの注視点
		&camera->vecUp); // カメラの上方向

	// ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,  // ビュー平面の視野角
		VIEW_ASPECT, // ビュー平面のアスペクト比
		VIEW_NEAR_Z, // ビュー平面のNearZ値
		VIEW_FAR_Z); // ビュー平面のFarZ値

	// プロジェクション行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);

	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", camera->posEye.x, camera->posEye.y, camera->posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", camera->posAt.x, camera->posAt.y, camera->posAt.z);
	DebugFont_Draw(2, 62, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", camera->rotAt.x, camera->rotAt.y, camera->rotAt.z);
}

// Cameraの初期化（コンストラクタ）
Camera::Camera()
{
	rotAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	lenEyeToAt = EYETOATLEN;
}

// Cameraのリセット
void Camera::CameraReset()
{
	rotAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
}

// 回転角度の補正
float Camera::Rotation_Correction(float r)
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