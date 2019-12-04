#include "xmodel.h"

static LPD3DXMESH pD3DXMeshModel; // �|���S�����f����\��
static LPD3DXBUFFER pD3DXBuffModel; // �}�e���A�����Ǘ�����
static DWORD nNumMatModel; // �}�e���A����

// XModel�̏�����
void XModel_Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// ����������1��ǂݍ���
	if (D3D_OK != D3DXLoadMeshFromX(
		"�t�@�C����",
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

// XModel�̏I������
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

// XModel�̍X�V
void XModel_Update()
{

}

// XModel�̕`��
void XModel_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATERIAL* pD3DXMat; // �}�e���A���̔z��̐擪�̃|�C���^���󂯎��

	pD3DXMat = (D3DXMATERIAL*)pD3DXBuffModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)nNumMatModel; nCntMat++)
	{
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		//pDevice->SetTexture(); // �K�v������΃e�N�X�`�����Z�b�g
		pD3DXMeshModel->DrawSubset(nCntMat);
	}
}