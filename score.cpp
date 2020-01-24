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
	//strncpy(alpha->word, "SCORE", 8);
	//alpha->len = strlen(alpha->word);
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
	//num->Draw(SCORE_DIGIT_MAX, ArrowManager::score_t);
}

// Scoreの加算
void Score::Add_Score(int s)
{
	//num->n += s;
}