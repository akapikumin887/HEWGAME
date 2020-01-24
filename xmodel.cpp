#include "xmodel.h"
#include "texture.h"

// Model�̏������i�R���X�g���N�^�j
XModel::XModel()
{
	pD3DXTextureModel = NULL;
	pD3DXMeshModel = NULL;
	pD3DXBuffModel = NULL;
	nNumMatModel = NULL;
}

// XModel�̏�����
void XModel::XModel_Initialize(const char *filepass)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ����������1��ǂݍ���
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
	pD3DXTextureModel = Texture_GetTexture(TEXTURE_INDEX_MAX);
}

// XModel�̏I������
void XModel::XModel_Finalize()
{
	if (pD3DXTextureModel != NULL)
	{
		pD3DXTextureModel->Release();
		pD3DXTextureModel = NULL;
	}

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

// XModel�̍X�V
void XModel::XModel_Update()
{

}

// XModel�̕`��
void XModel::XModel_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATERIAL* pD3DXMat; // �}�e���A���̔z��̐擪�̃|�C���^���󂯎��

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatModel; nCntMat++)
	{
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		if (nCntMat == 0)
		{
			
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MAX));
		}
		pD3DXMeshModel->DrawSubset(nCntMat);
	}
}