#include "wind.h"
#include "debug_font.h"
#include "mode.h"

static Wind wind; 
static ModeIndex ModeIdx;

// Wind�̏�����
void Wind_Initialize()
{
	ModeIdx = GetMode();
	if (ModeIdx == MODE_EASY)
	{
		wind.WindSpeed.x = 0;
	}
	else
	{
		wind.WindSpeed.x = 2.0f;
	}

	//	wind.WindSpeed.x = 3.0f * (rand() % 10);
	//	// ��
	//	if (rand() % 2 == 0)
	//	{
	//		wind.WindSpeed.x *= -1;
	//		wind.wright = false;
	//	}
	//	// �E
	//	else
	//	{
	//		wind.wright = true;
	//	}
	//}
}

// Wind�̏I������
void Wind_Finalize()
{

}

// Wind�̍X�V
void Wind_Update()
{

}

// Wind�̕`��
void Wind_Draw()
{
	if (ModeIdx != MODE_EASY)
	{
		DebugFont_Draw(2, 92, "����: �E");
		DebugFont_Draw(2, 122, "WindSpeedX: %.2lf", wind.WindSpeed.x);
	}
}

// Wind�̏��̎擾
Wind* GetWind()
{
	return &wind;
}

Wind::Wind()
{

}

Wind::~Wind()
{
	
}