#include "plane.h"

static Plane plane;

// Planeの初期化
void Plane_Initialize()
{
	plane.Set_Plane(TEXTURE_INDEX_MAX, D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(100.0f, 0.0f, 200.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
}

// Planeの終了処理
void Plane_Finalize()
{

}

// Planeの更新
void Plane_Update()
{
	
}

// Planeの描画
void Plane_Draw()
{
	plane.Draw_Plane();
}

// Planeの頂点情報取得（コンストラクタ）
Plane::Plane()
{
	planev = new VERTEX_3D;
}

// Planeの頂点情報のリリース（デストラクタ）
Plane::~Plane()
{
	delete planev;
}

// Planeの描画
void Plane::Draw_Plane()
{
	planev->Sprite_Draw_FaceEX(texture_index, pos, rot, scl, size, size_nor,revolution, revRadius, revSpd);
}

// Planeのセット
void Plane::Set_Plane(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, D3DXVECTOR3 sz, D3DXVECTOR3 szn, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	planev->CreateFaceEX(sz, szn);
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	size = sz;
	size_nor = szn;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Planeの回転の補正
float Plane::Rotation_Correction(float r)
{
	return planev->Rotation_Correction(r);
}