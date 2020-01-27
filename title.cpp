#include "title.h"
#include "input.h"
#include "ui.h"
#include "fade.h"

static UI bg;
static UI title;
static UI start;
static UI ranking;
static UI arrow;

static bool up;

void Title_Initialize(void)
{	
	bg.Initialize(TEXTURE_INDEX_BG, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	title.Initialize(TEXTURE_INDEX_TITLE, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	start.Initialize(TEXTURE_INDEX_TITLE_START, D3DXVECTOR2((float)Texture_GetWidth(TEXTURE_INDEX_TITLE_START), (float)Texture_GetHeight(TEXTURE_INDEX_TITLE_START)),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f));
	ranking.Initialize(TEXTURE_INDEX_TITLE_RANKING, D3DXVECTOR2((float)Texture_GetWidth(TEXTURE_INDEX_TITLE_RANKING), (float)Texture_GetHeight(TEXTURE_INDEX_TITLE_RANKING)),
		D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.85f));
	arrow.Initialize(TEXTURE_INDEX_TITLE_ARROW, D3DXVECTOR2((float)Texture_GetWidth(TEXTURE_INDEX_TITLE_ARROW), (float)Texture_GetHeight(TEXTURE_INDEX_TITLE_ARROW)),
		D3DXVECTOR2(start.pos.x + start.tw * 0.5f, start.pos.x));
	arrow.pos.x += arrow.tw * 0.6f;
	up = true;
}

void Title_Finalize(void)
{
	//égÇ¡ÇΩÇ‡ÇÃÇÃèàóùÅAâï˙
}

void Title_Update(void)
{

	if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S))
	{
		up = !up;
	}
	
	if (up)
	{
		arrow.pos.y = start.pos.y;

		if (GetKeyboardTrigger(DIK_RETURN))
		{
			Fade(SCENE_MODE);
		}
	}
	else
	{
		arrow.pos.y = ranking.pos.y;
	}
}

void Title_Draw(void)
{
	bg.Draw();
	title.Draw();
	start.Draw();
	ranking.Draw();
	arrow.Draw();
}

