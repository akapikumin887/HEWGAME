#include "camera.h"
#include "input.h"
#include "debug_font.h"
#include "cube.h"
#include "target.h"
#include "aiming.h"

static Camera camera;

// Cameraの初期化
void Camera_Initialize()
{
	camera.Initialize();
}

// Cameraの終了処理
void Camera_Finalize()
{
	camera.Finalize();
}

// Cameraの更新
void Camera_Update()
{
	camera.Update();
}

// Cameraの描画
void Camera_Draw()
{
	camera.Draw();

	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", camera.posEye.x, camera.posEye.y, camera.posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", camera.posAt.x, camera.posAt.y, camera.posAt.z);
	DebugFont_Draw(2, 62, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", camera.rotEye.x, camera.rotEye.y, camera.rotEye.z);
	//DebugFont_Draw(640, 32, "CameraView: Xx: %.02lf Xy: %.02lf Xz: %.02lf", camera.mtxView._11, camera.mtxView._12, camera.mtxView._13);
	//DebugFont_Draw(640, 62, "CameraView: Yx: %.02lf Yy :%.02lf Yz: %.02lf", camera.mtxView._21, camera.mtxView._22, camera.mtxView._23);
	//DebugFont_Draw(640, 92, "CameraView: Zx: %.02lf Zy :%.02lf Zz: %.02lf", camera.mtxView._31, camera.mtxView._32, camera.mtxView._33);
	//DebugFont_Draw(640, 122, "CameraView: Tx: %.02lf Ty: %.02lf Tz: %.02lf", camera.mtxView._41, camera.mtxView._42, camera.mtxView._43);
}

Camera* Get_Camera()
{
	return &camera;
}

// メンバー関数の定義

// Cameraの初期化（コンストラクタ）
Camera::Camera()
{
	
}

// Cameraの初期化
void Camera::Initialize()
{
	posEye = D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z);
	posAt = D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z);
	//posEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//posAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	direction = posAt - posEye; // Cameraの向きの計算
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	rotEye = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	lenEyeToAt = EYETOATLEN;
	bZoom_Forward = false;
	bZoom_Back = false;
	zoom_cnt = 0.0f;
}

// Cameraの終了処理
void Camera::Finalize()
{

}

// Cameraの更新
void Camera::Update()
{
	// ファーストパーソン
	// Cameraのリセット
	Camera_Reset();

	//// CameraEyeの回転（自転）
	CameraEye_Rot();

	// CameraEyeの移動
	CameraEye_Move();

	// CameraAtの移動
	CameraAt_Move();

	// サードパーソン

}

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

	DebugFont_Draw(2, 2, "CameraRot:  x: %.2lf  y: %.2lf  z: %.2lf", rotAt.x, rotAt.y, rotAt.z);
}

// Cameraのリセット
void Camera::Camera_Reset()
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		Initialize();
	}
}

// CameraEyeの移動
void Camera::CameraEye_Move()
{
	// Y軸移動（上下）
	if (GetKeyboardPress(DIK_W))
	{
		posEye.y += CAMERA_MOVE_SPEED;
	}

	if (GetKeyboardPress(DIK_S))
	{
		posEye.y -= CAMERA_MOVE_SPEED;
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

	// Cameraズーム（前後）
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * GetMouseZ() * CAMERA_ZOOM_SPEED;
}

// CameraEyeの回転（自転）
void Camera::CameraEye_Rot()
{
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
void Camera::CameraAt_Move()
{
	posAt.y = posEye.y + lenEyeToAt * sinf(D3DXToRadian(rotEye.x));
	posAt.x = posEye.x + lenEyeToAt * sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
	posAt.z = posEye.z + lenEyeToAt * cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x));
}

// Cameraのズーム前進
void Camera::Camera_Zoom_Forward(float zm, float zi)
{
	Aiming *aiming;
	aiming = Get_Aiming();

	// 構え時のズーム
	if (aiming->prepare && zoom_cnt < zm)
	{
		Reset_Zoom();
		zoom_cnt += zi;
		Set_Zoom();
	}
	// 矢が的にあったときのズーム
	else if (bZoom_Forward)
	{
		Reset_Zoom();
		zoom_cnt += zi;
		if (zoom_cnt > zm)
		{
			zoom_cnt = zm;
		}
		Set_Zoom();
		if (GetKeyboardTrigger(DIK_SPACE))
		{
			bZoom_Forward = false;
			bZoom_Back = true;
			Cube::bFlying = false;
		}
	}
}

// Cameraのズーム後退
void Camera::Camera_Zoom_Back(float zm, float zi)
{
	if (bZoom_Back)
	{
		Reset_Zoom();
		zoom_cnt -= zi;
		if (zoom_cnt <= zm)
		{
			zoom_cnt = zm;
			bZoom_Back = false;
		}
		Set_Zoom();
	}
}

// CameraEyeのセット
void Camera::Set_Zoom()
{
	posEye.y += sinf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.x += sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.z += cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
}

// CameraEyeのリセット
void Camera::Reset_Zoom()
{
	posEye.y -= sinf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.x -= sinf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
	posEye.z -= cosf(D3DXToRadian(rotEye.y)) * cosf(D3DXToRadian(rotEye.x)) * zoom_cnt;
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

void Camera::Initialize(D3DXVECTOR3 p)
{
	posEye = D3DXVECTOR3(p.x, p.y + 5.0f, p.z - 50.0f);
	posAt = D3DXVECTOR3(p.x, p.y, p.z);
	direction = posAt - posEye; // Cameraの向きの計算
	lenEyeToAt = sqrtf(powf(direction.x, 2.0f) + powf(direction.y, 2.0f) + powf(direction.z, 2.0f));
	D3DXVec3Normalize(&direction, &direction); // Cameraの向きの正規化
	rotAt = D3DXVECTOR3(D3DXToDegree(atan2f(direction.y, direction.z)), D3DXToDegree(atan2f(direction.x, direction.z)), 0.0f); // 向きによるCameraの回転
	vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	bZoom_Forward = false;
	bZoom_Back = false;
	zoom_cnt = 0.0f;
}

void Camera::Finalize(D3DXVECTOR3 p)
{
	Initialize(p);
}

void Camera::Update(D3DXVECTOR3 p)
{

}

void Camera::CameraAt_Rot()
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

	posEye.y = posAt.y - lenEyeToAt * sinf(D3DXToRadian(rotAt.x));
	posEye.x = posAt.x - lenEyeToAt * sinf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
	posEye.z = posAt.z - lenEyeToAt * cosf(D3DXToRadian(rotAt.y)) * cosf(D3DXToRadian(rotAt.x));
}