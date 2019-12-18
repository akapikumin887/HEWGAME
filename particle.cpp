/*

	※ノーサポートであれば勝手に参考＆流用してもＯＫ

	ただしクラスになっているので、C言語風にするなり
	そのまま活用するなりは自由

*/

#include "particle.h"
#include "camera.h"

///////////////////////////////////////////////////////////
//
//PARTICLE　
//   パーティクルを管理するクラス
//
///////////////////////////////////////////////////////////

//=============================================
//スタティックメンバー
//=============================================
int				PARTICLE::numparticle;	//パーティクル数
particle_base*  PARTICLE::obj_pool;		//パーティクル配列
VERTEX_3D		PARTICLE::vertex[4];	//パーティクル頂点データ

//=============================================
//初期化
//
//int num_particle パーティクル最大数
//=============================================
void PARTICLE::Init(int num_particle)
{
	//パーティクル構造体の配列を作成
	obj_pool = new particle_base[ num_particle ];

	//全パーティクル初期化
	for(int i=0;i<num_particle;i++)
	{	//パーティクルに初期化を指示する
		obj_pool[i].InitParticle();
	}

	//パーティクル数保存	
	numparticle = num_particle;

	//頂点作成
	vertex[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertex[1].vtx = D3DXVECTOR3( 0.5f, 0.5f, 0.0f);
	vertex[2].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertex[3].vtx = D3DXVECTOR3( 0.5f, -0.5f, 0.0f);

	vertex[0].nor = D3DXVECTOR3( 0,0,-1);
	vertex[1].nor = D3DXVECTOR3( 0,0,-1);
	vertex[2].nor = D3DXVECTOR3( 0,0,-1);
	vertex[3].nor = D3DXVECTOR3( 0,0,-1);

	vertex[0].tex = D3DXVECTOR2( 0.0f, 0.0f);
	vertex[1].tex = D3DXVECTOR2( 1.0f, 0.0f);
	vertex[2].tex = D3DXVECTOR2( 0.0f, 1.0f);
	vertex[3].tex = D3DXVECTOR2( 1.0f, 1.0f);

	vertex[0].diffuse = D3DXCOLOR(1.0f, 1.0, 1.0, 1.0);
	vertex[1].diffuse = D3DXCOLOR(1.0f, 1.0, 1.0, 1.0);
	vertex[2].diffuse = D3DXCOLOR(1.0f, 1.0, 1.0, 1.0);
	vertex[3].diffuse = D3DXCOLOR(1.0f, 1.0, 1.0, 1.0);
}

//=============================================
//後始末
//=============================================
void PARTICLE::Uninit()
{

	//パーティクル構造体配列の解放
	if(obj_pool != NULL)
	{
		delete[] obj_pool;
		obj_pool = NULL;
	}
}

//=============================================
//パーティクルの更新
//=============================================
void PARTICLE::Update()
{

	//生きている全てのパーティクルを処理
	for(int i=0;i<numparticle;i++)
	{
		if(obj_pool[i].GetLife() > -1)
		{	//パーティクルに更新を指示する
			obj_pool[i].UpdateParticle();
		}
	}

	//パーティクル発生監視処理
	PARTICLE::ParticleOccer();
}

//=============================================
//パーティクル表示
//=============================================
void PARTICLE::Draw()
{

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//αブレンドON
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	//生きている全てのパーティクルを処理
	for(int i=0;i<numparticle;i++)
	{
		if(obj_pool[i].GetLife() > -1)
		{	//パーティクルに表示を指示する
			obj_pool[i].DrawParticle();
		}
	}
	
	//αブレンドOFF
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//=============================================
//パーティクル発生監視
//
// Lifeが0未満なら初期化
//=============================================
void PARTICLE::ParticleOccer()
{

	//寿命の来ているパーティクルを探して復活させる
	for(int i=0; i<numparticle; i++)
	{
		if(obj_pool[i].GetLife() < 0 )
		{	//パーティクルに再初期化を指示する
			obj_pool[i].InitParticle();
			//break;//1フレームに1個ずつ復活
		}
	}
}

//=============================================
//パーティクルの頂点色セット
//
//D3DXVECTOR4 col 頂点カラー　xyzw = RGBA
//=============================================
void PARTICLE::SetColor(D3DXVECTOR4 col)
{
	vertex[0].diffuse = D3DXCOLOR( col.x, col.y, col.z, col.w);
	vertex[1].diffuse = D3DXCOLOR( col.x, col.y, col.z, col.w);
	vertex[2].diffuse = D3DXCOLOR( col.x, col.y, col.z, col.w);
	vertex[3].diffuse = D3DXCOLOR( col.x, col.y, col.z, col.w);
}

///////////////////////////////////////////////////////////
//
//particle_base
//	パーティクルそのもののクラス
//
///////////////////////////////////////////////////////////

//=============================================
//パーティクルの初期化
//=============================================
void	particle_base::InitParticle()
{
	//カラーセット
	color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	//スケールセット
	scl = D3DXVECTOR3(1,1,1);

	//寿命セット
	int life = rand() % 130 + 50; //乱数＋基本値
	Life = life;

	//座標セット (float)rand() / (float)RAND_MAXで0.0～1.0が作れる
	//Position.x = ((float)rand() / (float)RAND_MAX) * 20.0f - 10.0f;
	//Position.y = ((float)rand() / (float)RAND_MAX) * 20.0f - 10.0f;
	//Position.z = ((float)rand() / (float)RAND_MAX) * 10.0f - 5.0f;

	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	direction.x = direction.y = direction.z = 1.0f;
	direction.x = (rand() % 2 ? direction.x : -direction.x);
	direction.z = (rand() % 2 ? direction.z : -direction.z);

	move = D3DXVECTOR3(direction.x*((float)rand() / (float)RAND_MAX + 0.1f), (float)rand() / (float)RAND_MAX + 0.5f, direction.z * ((float)rand() / (float)RAND_MAX + 0.1f));
}

//=============================================
//パーティクルの更新
//=============================================
void	particle_base::UpdateParticle()
{

	//なにもしない、ただ寿命が減るだけ
	Life--;
	if(Life < 0)
	{	//0未満になると初期化してくれる
		Life = -1;//念のため
	}

	pos += move;
	if (pos.y < 0.0f)
	{
		pos.y = 0.0f;
		move.y *= -1;
	}
	move.y -= 0.1f;
}

//=============================================
//パーティクルの表示
//=============================================
void	particle_base::DrawParticle()
{

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//ビュー行列取得&ビルボード行列作成
	D3DXMATRIX	BillMatrix = Get_Camera()->mtxView;//<<カメラ取得はreturn g_mtxView;をmainに作る
	BillMatrix._41 = 
	BillMatrix._42 = 
	BillMatrix._43 = 0.0f;
	D3DXMatrixTranspose(&BillMatrix, &BillMatrix);

	//スケーリング行列
	D3DXMATRIX	mtxScl;
	D3DXMatrixIdentity(&mtxScl);
	mtxScl._11 = scl.x;
	mtxScl._22 = scl.y;
	mtxScl._33 = scl.z;

	//平行移動行列
	D3DXMATRIX	mtxTrs;
	D3DXMatrixIdentity(&mtxTrs);
	mtxTrs._41 = pos.x;
	mtxTrs._42 = pos.y;
	mtxTrs._43 = pos.z;

	//ワールド行列作成&セット
	D3DXMatrixMultiply(&BillMatrix, &mtxScl, &BillMatrix);
	D3DXMatrixMultiply(&BillMatrix, &BillMatrix, &mtxTrs);
	pDevice->SetTransform(D3DTS_WORLD, &BillMatrix);

	//頂点フォーマットのセット
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_KIZUNA));

	//ライティングOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//パーティクルを描画する
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, PARTICLE::GetVertex(), sizeof(VERTEX_3D));
}