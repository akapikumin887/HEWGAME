#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "common.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "sprite.h"
#include "texture.h"
#include "camera.h"

// �O���[�o���ϐ�
static D3DCOLOR g_Color = 0xffffffff; // ���s����
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxGWorld;

//====================================================
// ������
//====================================================
void Sprite_Initialize_2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	// �o�[�e�b�N�X�o�b�t�@���쐬����
	pDevice->CreateVertexBuffer(sizeof(Vertex_2D) * 4, // 4���_�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_DEFAULT,
		&g_pVertexBuffer,
		NULL);

	// �C���f�b�N�X�o�b�t�@���g��
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, // �C���f�b�N�X6���Ԃ�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIndexBuffer,
		NULL);

	// �C���f�b�N�X�ɏ�������ł���
	LPWORD pIndex = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0); // �C���f�b�N�X�z��̃|�C���^�����炤
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;
	g_pIndexBuffer->Unlock(); // �`�����ݏI��
}

//====================================================
// �X�v���C�g�\��
//====================================================
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx,int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�

	Vertex_2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

	// ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X
	
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_Draw_Matrix_2D(TextureIndex texture_index, D3DXVECTOR2 s, D3DXVECTOR2 p, int tx, int ty, int tw, int th, float r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// �s��ϐ��錾
	D3DXMATRIX mtxWorld; // ���[���h�s��
	D3DXMATRIX mtxScl;   // �X�P�[���s��
	D3DXMATRIX mtxRot;   // ��]�s��
	D3DXMATRIX mtxTrs;   // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, D3DXToRadian(r));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�
	
	Vertex_2D* pv;

	float	lx = s.x / 2.0f;
	float	ly = s.y / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

	// ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		-lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		+lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		-lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		+lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}

	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	// ���_���W��ϊ�����
	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(
			&pv[i].position, // �ϊ����ʂ̊i�[��|�C���^
			&pv[i].position, // �ϊ��O�f�[�^�̃|�C���^
			&mtxWorld);      // �ϊ��Ɏg���s��̃|�C���^
	}

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer,    // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex_2D));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

//====================================================
// BG�p�X�v���C�g�`��
// �@tx, ty �e�N�X�`��������W�@�@tw, th �\���X�v���C�g�T�C�Y
//====================================================
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}

	float w = (float)Texture_GetWidth(texture_index);//�e�N�X�`���T�C�Y
	float h = (float)Texture_GetHeight(texture_index);

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = u[0] + 1.0f;
	v[1] = v[0] + 1.0f; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�

	Vertex_2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

    // ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_SetColor_2D(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Finalize_2D(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

// �X�P�[���s��̐ݒ�(���[���h�s��̏��������܂�)
D3DXMATRIX Set_Mtx_Scl(D3DXMATRIX mtx, D3DXVECTOR3 s)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl; // �X�P�[�����O�s��

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, s.x, s.y, s.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScl); // World * Scaling

	return mtx;
}

// ��]�s��̐ݒ�
D3DXMATRIX Set_Mtx_Rot(D3DXMATRIX mtx, D3DXVECTOR3 r)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxRot; // ��]�s��

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(r.y), D3DXToRadian(r.x), D3DXToRadian(r.z));
	D3DXMatrixMultiply(&mtx, &mtx, &mtxRot); // World * Rotation

	return mtx;
}

// ���s�ړ��s��̐ݒ�
D3DXMATRIX Set_Mtx_Trs(D3DXMATRIX mtx, D3DXVECTOR3 p)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxTrs; // ���s�ړ��s��

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, p.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrs); // World * Translation

	return mtx;
}

// Billboard�i�o�b�t�@�j�̐���
D3DXVECTOR3 CreateBillboard(D3DXVECTOR3 sz, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY, // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,      // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,    // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&vb,                // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);              // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	vb->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-sz.x / 2, sz.y / 2, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(sz.x / 2, sz.y / 2, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-sz.x / 2, -sz.y / 2, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(sz.x / 2, -sz.y / 2, 0.0f);

	for (int i = 0; i < 4; i++)
	{
		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	vb->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	ib->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 4; i++)
	{
		pIdx[i] = i;
	}

	ib->Unlock();

	return sz;
}

// Billboard�̕`�掞��Camera��]�̃Z�b�g
D3DXMATRIX Set_Biillboard_Camera(D3DXMATRIX mtx)
{
	mtx._41 = 0.0f; // Camera�̕��s�ړ��s���0��
	mtx._42 = 0.0f;
	mtx._43 = 0.0f;
	D3DXMatrixTranspose(&mtx, &mtx); // Camera�s��̓]�u

	return mtx;
}

