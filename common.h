#pragma once
#ifndef COMMON_H_
#define COMMON_H_

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080) 
#define PLAYER_SIZE_COEFFICIENT 2.5f
#define FPS_STANTARD 60.0f

enum SCENE {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_MODE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_RANKING,
};

void SetScene(SCENE s);
SCENE GetScene();
#endif