#include "plane.h"

// Planeの初期化（コンストラクタ）
Plane::Plane()
{
	faceEX = new FaceEX;
}

// Planeの終了処理（デストラクタ）
Plane::~Plane()
{
	delete faceEX;
}

// Planeの初期化
void Plane::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz, D3DXVECTOR3 szn)
{
	texture_index = tex_idx;
	pos = D3DXVECTOR3(p.x, p.y, p.z);
	rot = D3DXVECTOR3(r.x, r.y, r.z);
	faceEX->CreateFaceEX(sz, szn);
}

// Planeの終了処理
void Plane::Finalize()
{

}

// Planeの更新
void Plane::Update()
{

}

// Planeの描画
void Plane::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ワールド行列を単位行列に初期化

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ワールドマトリックスを設定

	faceEX->Draw(texture_index);
}