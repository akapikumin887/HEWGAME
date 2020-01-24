#include "aiming.h"
#include "debug_font.h"
#include "input.h"
#include "tool_functions.h"
#include "game.h"

double Aiming2D::timeCnt;

// Aiming2D
// Aiming2Dの初期化（コンストラクタ）
Aiming2D::Aiming2D()
{
	timer = NULL;
	num = new Number;
	alpha = new Alphabet;
}

// Aiming2Dの終了処理（デストラクタ）
Aiming2D::~Aiming2D()
{
	if (timer != NULL)
	{
		delete timer;
		timer = NULL;
	}
	delete num;
	delete alpha;
}

// Aiming2Dの初期化
void Aiming2D::Initialize()
{
	texture_index = TEXTURE_INDEX_AIMING;
	pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
	color = D3DCOLOR_RGBA(255, 255, 255, 255);
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(texture_index);
	th = Texture_GetHeight(texture_index);

	state = AIMING_STATE_FREE;

	// Numberの初期化
	num->Initialize(TEXTURE_INDEX_NUMBER, D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(SCREEN_WIDTH - (TIME_DIGIT_MAX - 1) * 32.0f, 64.0f), 0, 0, 10, 1, TIME_DIGIT_MAX / 2, TIME_DIGIT_MAX / 2);
	// Alphabetの初期化
	alpha->Initialize(TEXTURE_INDEX_ALPHABET, "TIME", D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(num->pos.x - num->digit_i * num->tw, 64.0f), 0, 0, 13, 2);
}

// Aiming2Dの終了処理
void Aiming2D::Finalize()
{
	if (timer != NULL)
	{
		delete timer;
		timer = NULL;
	}
}

// Aiming2Dの更新
void Aiming2D::Update()
{	
	switch (state)
	{
	case AIMING_STATE_FREE:
		Free();
		break;
	case AIMING_STATE_PREPARE:
		Prepare();
		break;
	case AIMING_STATE_ZOOM_FORWARD:
		Zoom_Forward(zoomStart);
		break;
	case AIMING_STATE_ZOOM_BACKWARD_PREPARE:
		Zoom_Backward(zoomStart);
		break;
	case AIMING_STATE_ZOOM_BACKWARD_SHOT:
		Zoom_Backward(zoomStart, ZOOM_FORWARD + ZOOM_CHECK, ZOOM_INCREASING * 2);
		break;
	case AIMING_STATE_CHECK_ARROW:
		Zoom_Forward(zoomStart, ZOOM_CHECK, ZOOM_INCREASING * 2);
		break;
	default:
		break;
	}
}

// Aiming2Dの描画
void Aiming2D::Draw()
{
	// 構え状態中
	if (state == AIMING_STATE_PREPARE || state == AIMING_STATE_ZOOM_FORWARD)
	{
		LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

		Sprite_SetColor_2D(color); // 色のセット

		Sprite_Draw_2D(texture_index, pos.x, pos.y, tx, ty, tw, th);
	}
	DebugFont_Draw(2, 92, "posHit  x: %.2lf  y: %.2lf  z: %.2lf", posHit.x, posHit.y, posHit.z);
	DebugFont_Draw(2, 152, "timeCnt  x: %.2lf", timeCnt);
	Draw_Timer();
}

// Timerの表示
void Aiming2D::Draw_Timer()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// タイマー時間表示
	// カウントダウンタイマーがある場合
	if (timer != NULL)
	{
		num->Draw(timeCnt, num->digit_i, num->digit_d);
		
	}
	// カウントダウンタイマーがない場合
	else
	{
		num->Draw(0, num->digit_i, num->digit_d);
	}
	alpha->Draw();
}

// フリー状態
void Aiming2D::Free()
{
	if (GetKeyboardTrigger(DIK_LSHIFT) && ArrowManager::cnt < ARROW_MAX)
	{
		ArrowManager::Add_Arrow();
		state = AIMING_STATE_ZOOM_FORWARD;
		zoomStart = Get_Game_CameraFP()->posEye.z;
	}
}

// 構え状態
void Aiming2D::Prepare()
{
	// Aiming2Dからの射線が当たった座標の取得
	// 必要の情報の取得
	CameraFP *cameraFP = Get_Game_CameraFP();
	Target *target = Get_Game_Target();

	// Cameraから出される射線が当たった座標の取得
	posHit = RayHit(
		D3DXVECTOR3(cameraFP->posEye.x, cameraFP->posEye.y, target->pos.z), // 水平状態の時、射線のが当たった座標
		D3DXVECTOR2(cameraFP->rotEye.x, cameraFP->rotEye.y), // 射線の回転
		target->pos.z - cameraFP->posEye.z); // 水平状態の時、CameraからTargetへの距離

	if (timer == NULL)
	{
		timer = new Timer; // カウントダウンタイマーインスタンスの生成
		timer->SystemTimer_Initialize();
		timer->SystemTimer_Start(); // タイムスタート
	}

	// カウントダウンを更新
	timeCnt = TIME_COUNT_MAX - timer->SystemTimer_GetTime();
	
	// 時間制限を超えた
	if (timeCnt < 0.0f)
	{
		Arrow *arrows = ArrowManager::arrows;
		for (int i = 0; i < ArrowManager::num; i++)
		{
			// 発射準備中
			if (arrows[i].state == ARROW_STATE_PREPARE)
			{
				// 非表示にする
				arrows[i].state = ARROW_STATE_NONE;
				arrows[i].Display = false;
				break;
			}
		}
		//aiming.charge_span = 0;
		state = AIMING_STATE_ZOOM_BACKWARD_PREPARE;
		zoomStart = cameraFP->posEye.z;
		delete timer; // カウントダウンタイマーインスタンスの削除
		timer = NULL;
		return;
	}
	// 時間制限内
	else
	{
		if (GetKeyboardPress(DIK_Z))
		{
			state = AIMING_STATE_NONE;

			Arrow *arrows = ArrowManager::arrows;
			for (int i = 0; i < ArrowManager::num; i++)
			{
				// 発射準備中
				if (arrows[i].state == ARROW_STATE_PREPARE)
				{
					// 非表示にする
					arrows[i].state = ARROW_STATE_FLYING;
					break;
				}
			}
			delete timer;
			timer = NULL;
		}
	}
}

