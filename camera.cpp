#include "camera.h"
#include "input.h"
#include "tool_functions.h"

// メンバー関数の定義
// Cameraの描画
void Camera::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ビュー行列の作成
	D3DXMatrixLookAtLH(&mtxView,
		&posEye, // カメラの位置
		&posAt,  // カメラの注視点
		&vecUp); // カメラの上方向

	// ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		VIEW_ANGLE,  // ビュー平面の視野角
		VIEW_ASPECT, // ビュー平面のアスペクト比
		VIEW_NEAR_Z, // ビュー平面のNearZ値
		VIEW_FAR_Z); // ビュー平面のFarZ値

	// プロジェクション行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

// Cameraのズーム前進
void Camera::Zoom_Foward(float zi)
{
	posEye.z += zi;	
}

// Cameraのズーム後退
void Camera::Zoom_Backward(float zi)
{
	posEye.z -= zi;
}

// ファーストパーソンCamera
// Cameraの初期化
void CameraFP::Initialize(D3DXVECTOR3 peye, D3DXVECTOR3 pat)
{
	posEye = posEyeDef = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = posAtDef = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	direction = posAt - posEye; // Cameraの向きの計算
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotEyeDef = rotEye = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
	bSetValue = false;
}

// Cameraの終了処理
void CameraFP::Finalize()
{
	posEye = posEyeDef;
	posAt = posAtDef;
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotEye = rotEyeDef;
	bSetValue = false;
}

// Cameraの更新
void CameraFP::Update()
{
	// Cameraのリセット
	Camera_Reset();

	if (!bZoom)
	{
		// CameraEyeの回転（自転）
		CameraEye_Rot();
	}

	// CameraEyeの移動
	CameraEye_Move();

	// CameraAtの移動
	CameraAt_Move();
}

// Cameraのリセット
void CameraFP::Camera_Reset()
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		Finalize();
	}
}

// CameraEyeの移動
void CameraFP::CameraEye_Move()
{
	// Z軸移動（前後）
	// SPACEを押している場合、Y軸移動（上下）
	if (GetKeyboardPress(DIK_W))
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			posEye.y += CAMERA_MOVE_SPEED;
		}
		else
		{
			posEye.x += sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
			posEye.z += cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		}
	}

	if (GetKeyboardPress(DIK_S))
	{
		if (GetKeyboardPress(DIK_SPACE))
		{
			posEye.y -= CAMERA_MOVE_SPEED;
		}
		else
		{
			posEye.x -= sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
			posEye.z -= cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		}
	}
	// X軸移動（左右）
	if (GetKeyboardPress(DIK_A))
	{
		posEye.x -= cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		posEye.z += sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
	}
	if (GetKeyboardPress(DIK_D))
	{
		posEye.x += cosf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
		posEye.z -= sinf(D3DXToRadian(rotEye.y)) * CAMERA_MOVE_SPEED;
	}

	// ズーム（前後）
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
}

// CameraEyeの回転（自転）
void CameraFP::CameraEye_Rot()
{
	// マウス操作
	rotEye.x -= GetMouseY() * CAMERAEYEROT;
	if (rotEye.x > ROT_X_LIMIT)
	{
		rotEye.x = ROT_X_LIMIT;
	}
	if (rotEye.x < -ROT_X_LIMIT)
	{
		rotEye.x = -ROT_X_LIMIT;
	}
	rotEye.y += GetMouseX() * CAMERAEYEROT;

	// キーボード操作
	// X軸回転（上下回転）
	if (GetKeyboardPress(DIK_UP) && rotEye.x < ROT_X_LIMIT)
	{
		rotEye.x += CAMERAATROT;
	}
	if (GetKeyboardPress(DIK_DOWN) && rotEye.x > -ROT_X_LIMIT)
	{
		rotEye.x -= CAMERAATROT;
	}
	// Y軸回転（左右回転）
	if (GetKeyboardPress(DIK_LEFT))
	{
		rotEye.y -= CAMERAATROT;
		rotEye.y = Rotation_Correction(rotEye.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		rotEye.y += CAMERAATROT;
		rotEye.y = Rotation_Correction(rotEye.y);
	}
}

// CameraAtの移動
void CameraFP::CameraAt_Move()
{
	posAt.y = posEye.y + lenEyeToAt * sinf(D3DXToRadian(rotEye.x));
	posAt.x = posEye.x + lenEyeToAt * sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
	posAt.z = posEye.z + lenEyeToAt * cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
}

// Camera回転のリセット値のセット
void CameraFP::Set_Rot_Reset_Value()
{
	if (!bSetValue)
	{
		rot_reset_x = (rotEye.x - rotEyeDef.x) / ROT_RESET_FRAME;
		rot_reset_y = (rotEye.y - rotEyeDef.y) / ROT_RESET_FRAME;
		bSetValue = true;
	}
}

// Camera回転のリセット
void CameraFP::Rot_Reset()
{
	rotEye.x = Angle_To_Target(rotEye.x, rot_reset_x, rotEyeDef.x);
	rotEye.y = Angle_To_Target(rotEye.y, rot_reset_y, rotEyeDef.y);
}

// サードパーソンCamera
// Cameraの初期化
void CameraTP::Initialize(D3DXVECTOR3 p, D3DXVECTOR3 pc)
{
	posEye = posEyeDef = D3DXVECTOR3(p.x + pc.x, p.y + pc.y, p.z + pc.z);
	posAt = posAtDef = D3DXVECTOR3(p.x, p.y, p.z);
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	direction = posAt - posEye; // Cameraの向きの計算
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotAt = rotAtDef = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
}

// Cameraの終了処理
void CameraTP::Finalize()
{
	posEye = posEyeDef;
	posAt = posAtDef;
	lenEyeToAt = EYETOATLEN;
	bZoom = false;
	rotAt = rotAtDef;
}

// Cameraの更新
void CameraTP::Update(D3DXVECTOR3 p)
{
	// CameraAtの移動
	CameraAt_Move(p);

	// CameraAtの回転（自転）
	CameraAt_Rot();

	// CameraEyeの移動
	CameraEye_Move();
}

// CameraAtの移動
void CameraTP::CameraAt_Move(D3DXVECTOR3 p)
{
	posAt = p;
}

// CameraAtの回転（自転）
void CameraTP::CameraAt_Rot()
{
	// X軸回転（上下回転）
	if (GetKeyboardPress(DIK_UP) && rotAt.x > -ROT_X_LIMIT)
	{
		rotAt.x -= CAMERAATROT;
	}
	if (GetKeyboardPress(DIK_DOWN) && rotAt.x < ROT_X_LIMIT)
	{
		rotAt.x += CAMERAATROT;
	}
	// Y軸回転（左右回転）
	if (GetKeyboardPress(DIK_LEFT))
	{
		rotAt.y -= CAMERAATROT;
		rotAt.y = Rotation_Correction(rotAt.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		rotAt.y += CAMERAATROT;
		rotAt.y = Rotation_Correction(rotAt.y);
	}
}

// CameraEyeの移動
void CameraTP::CameraEye_Move()
{
	posEye.y = posAt.y - lenEyeToAt * sinf(D3DXToRadian(rotAt.x));
	posEye.x = posAt.x - lenEyeToAt * sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
	posEye.z = posAt.z - lenEyeToAt * cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
}