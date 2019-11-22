#include "aiming.h"
#include "input.h"
#include "debug_font.h"
#include "cube.h"

static Aiming aiming;
static double timeCnt;

// Aimingの初期化
void Aiming_Initialize()
{
	aiming.Set_Aiming(TEXTURE_INDEX_AIMING, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

// Aimingの終了処理
void Aiming_Finalize()
{
	aiming.prepare = false;
}

// Aimingの更新
void Aiming_Update()
{
	// マウス情報の取得
	DIMOUSESTATE *MouseState = GetMouseState();

	// Aimingの操作
	aiming.pos.x += MouseState->lX * 0.05f;
	aiming.pos.y -= MouseState->lY * 0.05f;

	// 構え（矢の生成）
	if (Keyboard_IsTrigger(DIK_LSHIFT) && !aiming.prepare)
	{
		Add_Cube();
		aiming.prepare = true;
		aiming.timec = new Time;
		aiming.timec->SystemTimer_Start();
	}

	// 構え状態中
	if (aiming.prepare)
	{
		// カウントダウンを更新
		timeCnt = TIME_COUNT_MAX - aiming.timec->SystemTimer_GetTime();

		// 時間制限を超えた
		if (timeCnt < 0.0f)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					cube[i].nShotted = true;
					break;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false;
			delete aiming.timec;
			return;
		}

		// 弓を引く（チャージ）
		if (Keyboard_IsPress(DIK_Z))
		{	// SPACEを押している間チャージする
			if (aiming.charge_span < CHARGE_SPAN)
			{
				aiming.charge_span++;
			}
		}
		else
		{
			if (aiming.charge_span > 0)
			{
				aiming.charge_span--;
			}
		}

		// 発射
		if (Keyboard_IsTrigger(DIK_X) && aiming.charge_span > 0)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				if (cube[i].bUse && !cube[i].bShotted)
				{
					cube[i].bShotted = true;
					//cube[i].charge = aiming.charge_span;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false;
			delete aiming.timec;
		}
	}
}

// Aimingの描画
void Aiming_Draw()
{
	aiming.Draw_Aiming();
	DebugFont_Draw(2, 92, "Aiming  x: %.2lf  y: %.2lf  z: %.2lf", aiming.pos.x, aiming.pos.y, aiming.pos.z);
	DebugFont_Draw(640, 32, "AimingTime: %.02lf", timeCnt);
}

// Aiming情報の取得
Aiming* Get_Aiming()
{
	return &aiming;
}

// Aimingの頂点情報取得（コンストラクタ）
Aiming::Aiming()
{
	aimingv = new VERTEX_3D;
	isAiming = false;
	prepare = false;
}

// Aimingの頂点情報のリリース（デストラクタ）
Aiming::~Aiming()
{
	delete aimingv;
}

// Aimingの描画
void Aiming::Draw_Aiming()
{
	aimingv->Sprite_Draw_Face(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
}

// Aimingのセット
void Aiming::Set_Aiming(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Aimingの回転の補正
float Aiming::Rotation_Correction(float r)
{
	return aimingv->Rotation_Correction(r);
}