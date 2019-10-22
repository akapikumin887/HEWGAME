#ifndef COMMON_H_
#define COMMON_H_

#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720) 
#define PLAYER_SIZE_COEFFICIENT 2.5f

typedef enum {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
}SCENE;

void SetScene(SCENE s);
#endif