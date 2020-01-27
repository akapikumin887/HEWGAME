#pragma once
#include <d3d.h>

#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C���p�X�{�t�@�C���e����������Ȃ���Α��₷

enum TextureIndex {
	TEXTURE_INDEX_BG,
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_TITLE_START,
	TEXTURE_INDEX_TITLE_RANKING,
	TEXTURE_INDEX_TITLE_ARROW,
	TEXTURE_INDEX_MODE_EASY,
	TEXTURE_INDEX_MODE_NORMAL,
	TEXTURE_INDEX_MODE_HARD,
	TEXTURE_INDEX_MODE_SELECT,
	TEXTURE_INDEX_AIMING,
	TEXTURE_INDEX_BOW,
	TEXTURE_INDEX_ARROW,
	TEXTURE_INDEX_TARGET,
	TEXTURE_INDEX_SKY,
	TEXTURE_INDEX_PLANE,
	TEXTURE_INDEX_SIDE,
	TEXTURE_INDEX_RESULT_BOARD,
	TEXTURE_INDEX_RESULT_MARK,
	TEXTURE_INDEX_NUMBER,
	TEXTURE_INDEX_ALPHABET,
	TEXTURE_INDEX_MAX
};

int Texture_Load(void); // �e�N�X�`���̃��[�h
void Texture_Release(void); // �e�N�X�`���̃����[�X
LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index); // �e�N�X�`���C���^�[�t�F�[�X�̎擾
int Texture_GetWidth(TextureIndex index); // �摜�T�C�Y�̎擾
int Texture_GetHeight(TextureIndex index); // �摜�T�C�Y�̎擾