#include "mode.h"

static Mode mode;

// Mode�̏�����
void Mode_Initialize()
{

}

// Mode�̏I������
void Mode_Finalize()
{

}

// Mode�̍X�V
void Mode_Update()
{

}

// Mode�̕`��
void Mode_Draw()
{

}

Mode::Mode()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_MODE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Mode::~Mode()
{

}