// Billboard�̕`��
void Draw_Billboard(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXMATRIX &mtxW, D3DXMATRIX mtxC, LPDIRECT3DVERTEXBUFFER9 vb, LPDIRECT3DINDEXBUFFER9 ib)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxScl;    // �X�P�[�����O�s��
	D3DXMATRIX mtxCamera; // ��]�s��(Camera�]�u)
	D3DXMATRIX mtxTrs;    // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxW); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, 1, 1, 1);
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	mtxCamera = mtxC;
	mtxCamera._41 = 0.0f; // Camera�̕��s�ړ��s���0��
	mtxCamera._42 = 0.0f;
	mtxCamera._43 = 0.0f;
	D3DXMatrixTranspose(&mtxCamera, &mtxCamera); // Camera�s��̓]�u
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxCamera); // World * Rotation

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, p.x, p.y, p.z);
	D3DXMatrixMultiply(&mtxW, &mtxW, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxW);

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, vb, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(ib);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// Face
// Face�̏������i�R���X�g���N�^�j
Face::Face()
{
	pD3DVtxBuffer = NULL;
	pD3DIdxBuffer = NULL;
}

// Face�̏I�������i�f�X�g���N�^�j
Face::~Face()
{
	if (pD3DVtxBuffer)
	{
		pD3DVtxBuffer->Release();
		pD3DVtxBuffer = NULL;
	}

	if (pD3DIdxBuffer)
	{
		pD3DIdxBuffer->Release();
		pD3DIdxBuffer = NULL;
	}
}

// Face�i�o�b�t�@�j�̐���
void Face::CreateFace(D3DXVECTOR3 sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY,   // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,        // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,      // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffer, // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);                // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-sz.x / 2, sz.y, sz.z / 2);
	pVtx[1].vtx = D3DXVECTOR3(sz.x / 2, sz.y, sz.z / 2);
	pVtx[2].vtx = D3DXVECTOR3(-sz.x / 2, sz.y, -sz.z / 2);
	pVtx[3].vtx = D3DXVECTOR3(sz.x / 2, sz.y, -sz.z / 2);

	for (int i = 0; i < 4; i++)
	{
		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	pD3DVtxBuffer->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBuffer,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	pD3DIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 4; i++)
	{
		pIdx[i] = i;
	}

	pD3DIdxBuffer->Unlock();
}

// Face�̕`��
void Face::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, pD3DVtxBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBuffer);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// FaceEX
// FaceEX�̏������i�R���X�g���N�^�j
FaceEX::FaceEX()
{
	pD3DVtxBufferEX = NULL;
	pD3DIdxBufferEX = NULL;
}

// FaceEX�̏I�������i�f�X�g���N�^�j
FaceEX::~FaceEX()
{
	if (pD3DVtxBufferEX)
	{
		pD3DVtxBufferEX->Release();
		pD3DVtxBufferEX = NULL;
	}

	if (pD3DIdxBufferEX)
	{
		pD3DIdxBufferEX->Release();
		pD3DIdxBufferEX = NULL;
	}
}

// FaceEX�i�o�b�t�@�j�̐���(�k�ރ|���S��)
void FaceEX::CreateFaceEX(D3DXVECTOR3 sz, D3DXVECTOR3 szn)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	kx = (int)(sz.x / szn.x); // ���i��
	kz = (int)(sz.z / szn.z); // �c�i��

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((kx + 1) * (kz + 1)), // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY, // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,      // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,    // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBufferEX,   // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);              // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBufferEX->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < (kx + 1) * (kz + 1); i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i].vtx = D3DXVECTOR3((i % (kx + 1)) * szn.x - sz.x / 2.0f, sz.y, sz.z / 2.0f - i / (kx + 1) * szn.z);

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		//pVtx[i].tex = D3DXVECTOR2((float)(i % (kx + 1)), (float)(i / (kx + 1))); // �}�X���ƂɃe�N�X�`����\��
		pVtx[i].tex = D3DXVECTOR2((float)(i % (kx + 1)) / kx, (float)(i / (kx + 1)) / kz); // �S�̂Ƀe�N�X�`����\��
	}

	//���_�f�[�^���A�����b�N����
	pD3DVtxBufferEX->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBufferEX,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	pD3DIdxBufferEX->Lock(0, 0, (void**)&pIdx, 0);

	int count = 0;

	for (int i = 0; i < kx; i++)
	{
		for (int j = 0; j < kz + 1; j++)
		{
			pIdx[count] = i + j * (kx + 1);
			pIdx[count + 1] = pIdx[count] + 1;
			count += 2;
		}
		pIdx[count] = pIdx[count - 1];
		pIdx[count + 1] = pIdx[count - (kz + 1) * 2 + 1];
	}

	pD3DIdxBufferEX->Unlock();
}

