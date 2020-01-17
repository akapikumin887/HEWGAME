#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"
#include "ui.h"

#ifndef Score_H_
#define Score_H_

class Score
{
public:
	Number   *num;
	Alphabet *alpha;
	int      s[5];

	Score();  // Score�̏������i�R���X�g���N�^�j
	~Score(); // Score�̏I�������i�f�X�g���N�^�j

	void Initialize(); // Score�̏�����
	void Finalize();   // Score�̏I������
	void Update();     // Score�̍X�V
	void Draw();       // Score�̕`��

	void Add_Score(int s); // Score�̉��Z
};
#endif