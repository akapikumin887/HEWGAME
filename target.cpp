#include "target.h"
#include "tool_functions.h"

// Targetの初期化（コンストラクタ）
Target::Target()
{
	face = new Face;
}

// Targetの終了処理（デストラクタ）
Target::~Target()
{
	delete face;
}

void Target::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz)
{
	texture_index = tex_idx;
	pos = D3DXVECTOR3(p.x, p.y, p.z);
	rot = D3DXVECTOR3(r.x, r.y, r.z);
	
	face->CreateFace(sz);

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

void Target::Finalize()
{
	
}

void Target::Update()
{

}

void Target::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ワールド行列を単位行列に初期化

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ワールドマトリックスを設定

	face->Draw(texture_index);
}