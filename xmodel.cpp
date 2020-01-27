#include "xmodel.h"
#include "texture.h"

// Modelの初期化（コンストラクタ）
XModel::XModel()
{
	//pD3DXTextureModel = NULL;
	pD3DXMeshModel = NULL;
	pD3DXBuffModel = NULL;
	nNumMatModel = NULL;
}

XModel::~XModel()
{
	/*if (pD3DXTextureModel != NULL)
	{
		pD3DXTextureModel->Release();
		pD3DXTextureModel = NULL;
	}*/

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

// XModelの初期化
void XModel::XModel_Initialize(const char *filepass)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// 初期化時に1回読み込む
	if (D3D_OK != D3DXLoadMeshFromX(
		filepass,
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
void XModel::XModel_Finalize()
{

}

// XModelの更新
void XModel::XModel_Update()
{

}

// XModelの描画
void XModel::XModel_Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATERIAL* pD3DXMat; // マテリアルの配列の先頭のポインタを受け取る

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatModel; nCntMat++)
	{
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
		pD3DXMeshModel->DrawSubset(nCntMat);
	}
}