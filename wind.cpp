#include "wind.h"
#include "debug_font.h"
#include "arrow.h"

D3DXVECTOR3 Wind::speed;

// Windの初期化
void Wind::Initialize()
{
	speed = D3DXVECTOR3(((rand() % 16) * 1.0f + 5.0f) / 2, 0.0f, 0.0f) / WIND_FORCE_MAG;

	if (rand() % 2 == 0)
	{
		speed *= -1;
	}
}

// Windの終了処理
void Wind::Finalize()
{

}

// Windの更新
void Wind::Update()
{
	
}

// Windの描画
void Wind::Draw()
{
	
}