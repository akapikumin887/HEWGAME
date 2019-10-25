#include <d3dx9.h>
#include <d3d.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"


#define TEXTURE_FILENAME_MAX (64)

// �t�@�C���Ǘ��\��
typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
}TextureFile;

// �e�N�X�`���z��
static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/BG01.jpg", SCREEN_WIDTH, SCREEN_HEIGHT },
	{ "asset/texture/aiming.png", 60, 60 },
	{ "asset/texture/arrow00.tga", 120, 120 },
	{ "asset/texture/arrow01.tga", 150, 150 },
	// �w�b�_�[�Œ�`����enum�ƑΉ����Ă���̂Ő����قȂ������`��������킷�摜�t�@�C���̏��Ԃ��قȂ����肵�Ȃ��悤�ɂ���
};

// �e�N�X�`���̐�
static const int TEXTURE_FILE_COUNT =
sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

// enum�ƃe�N�X�`���t�@�C���̐����������H
static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT,
	"TEXTURE_INDEX_MAX!=TEXTURE_FILE_COUNT");

// �O���[�o���ϐ�
static LPDIRECT3DTEXTURE9
g_pTextures[TEXTURE_FILE_COUNT];

int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();
	if (!pDevice)return TEXTURE_FILE_COUNT; // ���[�h�ł��Ȃ���������Ԃ�

	int failed_count = 0; // ���[�h�ł��Ȃ�������
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if ( FAILED( D3DXCreateTextureFromFile(
			pDevice,
			g_TextureFiles[i].filename,
			&g_pTextures[i])))
		{
			// �Ȃ񂩃G���[�\��
			failed_count++;
		}
	}
	return failed_count;
}

void Texture_Release(void)
{
	for (int i = 0; i < TEXTURE_FILE_COUNT; i++)
	{
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

// �C���^�[�t�F�[�X�擾
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL; // �o�^����Ă��Ȃ����o�Ȃ��ꍇ�̓G���[�Ƃ���NULL
	}
	return g_pTextures[index]; // �w��ID�̃C���^�[�t�F�[�X��Ԃ�
}

int Texture_GetWidth(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[index].width;
}

int Texture_GetHeight(TextureIndex index)
{
	if (index < 0 || index >= TEXTURE_INDEX_MAX)
	{
		return NULL;
	}
	return g_TextureFiles[index].height;
}