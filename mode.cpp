#include "mode.h"
#include "input.h"
#include "tool_functions.h"
#include "camera.h"
#include "fade.h"
#include "ui.h"

static CameraTP camera;
static Mode mode[MODE_OBJECT_MAX];
static bool right;
static bool left;
static MODE g_mode;

void Mode_Initialize()
{
	camera.Initialize(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -10.0f));
	mode[0].Initialize(TEXTURE_INDEX_BG, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	mode[1].Initialize(TEXTURE_INDEX_MODE_EASY, D3DXVECTOR3(-3.0f, 0.0f, 0.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 3.0f));
	mode[2].Initialize(TEXTURE_INDEX_MODE_NORMAL, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 3.0f));
	mode[3].Initialize(TEXTURE_INDEX_MODE_HARD, D3DXVECTOR3(3.0f, 0.0f, 0.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 3.0f));
	mode[4].Initialize(TEXTURE_INDEX_MODE_SELECT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 3.0f));
	
	right = left = false;
	g_mode = MODE_NONE;
}

void Mode_Finalize()
{
	camera.Finalize();
	for (int i = 0; i < MODE_OBJECT_MAX; i++)
	{
		mode[i].Finalize();
	}
	right = left = false;
}

void Mode_Update()
{
	camera.Update(D3DXVECTOR3(camera.posAtDef));
	for (int i = 1; i < MODE_OBJECT_MAX - 1; i++)
	{
		mode[i].Update();
	}

	// 右
	if (right && !left)
	{
		mode[MODE_OBJECT_MAX - 1].pos = mode[1].pos;

		if (GetKeyboardTrigger(DIK_A))
		{
			right = false;
			left = true;
		}
		if (GetKeyboardTrigger(DIK_D))
		{
			right = false;
		}
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_mode = MODE_EASY;
		}
	}
	// 真ん中
	else if (!right && !left)
	{
		mode[MODE_OBJECT_MAX - 1].pos = mode[2].pos;

		if (GetKeyboardTrigger(DIK_A))
		{
			right = true;
		}
		if (GetKeyboardTrigger(DIK_D))
		{
			left = true;
		}
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_mode = MODE_NORMAL;
		}
	}
	// 左
	else if (!right && left)
	{
		mode[MODE_OBJECT_MAX - 1].pos = mode[3].pos;

		if (GetKeyboardTrigger(DIK_A))
		{
			left = false;
		}
		if (GetKeyboardTrigger(DIK_D))
		{
			right = true;
			left = false;
		}
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			g_mode = MODE_HARD;
		}
	}

	if (g_mode != MODE_NONE)
	{
		Fade(SCENE_GAME);
		return;
	}
}

void Mode_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	camera.Draw();
	
	for (int i = 0; i < MODE_OBJECT_MAX; i++)
	{
		mode[i].Draw();
	}
}

MODE Get_Mode()
{
	return g_mode;
}

void Set_Mode(MODE m)
{
	g_mode = m;
}

// Modeの初期化（コンストラクタ）
Mode::Mode()
{
	face = new Face;
}

// Modeの終了処理（デストラクタ）
Mode::~Mode()
{
	delete face;
}

void Mode::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz)
{
	texture_index = tex_idx;
	pos = p;
	rot = r;

	face->CreateFace(sz);
}

void Mode::Finalize()
{
	//Texture_Release();
}

void Mode::Update()
{
	static float d = 0.0f;

	pos.y += 0.05f * sinf(D3DXToRadian(d));

	d += 3.0f;
	d = Rotation_Correction(d);
}

void Mode::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ワールド行列を単位行列に初期化

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ワールドマトリックスを設定

	face->Draw(texture_index);
}