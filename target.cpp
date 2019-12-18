#include "target.h"
#include "camera.h"

static Target target;

// Targetの初期化
void Target_Initialize()
{
	target.Set_Target(TEXTURE_INDEX_TARGET, D3DXVECTOR3(TARGET_X, TARGET_Y, TARGET_Z * TARGET_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(TARGET_SIZE, 1.0f, TARGET_SIZE));
}

// Targetの終了処理
void Target_Finalize()
{

}

// Targetの更新
void Target_Update()
{
	
}

// Targetの描画
void Target_Draw()
{
	target.Draw_Target();
}

// Target情報の取得
Target* Get_Target()
{
	return &target;
}

// Targetの頂点情報取得（コンストラクタ）
Target::Target()
{
	targetv = new VERTEX_3D;
}

// Targetの頂点情報のリリース（デストラクタ）
Target::~Target()
{
	delete targetv;
}

// Targetの描画
void Target::Draw_Target()
{
	targetv->Sprite_Draw_Face(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
	targetv->Sprite_Draw_Face(texture_index, D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y + 120.0f, CAMERAEYE_Z), D3DXVECTOR3(180, 0, 0), scl, revolution, revRadius, revSpd);
}

// Targetのセット
void Target::Set_Target(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;

	// 各環の半径を初期化
	float c = TARGET_SIZE / 20;
	circle[0] = c / 2;
	for (int i = 1; i < 11; i++)
	{
		circle[i] = i * c;
		if (i == 8)
		{
			circle[i] -= c / 5;
		}
	}
}

// Targetの回転の補正
float Target::Rotation_Correction(float r)
{
	return targetv->Rotation_Correction(r);
}