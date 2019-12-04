#include "camera.h"
#include "input.h"
#include "debug_font.h"

static Camera camera;

// Cameraの初期化
void Camera_Initialize()
{

}

// Cameraの終了処理
void Camera_Finalize()
{
}

// Cameraの更新
void Camera_Update()
{
	// Cameraのリセット
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		camera.CameraReset();
	}

	// Camera注視点の操作
	// X軸回転（上下回転）
	if (Keyboard_IsPress(DIK_UP) && camera.rotAt.x < ROT_X_LIMIT)
	{
		camera.rotAt.x += CAMERAATROT;
	}
	if (Keyboard_IsPress(DIK_DOWN) && camera.rotAt.x > -ROT_X_LIMIT)
	{
		camera.rotAt.x -= CAMERAATROT;
	}
	// Y軸回転（左右回転）
	if (Keyboard_IsPress(DIK_LEFT))
	{
		camera.rotAt.y -= CAMERAATROT;
		camera.rotAt.y = camera.Rotation_Correction(camera.rotAt.y);
	}
	if (Keyboard_IsPress(DIK_RIGHT))
	{
		camera.rotAt.y += CAMERAATROT;
		camera.rotAt.y = camera.Rotation_Correction(camera.rotAt.y);
	}

	// Camera位置の操作
	// Z軸移動（前後）・SPACEが押されている間Y軸移動（上下）
	if (Keyboard_IsPress(DIK_W))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera.posEye.y += CAMERA_MOVE_SPEED;
		}
		else
		{
			camera.posEye.x += sinf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
			camera.posEye.z += cosf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	if (Keyboard_IsPress(DIK_S))
	{
		if (Keyboard_IsPress(DIK_SPACE))
		{
			camera.posEye.y -= CAMERA_MOVE_SPEED;
		}
		else
		{
			camera.posEye.x -= sinf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
			camera.posEye.z -= cosf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
		}
	}
	// X軸移動（左右）
	if (Keyboard_IsPress(DIK_A))
	{
		camera.posEye.x -= cosf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
		camera.posEye.z += sinf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
	}
	if (Keyboard_IsPress(DIK_D))
	{
		camera.posEye.x += cosf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
		camera.posEye.z -= sinf(D3DXToRadian(camera.rotAt.y)) * CAMERA_MOVE_SPEED;
	}

	// Camera注視点の更新
	camera.posAt.y = camera.posEye.y + camera.lenEyeToAt * sinf(D3DXToRadian(camera.rotAt.x));
	camera.posAt.x = camera.posEye.x + camera.lenEyeToAt * sinf(D3DXToRadian(camera.rotAt.y)) * cosf(D3DXToRadian(camera.rotAt.x));
	camera.posAt.z = camera.posEye.z + camera.lenEyeToAt * cosf(D3DXToRadian(camera.rotAt.y)) * cosf(D3DXToRadian(camera.rotAt.x));
}

// Cameraの描画
void Camera_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ビュー行列の作成
	D3DXMatrixLookAtLH(&camera.mtxView,
		&camera.posEye, // カメラの位置
		&camera.posAt,  // カメラの注視点
		&camera.vecUp); // カメラの上方向

	// ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &camera.mtxView);

	// プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&camera.mtxProjection,
		VIEW_ANGLE,  // ビュー平面の視野角
		VIEW_ASPECT, // ビュー平面のアスペクト比
		VIEW_NEAR_Z, // ビュー平面のNearZ値
		VIEW_FAR_Z); // ビュー平面のFarZ値

	// プロジェクション行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera.mtxProjection);

	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", camera.posEye.x, camera.posEye.y, camera.posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", camera.posAt.x, camera.posAt.y, camera.posAt.z);
	DebugFont_Draw(2, 62, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", camera.rotAt.x, camera.rotAt.y, camera.rotAt.z);
	//DebugFont_Draw(640, 32, "CameraView: Xx: %.02lf Xy: %.02lf Xz: %.02lf", camera.mtxView._11, camera.mtxView._12, camera.mtxView._13);
	//DebugFont_Draw(640, 62, "CameraView: Yx: %.02lf Yy :%.02lf Yz: %.02lf", camera.mtxView._21, camera.mtxView._22, camera.mtxView._23);
	//DebugFont_Draw(640, 92, "CameraView: Zx: %.02lf Zy :%.02lf Zz: %.02lf", camera.mtxView._31, camera.mtxView._32, camera.mtxView._33);
	//DebugFont_Draw(640, 122, "CameraView: Tx: %.02lf Ty: %.02lf Tz: %.02lf", camera.mtxView._41, camera.mtxView._42, camera.mtxView._43);
}

Camera* Get_Camera()
{
	return &camera;
}

// Cameraの初期化（コンストラクタ）
Camera::Camera()
{
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	//posEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	direction = posAt - posEye; // Cameraの向きの計算
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	rotAt = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	lenEyeToAt = EYETOATLEN;
	bZoom_Back = false;
	bZoom_Ready = false;
	zoom_cnt = 0.0f;
}

// Cameraのリセット
void Camera::CameraReset()
{
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	//posEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	direction = posAt - posEye; // Cameraの向きの計算
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	rotAt = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
	lenEyeToAt = EYETOATLEN;
	bZoom_Back = false;
	bZoom_Ready = false;
	zoom_cnt = 0.0f;
}

// Cameraのズーム前進
void Camera::Camera_Zoom_Forward()
{
	if (zoom_cnt < ZOOM_MAX)
	{
		Reset_CameraEye();
		zoom_cnt += ZOOM_INCREASING;
		Set_CameraEye();
	}
}

// Cameraのズーム後退
void Camera::Camera_Zoom_Back()
{
	if (camera.bZoom_Back)
	{
		Reset_CameraEye();
		zoom_cnt -= ZOOM_INCREASING;
		Set_CameraEye();
		if (zoom_cnt <= 0.0f)
		{
			zoom_cnt = 0.0f;
			camera.bZoom_Back = false;
		}
	}
}

// CameraEyeのセット
void Camera::Set_CameraEye()
{
	posEye.y += sinf(D3DXToRadian(rotAt.x)) * zoom_cnt;
	posEye.x += sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x)) * zoom_cnt;
	posEye.z += cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x)) * zoom_cnt;
}

// CameraEyeのリセット
void Camera::Reset_CameraEye()
{
	posEye.y -= sinf(D3DXToRadian(rotAt.x)) * zoom_cnt;
	posEye.x -= sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x)) * zoom_cnt;
	posEye.z -= cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x)) * zoom_cnt;
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