#include "wind.h"
#include "debug_font.h"
#include "arrow.h"

D3DXVECTOR3 Wind::speed;

// Wind�̏�����
void Wind::Initialize()
{
	speed = D3DXVECTOR3(((rand() % 16) * 1.0f + 5.0f) / 2, 0.0f, 0.0f) / WIND_FORCE_MAG;

	if (rand() % 2 == 0)
	{
		speed *= -1;
	}
}

// Wind�̏I������
void Wind::Finalize()
{

}

// Wind�̍X�V
void Wind::Update()
{
	
}

// Wind�̕`��
void Wind::Draw()
{
	
}