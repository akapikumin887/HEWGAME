#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <Windows.h>
#include<math.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"

#include <d3dx9.h>
#include "mode.h"
#include "input.h"
#include "debug_font.h"
#include "camera.h"
#include "fade.h"


//サンプルポリゴンの表示自体に関するパラメータ


static float g_posx = -5, g_posy =4, g_posz = 0;
static float g_posx2 = 0, g_posy2 =4, g_posz2 = 0;
static float g_posx3 = 5, g_posy3 =4, g_posz3 = 0;
//枠線
static float g_posx4 = -5, g_posy4 = 4, g_posz4 = 0;
static float g_Rotation = 0.0f, g_Rotation2 = 3.0f, g_Rotation3 = 0.01f; // スプライト回転角度 
static float g_velx = 0, g_vely = 0, g_velz = 0;

static Mode mode;
static CameraTP cameraTP;


void Mode::Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 12,  //頂点データ用に確保するバッファサイズ／(バイト単位)

		D3DUSAGE_WRITEONLY, //頂点バッファの使用法
		FVF_VERTEX_3D,      //使用する頂点フォーマット
		D3DPOOL_MANAGED,    //リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuff,     //頂点バッファインターフェースへのポインタ
		NULL);              //NULLに設定

	//頂点バッファの中身を埋める
	VERTEX_3D  *pVtx;

	//初期化でインターフェースを作成(一回やる)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pD3DIdxBuff,
		NULL);

	WORD* pIdx = NULL; //配列の先頭ポインタの入れ物


	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	//頂点座標の設定
	//一面
	pVtx[0].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, -5.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-2.0f, -5.0f, 0.0f);

	pVtx[3].vtx = D3DXVECTOR3(-2.0f, -5.0f, 0.0f);
	pVtx[4].vtx = D3DXVECTOR3(-2.0f, 0.5f, 0.0f);
	pVtx[5].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	
	//法線ベクトルの設定(現時点ではいじらない)
	//一面
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	

	//反射光の設定(現時点ではいじらない)
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);





	
	//光の初期化
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));
	g_light.Type = D3DLIGHT_DIRECTIONAL; //ライト種類
	//光色
	g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//環境光

	D3DXVECTOR3 vecDin =
		D3DXVECTOR3(0.0f, -0.6f, 1.0f);
	//光の指す方向ベクトル
	D3DXVec3Normalize(&vecDin, &vecDin);
	g_light.Direction = vecDin; //正規化してセット

	pDevice->SetLight(0, &g_light); //0番らいとへデータをセット

	pDevice->LightEnable(0, TRUE); //０番ライトを有効化


	g_pD3DIdxBuff->Unlock(); //必ずアンロックする


	//頂点データをアンロックする
	g_pD3DVtxBuff->Unlock();

	//ここまで　3Dポリゴン用頂点の準備
	//================================================================

}

void Mode::Finalize()
{
	//Texture_Release();

	
}

void Mode::Update()
{
	g_posy  += g_vely;
	g_posy2 += g_vely;
	g_posy3 += g_vely;
	g_posy4 += g_vely;
	
	//g_posx4 += g_velx;

	//往復
	if (g_posy <= 3.5f )
	{
		
		g_vely += 0.001f;
		
	}
	if (g_posy >= 3.5f )
	{
		
		g_vely += -0.001f;
		
	}
	
	if (g_posx4 <= -5.0)
	{
		g_posx4 = -5.0;
	}
	if (g_posx4 >= 5.0)
	{
		g_posx4 = 5.0;
	}
	//決定
	//右移動最大座標5,左最大座標-5
	
	if (GetKeyboardTrigger(DIK_D))
	{
		g_posx4 += 5.0f;
		
	}
	if (GetKeyboardTrigger(DIK_A))
	{
		g_posx4 -= 5.0f;
	}

	//EASYselect
	if (g_posx4 == -5.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_easy
		Fade(SCENE_GAME);
	}
	//NORMALselect
	if (g_posx4 == 0.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_normal
		Fade(SCENE_GAME);
	}
	//HARDselect
	if (g_posx4 == 5.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_hard
		Fade(SCENE_GAME);
	}


	
}

