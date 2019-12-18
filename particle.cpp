/*

	���m�[�T�|�[�g�ł���Ώ���ɎQ�l�����p���Ă��n�j

	�������N���X�ɂȂ��Ă���̂ŁAC���ꕗ�ɂ���Ȃ�
	���̂܂܊��p����Ȃ�͎��R

*/

#include "particle.h"
#include "camera.h"

///////////////////////////////////////////////////////////
//
//PARTICLE�@
//   �p�[�e�B�N�����Ǘ�����N���X
//
///////////////////////////////////////////////////////////

//=============================================
//�X�^�e�B�b�N�����o�[
//=============================================
int				PARTICLE::numparticle;	//�p�[�e�B�N����
particle_base*  PARTICLE::obj_pool;		//�p�[�e�B�N���z��
VERTEX_3D		PARTICLE::vertex[4];	//�p�[�e�B�N�����_�f�[�^

//=============================================
//������
//
//int num_particle �p�[�e�B�N���ő吔
//=============================================
void PARTICLE::Init(int num_particle)
{
	//�p�[�e�B�N���\���̂̔z����쐬
	obj_pool = new particle_base[ num_particle ];

	//�S�p�[�e�B�N��������
	for(int i=0;i<num_particle;i++)
	{	//�p�[�e�B�N���ɏ��������w������
		obj_pool[i].InitParticle();
	}

	//�p�[�e�B�N�����ۑ�	
	numparticle = num_particle;

	//���_�쐬
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
//��n��
//=============================================
void PARTICLE::Uninit()
{

	//�p�[�e�B�N���\���̔z��̉��
	if(obj_pool != NULL)
	{
		delete[] obj_pool;
		obj_pool = NULL;
	}
}

//=============================================
//�p�[�e�B�N���̍X�V
//=============================================
void PARTICLE::Update()
{

	//�����Ă���S�Ẵp�[�e�B�N��������
	for(int i=0;i<numparticle;i++)
	{
		if(obj_pool[i].GetLife() > -1)
		{	//�p�[�e�B�N���ɍX�V���w������
			obj_pool[i].UpdateParticle();
		}
	}

	//�p�[�e�B�N�������Ď�����
	PARTICLE::ParticleOccer();
}

//=============================================
//�p�[�e�B�N���\��
//=============================================
void PARTICLE::Draw()
{

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//���u�����hON
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	//�����Ă���S�Ẵp�[�e�B�N��������
	for(int i=0;i<numparticle;i++)
	{
		if(obj_pool[i].GetLife() > -1)
		{	//�p�[�e�B�N���ɕ\�����w������
			obj_pool[i].DrawParticle();
		}
	}
	
	//���u�����hOFF
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//=============================================
//�p�[�e�B�N�������Ď�
//
// Life��0�����Ȃ珉����
//=============================================
void PARTICLE::ParticleOccer()
{

	//�����̗��Ă���p�[�e�B�N����T���ĕ���������
	for(int i=0; i<numparticle; i++)
	{
		if(obj_pool[i].GetLife() < 0 )
		{	//�p�[�e�B�N���ɍď��������w������
			obj_pool[i].InitParticle();
			//break;//1�t���[����1������
		}
	}
}

//=============================================
//�p�[�e�B�N���̒��_�F�Z�b�g
//
//D3DXVECTOR4 col ���_�J���[�@xyzw = RGBA
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
//	�p�[�e�B�N�����̂��̂̃N���X
//
///////////////////////////////////////////////////////////

//=============================================
//�p�[�e�B�N���̏�����
//=============================================
void	particle_base::InitParticle()
{
	//�J���[�Z�b�g
	color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	//�X�P�[���Z�b�g
	scl = D3DXVECTOR3(1,1,1);

	//�����Z�b�g
	int life = rand() % 130 + 50; //�����{��{�l
	Life = life;

	//���W�Z�b�g (float)rand() / (float)RAND_MAX��0.0�`1.0������
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
//�p�[�e�B�N���̍X�V
//=============================================
void	particle_base::UpdateParticle()
{

	//�Ȃɂ����Ȃ��A�������������邾��
	Life--;
	if(Life < 0)
	{	//0�����ɂȂ�Ə��������Ă����
		Life = -1;//�O�̂���
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
//�p�[�e�B�N���̕\��
//=============================================
void	particle_base::DrawParticle()
{

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�r���[�s��擾&�r���{�[�h�s��쐬
	D3DXMATRIX	BillMatrix = Get_Camera()->mtxView;//<<�J�����擾��return g_mtxView;��main�ɍ��
	BillMatrix._41 = 
	BillMatrix._42 = 
	BillMatrix._43 = 0.0f;
	D3DXMatrixTranspose(&BillMatrix, &BillMatrix);

	//�X�P�[�����O�s��
	D3DXMATRIX	mtxScl;
	D3DXMatrixIdentity(&mtxScl);
	mtxScl._11 = scl.x;
	mtxScl._22 = scl.y;
	mtxScl._33 = scl.z;

	//���s�ړ��s��
	D3DXMATRIX	mtxTrs;
	D3DXMatrixIdentity(&mtxTrs);
	mtxTrs._41 = pos.x;
	mtxTrs._42 = pos.y;
	mtxTrs._43 = pos.z;

	//���[���h�s��쐬&�Z�b�g
	D3DXMatrixMultiply(&BillMatrix, &mtxScl, &BillMatrix);
	D3DXMatrixMultiply(&BillMatrix, &BillMatrix, &mtxTrs);
	pDevice->SetTransform(D3DTS_WORLD, &BillMatrix);

	//���_�t�H�[�}�b�g�̃Z�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̃Z�b�g
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_KIZUNA));

	//���C�e�B���OOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�p�[�e�B�N����`�悷��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, PARTICLE::GetVertex(), sizeof(VERTEX_3D));
}