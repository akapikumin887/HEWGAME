#include "title.h"

static Title title;

// Title�̏�����
void Title_Initialize()
{

}

// Title�̏I������
void Title_Finalize()
{

}

// Title�̍X�V
void Title_Update()
{

}

// Title�̕`��
void Title_Draw()
{

}

Title::Title()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = SCREEN_HEIGHT * 0.5f;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // �F��K���ɍ��
	TextureIndex = TEXTURE_INDEX_TITLE;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

Title::~Title()
{

}