// FaceEX�̕`��
void FaceEX::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, pD3DVtxBufferEX, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBufferEX);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)), 0, kz * kx * 2 + (kx - 1) * 2);
}

// Cube
// Cube�̏������i�R���X�g���N�^�j
Cube::Cube()
{
	pD3DVtxBuffer = NULL;
	pD3DIdxBuffer = NULL;
}

// Cube�̏I�������i�f�X�g���N�^�j
Cube::~Cube()
{
	if (pD3DVtxBuffer)
	{
		pD3DVtxBuffer->Release();
		pD3DVtxBuffer = NULL;
	}

	if (pD3DIdxBuffer)
	{
		pD3DIdxBuffer->Release();
		pD3DIdxBuffer = NULL;
	}
}

// Cube�i�o�b�t�@�j�̐���
void Cube::CreateCube(D3DXVECTOR3 sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXVECTOR3 s = sz / 2;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY, // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,      // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,    // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBuffer,     // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);              // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-s.x, s.y, s.z);
	pVtx[1].vtx = D3DXVECTOR3(s.x, s.y, s.z);
	pVtx[2].vtx = D3DXVECTOR3(-s.x, s.y, -s.z);

	pVtx[3].vtx = pVtx[1].vtx;
	pVtx[4].vtx = D3DXVECTOR3(s.x, s.y, -s.z);
	pVtx[5].vtx = pVtx[2].vtx;

	pVtx[6].vtx = pVtx[3].vtx;
	pVtx[7].vtx = D3DXVECTOR3(s.x, -s.y, s.z);
	pVtx[8].vtx = pVtx[4].vtx;

	pVtx[9].vtx = pVtx[7].vtx;
	pVtx[10].vtx = D3DXVECTOR3(s.x, -s.y, -s.z);
	pVtx[11].vtx = pVtx[8].vtx;

	pVtx[12].vtx = pVtx[9].vtx;
	pVtx[13].vtx = D3DXVECTOR3(-s.x, -s.y, s.z);
	pVtx[14].vtx = pVtx[10].vtx;

	pVtx[15].vtx = pVtx[13].vtx;
	pVtx[16].vtx = D3DXVECTOR3(-s.x, -s.y, -s.z);
	pVtx[17].vtx = pVtx[14].vtx;

	pVtx[18].vtx = pVtx[15].vtx;
	pVtx[19].vtx = pVtx[0].vtx;
	pVtx[20].vtx = pVtx[16].vtx;

	pVtx[21].vtx = pVtx[19].vtx;
	pVtx[22].vtx = pVtx[5].vtx;
	pVtx[23].vtx = pVtx[20].vtx;

	pVtx[24].vtx = pVtx[6].vtx;
	pVtx[25].vtx = pVtx[21].vtx;
	pVtx[26].vtx = pVtx[12].vtx;

	pVtx[27].vtx = pVtx[25].vtx;
	pVtx[28].vtx = pVtx[15].vtx;
	pVtx[29].vtx = pVtx[26].vtx;

	pVtx[30].vtx = pVtx[22].vtx;
	pVtx[31].vtx = pVtx[11].vtx;
	pVtx[32].vtx = pVtx[23].vtx;

	pVtx[33].vtx = pVtx[31].vtx;
	pVtx[34].vtx = pVtx[17].vtx;
	pVtx[35].vtx = pVtx[32].vtx;

	// �@���x�N�g���̐ݒ�
	for (int i = 0; i < 6; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	for (int i = 6; i < 12; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
	for (int i = 12; i < 18; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	}
	for (int i = 18; i < 24; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	}
	for (int i = 24; i < 30; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
	for (int i = 30; i < 36; i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	}

	//���ˌ��̐ݒ�
	for (int i = 0; i < 36; i++)
	{
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	for (int i = 0; i < 36; i += 6)
	{
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[i + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i + 3].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[i + 5].tex = D3DXVECTOR2(0.0f, 1.0f);
	}

	//���_�f�[�^���A�����b�N����
	pD3DVtxBuffer->Unlock();

	// ���X�g�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBuffer,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	pD3DIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < 36; i++)
	{
		pIdx[i] = i;
	}

	pD3DIdxBuffer->Unlock();
}

// Cube�̕`��
void Cube::Draw(TextureIndex tex_idx)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, pD3DVtxBuffer, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBuffer);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, Texture_GetTexture(tex_idx));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12);
}