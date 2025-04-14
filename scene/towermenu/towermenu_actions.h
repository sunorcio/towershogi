#ifndef TOWERMENU_SCENE_H
 #include "towermenu_scene.c"
#endif
 



static void quitTowermenu(void){

	currentScene->state.run = 0;
}

static void playTowermenu(void){

	currentScene->state.returnControlValue = 1;
	currentScene->state.run = 0;
}
