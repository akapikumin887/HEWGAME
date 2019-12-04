#include "xmodel.h"

static LPD3DXMESH pD3DXMeshModel; // ポリゴンモデルを表す
static LPD3DXBUFFER pD3DXBuffModel; // マテリアルを管理する
static DWORD nNumMatModel; // マテリアル数

// XModelの初期化
void XModel_Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// 初期化時に1回読み込む
	if (D3D_OK != D3DXLoadMeshFromX(
		"ファイル名",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pD3DXBuffModel,
		NULL,
		&nNumMatModel,
		&pD3DXMeshModel))
	{

	}
}

// XModelの終了処理
void XModel_Finalize()
{
	if (pD3DXMeshModel != NULL)
	{
		pD3DXMeshModel->Release();
		pD3DXMeshModel = NULL;
	}

	if (pD3DXBuffModel != NULL)
	{
		pD3DXBuffModel->Release();
		pD3DXBuffModel = NULL;
	}
}

// XModelの更新
void XModel_Update()
{

}

// XModelの描画
void XModel_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATERIAL* pD3DXMat; // マテリアルの配列の先頭のポインタを受け取る

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatModel; nCntMat++)
	{
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		//pDevice->SetTexture(); // 必要があればテクスチャをセット
		pD3DXMeshModel->DrawSubset(nCntMat);
	}
}