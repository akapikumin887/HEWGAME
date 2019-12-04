#include "aiming.h"
#include "input.h"
#include "debug_font.h"
#include "camera.h"
#include "cube.h"

static Aiming aiming;
static Number time;
static AimingTime alphabet;
static Camera *camera;
static double timeCnt;

// Aimingの初期化
void Aiming_Initialize()
{
	aiming.Set_Aiming(TEXTURE_INDEX_AIMING, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// Alphabetの初期化
	strncpy(alphabet.ta, "TIME", 8);
	alphabet.len = strlen(alphabet.ta);
	camera = Get_Camera();
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
	if (Keyboard_IsTrigger(DIK_LSHIFT) && !aiming.prepare && !camera->bZoom_Ready && !camera->bZoom_Back)
	{
		Add_Cube(); // 矢の生成
		aiming.prepare = true;
		aiming.time_cnt = new Timer; // カウントダウンタイマーインスタンスの生成
		aiming.time_cnt->SystemTimer_Start(); // タイムスタート
	}

	// 構え状態中
	if (aiming.prepare)
	{
		// Cameraズーム前進
		camera->Camera_Zoom_Forward();

		// カウントダウンを更新
		timeCnt = TIME_COUNT_MAX - aiming.time_cnt->SystemTimer_GetTime();

		// 時間制限を超えた
		if (timeCnt < 0.0f)
		{
			Cube *cube = Get_Cube(); // Cube情報の取得
			for (int i = 0; i < CUBE_MAX; i++)
			{
				// 使用中で、発射されていない、時間制限で発射されていないではない矢
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					// 時間制限で未発射だから、フラグをtrueに
					cube[i].nShotted = true;
					break;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false; // 構え解除
			camera->bZoom_Back = true; // Cameraズーム後退フラグ
			delete aiming.time_cnt; // カウントダウンタイマーインスタンスの削除
			aiming.time_cnt = NULL;
			return;
		}

		// ズームが終わるまでチャージはできない（チャージはできないから、発射もできない）
		if (camera->zoom_cnt >= ZOOM_MAX)
		{
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
		}

		// 発射
		if (Keyboard_IsTrigger(DIK_X) && aiming.charge_span > 0)
		{
			Cube *cube = Get_Cube();
			for (int i = 0; i < CUBE_MAX; i++)
			{
				// 使用中で、発射されていない、時間制限で発射されていないではない矢を発射
				if (cube[i].bUse && !cube[i].bShotted && !cube[i].nShotted)
				{
					cube[i].bShotted = true; // 発射されたことにする
					//cube[i].charge = aiming.charge_span;
				}
			}
			aiming.charge_span = 0;
			aiming.prepare = false; // 構え解除
			delete aiming.time_cnt; // カウントダウンタイマーインスタンスの削除
			aiming.time_cnt = NULL;
		}
	}

	// Cameraズーム後退
	camera->Camera_Zoom_Back();
}

// Aimingの描画
void Aiming_Draw()
{
	aiming.Draw_Aiming();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	Sprite_SetColor_2D(time.color); // 色のセット

	// タイマー時間表示
	// カウントダウンタイマーがある場合
	if (aiming.time_cnt != NULL)
	{
		for (int i = 0; i < TIME_DIGIT_MAX - 1; i++)
		{
			// 小数点以降2桁から始まる（iが0の時、カウントダウンを10の-2乗で割ったら、100倍になる）
			time.n_tmp = (int)(timeCnt / pow((double)10, (double)i - 2));
			time.n_tmp = time.n_tmp % 10; // 1の位の値を取り出す

			// 取り出した値を表示
			// 小数点以降2桁を表示出来たら、1マスあけて、小数点以前の2桁を表示する
			Sprite_Draw_2D(time.TextureIndex,
				SCREEN_WIDTH - time.pos.x * (i < TIME_DIGIT_MAX / 2 ? i : i + 1) * 2 - time.pos.x, time.pos.y * 4,
				time.tx + time.n_tmp * time.tw, time.ty,
				time.tw, time.th);
		}
	}
	// カウントダウンタイマーがない場合
	else
	{
		for (int i = 0; i < TIME_DIGIT_MAX - 1; i++)
		{
			// 全部0で表示
			Sprite_Draw_2D(time.TextureIndex,
				SCREEN_WIDTH - time.pos.x * (i < TIME_DIGIT_MAX / 2 ? i : i + 1) * 2 - time.pos.x, time.pos.y * 4,
				time.tx, time.ty,
				time.tw, time.th);
		}
	}

	Sprite_SetColor_2D(alphabet.color); // 色のセット

	// アルファベットの表示
	for (int i = 0; i < alphabet.len; i++)
	{
		Sprite_Draw_2D(alphabet.TextureIndex,
			SCREEN_WIDTH - time.pos.x * DIGIT_MAX * 2 - alphabet.pos.x * (alphabet.len - i) * 2, alphabet.pos.y * 4,
			alphabet.tx + ((alphabet.ta[i] - 65) % 13) * alphabet.tw, alphabet.ty + (alphabet.ta[i] - 65) / 13 * alphabet.th,
			alphabet.tw, alphabet.th);
	}
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
	time_cnt = NULL;
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