void Mode::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//ここを追加　テスト用カメラ情報を準備
	
	//=============================================================
	//ここを追加　ポリゴンのワールド行列の作成

	//EASY

	D3DXMATRIX mtxScl; //スケーリング行列
	D3DXMATRIX mtxRot; //回転行列
	D3DXMATRIX mtxTrs; //平行移動行列

	g_pos = D3DXVECTOR3(g_posx, g_posy, g_posz); //位置
	g_rot = D3DXVECTOR3(0, g_Rotation,0); //向き(回転)
	g_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ(スケール)

	D3DXMatrixIdentity(&g_mtxworld);     //ワールド行列を単位行列に初期化

	//スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl, g_scl.x, g_scl.y, g_scl.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxScl); //World * Scaling

	//平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs, g_pos.x, g_pos.y, g_pos.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxTrs); //World * Translation

	//回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot.y, g_rot.x, g_rot.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxRot); //World * Rotation


	//ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld);

	//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画

	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); //光源処理をON
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_EASY));
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


	//板(NORMAL)
	//往復
	D3DXMATRIX mtxScl2; //スケーリング行列
	D3DXMATRIX mtxRot2; //回転行列
	D3DXMATRIX mtxTrs2; //平行移動行列

	g_pos2 = D3DXVECTOR3(g_posx2, g_posy2, g_posz2); //位置
	g_rot2 = D3DXVECTOR3(0, g_Rotation, 0); //向き(回転
	g_scl2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ(スケール)

	D3DXMatrixIdentity(&g_mtxworld2);     //ワールド行列を単位行列に初期化

	//スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl2, g_scl2.x, g_scl2.y, g_scl2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxScl2); //World * Scaling
    
	//平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs2, g_pos2.x, g_pos2.y, g_pos2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxTrs2); //World * Translation

	//回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot2, g_rot2.y, g_rot2.x, g_rot2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxRot2); //World * Rotation


	//ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld2);

	//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_NORMAL));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);




	//板(HARD)

	D3DXMATRIX mtxScl3; //スケーリング行列
	D3DXMATRIX mtxRot3; //回転行列
	D3DXMATRIX mtxTrs3; //平行移動行列

	g_pos3 = D3DXVECTOR3(g_posx3, g_posy3, g_posz3); //位置
	g_rot3 = D3DXVECTOR3(0, g_Rotation, 0); //向き(回転)
	g_scl3 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ(スケール)

	D3DXMatrixIdentity(&g_mtxworld3);     //ワールド行列を単位行列に初期化

	//スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl3, g_scl3.x, g_scl3.y, g_scl3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxScl3); //World * Scaling

	//平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs3, g_pos3.x, g_pos3.y, g_pos3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxTrs3); //World * Translation

	//回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot3, g_rot3.y, g_rot3.x, g_rot3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxRot3); //World * Rotation


	//ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld3);

	//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_HARD));
	//pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_SELECT));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


	//枠線

	D3DXMATRIX mtxScl4; //スケーリング行列
	D3DXMATRIX mtxRot4; //回転行列
	D3DXMATRIX mtxTrs4; //平行移動行列

	g_pos4 = D3DXVECTOR3(g_posx4, g_posy4, g_posz4); //位置
	g_rot4 = D3DXVECTOR3(0, g_Rotation, 0); //向き(回転)
	g_scl4 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ(スケール)

	D3DXMatrixIdentity(&g_mtxworld4);     //ワールド行列を単位行列に初期化

	//スケール行列を作成＆ワールド行列へ合成
	D3DXMatrixScaling(&mtxScl4, g_scl4.x, g_scl4.y, g_scl4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxScl4); //World * Scaling

	//平行移動行列を作成＆ワールド行列へ合成
	D3DXMatrixTranslation(&mtxTrs4, g_pos4.x, g_pos4.y, g_pos4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxTrs4); //World * Translation

	//回転行列を作成＆ワールド行列へ合成
	D3DXMatrixRotationYawPitchRoll(&mtxRot4, g_rot4.y, g_rot4.x, g_rot4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxRot4); //World * Rotation


	//ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld4);

	//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//描画したいポリゴンの頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ポリゴンの描画
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_SELECT));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);








	//DebugFont_Draw(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"x;%d y;%d",CameraEyeAt_x,CameraEyeAt_y);
}

void Mode_Initialize()
{
	cameraTP.Initialize(D3DXVECTOR3(-1.0f, -2.5f, 40.0f));
	mode.Initialize();
}

void Mode_Finalize()
{
	cameraTP.Finalize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mode.Finalize();
}

void Mode_Update()
{
	mode.Update();
}

void Mode_Draw()
{
	cameraTP.Draw();
	mode.Draw();
}