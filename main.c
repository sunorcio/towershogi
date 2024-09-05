



#include <time.h>

#include <isola/isola.h>

#include <scene/towermenu/towermenu.h>
#include <scene/towershogi/towershogi.h>




int main(int argc, char **argv){

	srand(time(0));
	isolaInit();




	if (towermenuLoop()) {
		towershogiLoop();
	}




	isolaQuit();
	return 0;
}




