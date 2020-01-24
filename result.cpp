#include "result.h"
#include <string>
#include "debug_font.h"
#include "arrow.h"
#include "target.h"

using std::string;

static string str[ARROW_MAX] = {
	"ST",
	"ND",
	"RD",
	"TH",
	"TH"
};

static UI       board;
static UI       target;
static UI       mark[ARROW_MAX];
static Alphabet alpha[ARROW_MAX];
static Number   order[ARROW_MAX];
static Number   num[ARROW_MAX];


// ResultÇÃèâä˙âª
void Result_Initialize()
{
	board.Initialize(TEXTURE_INDEX_RESULT_BOARD, D3DXVECTOR2(512.0f, 512.0f), D3DXVECTOR2(BOARD_POS_X, BOARD_POS_Y));
	target.Initialize(TEXTURE_INDEX_TARGET, D3DXVECTOR2(250.0f, 250.0f), D3DXVECTOR2(BOARD_POS_X, BOARD_POS_Y));
	D3DXVECTOR2 mark_pos;
	float r = TARGET_SIZE / 2;
	for (int i = 0; i < ARROW_MAX; i++)
	{
		mark_pos.x = ArrowManager::arrows[i].posHit.x / r * target.size.x;
		mark_pos.y = -ArrowManager::arrows[i].posHit.y / r * target.size.y;
		mark[i].Initialize(TEXTURE_INDEX_RESULT_MARK, D3DXVECTOR2(50.0f, 50.0f), D3DXVECTOR2(target.pos.x + mark_pos.x, target.pos.y + mark_pos.y));
		alpha[i].Initialize(TEXTURE_INDEX_ALPHABET, str[i], D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(STRING_POS_X, STRING_POS_Y + 32.0f * i), 0, 0, 13, 2);
		order[i].Initialize(TEXTURE_INDEX_NUMBER, D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(STRING_POS_X - 32.0f * str[i].size(), alpha[i].pos.y), 0, 0, 10, 1);
		num[i].Initialize(TEXTURE_INDEX_NUMBER, D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(STRING_POS_X + (SCORE_DIGIT_MAX + 1) * 32.0f, alpha[i].pos.y), 0, 0, 10, 1, SCORE_DIGIT_MAX);
	}
}

// ResultÇÃèIóπèàóù
void Result_Finalize()
{

}

// ResultÇÃçXêV
void Result_Update()
{

}

// ResultÇÃï`âÊ
void Result_Draw()
{
	board.Draw();
	target.Draw();
	for (int i = 0; i < ARROW_MAX; i++)
	{
		mark[i].Draw();
		alpha[i].Draw();
		order[i].Draw(i + 1);
		num[i].Draw(ArrowManager::arrows[i].score, num[i].digit_i);
		DebugFont_Draw(640, 32 * (i + 1), "%dî≠ñ⁄: %d", i + 1, ArrowManager::arrows[i].score);
		DebugFont_Draw(2, 32 * (i + 1), "posHit x: %.02lf  y: %.02lf", ArrowManager::arrows[i].posHit.x, ArrowManager::arrows[i].posHit.y);
	}
}