// 前進Zoom待ち
void Aiming2D::Zoom_Forward(float start, float zm, float zi)
{
	CameraFP *cameraFP = Get_Game_CameraFP();
	
	cameraFP->bZoom = true;

	if (cameraFP->rotEye.x != cameraFP->rotEyeDef.x || cameraFP->rotEye.y != cameraFP->rotEyeDef.y)
	{
		cameraFP->Set_Rot_Reset_Value();
		cameraFP->Rot_Reset();
	}
	else
	{
		// Camera回転のリセットが終わったら、リセット値セットフラグをオフに
		cameraFP->bSetValue = false;

		if (cameraFP->posEye.z < start + zm)
		{
			cameraFP->Zoom_Foward(zi);
		}
		else
		{
			if (state == AIMING_STATE_ZOOM_FORWARD)
			{
				// ズームが終わったら、Zoomフラグをオフに
				cameraFP->bZoom = false;
				state = AIMING_STATE_PREPARE;

				Arrow *arrows = ArrowManager::arrows;
				for (int i = 0; i < ArrowManager::num; i++)
				{
					if (arrows[i].state == ARROW_STATE_WAIT_ZOOM)
					{
						arrows[i].state = ARROW_STATE_PREPARE;
						arrows[i].Display = true;
					}
				}
			}
			else if (state == AIMING_STATE_CHECK_ARROW && GetKeyboardTrigger(DIK_C))
			{
				state = AIMING_STATE_ZOOM_BACKWARD_SHOT;
				zoomStart = cameraFP->posEye.z;
			}
		}
	}
}

// 後退Zoom待ち
void Aiming2D::Zoom_Backward(float start, float zm, float zi)
{
	CameraFP *cameraFP = Get_Game_CameraFP();

	cameraFP->bZoom = true;

	if (cameraFP->rotEye.x != cameraFP->rotEyeDef.x || cameraFP->rotEye.y != cameraFP->rotEyeDef.y)
	{
		cameraFP->Set_Rot_Reset_Value();
		cameraFP->Rot_Reset();
	}
	else
	{
		// Camera回転のリセットが終わったら、リセット値セットフラグをオフに
		cameraFP->bSetValue = false;

		if (cameraFP->posEye.z > start - zm)
		{
			cameraFP->Zoom_Backward(zi);
		}
		else
		{
			// ズームが終わったら、Zoomフラグをオフに
			cameraFP->bZoom = false;
			state = AIMING_STATE_FREE;
		}
	}
}

// Aiming3D
// Aiming3Dの初期化（コンストラクタ）
Aiming3D::Aiming3D()
{
	face = new Face;
}

// Aiming3Dの終了処理（デストラクタ）
Aiming3D::~Aiming3D()
{
	delete face;
}

// Aiming3Dの初期化
void Aiming3D::Initialize()
{
	texture_index = TEXTURE_INDEX_AIMING;
	pos = Pos_With_Camera();
	rot = D3DXVECTOR3(-90.0f, 0.0f, 0.0f);
	face->CreateFace(D3DXVECTOR3(0.1f, 0.0f, 0.1f));
	prepare = false;
}

// Aiming3Dの終了処理
void Aiming3D::Finalize()
{

}

// Aiming3Dの更新
void Aiming3D::Update()
{
	// Aiming3Dからの射線が当たった座標の取得
	// 必要の情報の取得
	CameraFP *cameraFP = Get_Game_CameraFP();
	Target *target = Get_Game_Target();

	// CameraによるAimingの位置更新
	pos = Pos_With_Camera();

	// Cameraから出される射線が当たった座標の取得
	posHit = RayHit(
		D3DXVECTOR3(cameraFP->posEye.x, cameraFP->posEye.y, target->pos.z), // 水平状態の時、射線のが当たった座標
		D3DXVECTOR2(cameraFP->rotEye.x, cameraFP->rotEye.y), // 射線の回転
		target->pos.z - cameraFP->posEye.z); // 水平状態の時、CameraからTargetへの距離

	// 構え状態
	if (GetKeyboardTrigger(DIK_LSHIFT) && !prepare && ArrowManager::cnt < 5)
	{
		ArrowManager::Add_Arrow();
		prepare = true;
		
	}
}

// Aiming3Dの描画
void Aiming3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl; // スケーリング行列
	D3DXMATRIX mtxRot; // 回転行列
	D3DXMATRIX mtxTrs; // 平行移動行列

	D3DXMatrixIdentity(&mtxWorld); // ワールド行列を単位行列に初期化

	// スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, 1, 1, 1);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// 回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y + rot_Camera.y), D3DXToRadian(rot.x + rot_Camera.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// 平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	face->Draw(texture_index);

	DebugFont_Draw(2, 92, "posHit  x: %.2lf  y: %.2lf  z: %.2lf", posHit.x, posHit.y, posHit.z);
}

// Cameraによる位置
D3DXVECTOR3 Aiming3D::Pos_With_Camera()
{
	rot_Camera = Get_Game_CameraFP()->rotEye;
	rot_Camera.x *= -1;
	return Get_PointXYZ_OnLine(Get_Game_CameraFP()->posEye, rot_Camera, 2.0f);
}