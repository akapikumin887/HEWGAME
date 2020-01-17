#include "score.h"
#include "debug_font.h"

// Scoreの初期化（コンストラクタ）
Score::Score()
{
	num = new Number;
	alpha = new Alphabet;
}

// Scoreの終了処理（デストラクタ）
Score::~Score()
{
	delete num;
	delete alpha;
}

// Scoreの初期化
void Score::Initialize()
{	
	// Alphabetの初期化
	strncpy(alpha->word, "SCORE", 8);
	alpha->len = strlen(alpha->word);

	for (int i = 0; i < 5; i++)
	{
		s[i] = 0;
	}
}

// Scoreの終了処理
void Score::Finalize()
{
	
}

// Scoreの更新
void Score::Update()
{
	
}

// Scoreの描画
void Score::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Sprite_SetColor_2D(num->color); // 色のセット

	// スコア表示
	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		int tmp;
		// 1の位から始まる
		tmp = num->n / (int)powf((float)10, (float)i);
		tmp = tmp % 10; // 1の位の値を取り出す

		// 取り出した値を表示
		Sprite_Draw_2D(num->texture_index,
			SCREEN_WIDTH - num->pos.x * i * 2 - num->pos.x, num->pos.y,
			num->tx + tmp * num->tw, num->ty,
			num->tw, num->th);
	}
	
	Sprite_SetColor_2D(alpha->color); // 色のセット

	// アルファベットの表示
	for (int i = 0; i < alpha->len; i++)
	{
		Sprite_Draw_2D(alpha->texture_index,
			SCREEN_WIDTH - num->pos.x * DIGIT_MAX * 2 - alpha->pos.x * (alpha->len - i) * 2, alpha->pos.y,
			alpha->tx + ((alpha->word[i] - 65) % 13) * alpha->tw, alpha->ty + (alpha->word[i] - 65) / 13 * alpha->th,
			alpha->tw, alpha->th);
	}
}

// Scoreの加算
void Score::Add_Score(int s)
{
	num->n += s;
}