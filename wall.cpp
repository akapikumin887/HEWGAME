#include "wall.h"
#include "aiming.h"

static Wall target[3];

// Wallの初期化
void Wall_Initialize()
{
	target[0].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(10.0f, 1.0f, 10.0f));
	target[1].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(-5.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 270.0f), D3DXVECTOR3(10.0f, 1.0f, 10.0f));
	target[2].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 1.0f, 5.0f));
}

// Wallの終了処理
void Wall_Finalize()
{

}

// Wallの更新
void Wall_Update()
{
	target[0].Rotation_Correction(target[0].rot.z += 1.0f);
}

// Wallの描画
void Wall_Draw()
{
	//wall[0].Draw_Wall();
	//wall[1].Draw_Wall();
	target[2].Draw_Wall();
}

// Wallの頂点情報取得（コンストラクタ）
Wall::Wall()
{
	wallv = new VERTEX_3D;
}

// Wallの頂点情報のリリース（デストラクタ）
Wall::~Wall()
{
	delete wallv;
}

// Wallの描画
void Wall::Draw_Wall()
{
	wallv->Sprite_Draw_Face(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
}

// Wallのセット
void Wall::Set_Wall(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Wallの回転の補正
float Wall::Rotation_Correction(float r)
{
	return wallv->Rotation_Correction(r);
}