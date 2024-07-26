



#include <time.h>


#include <isola/isola.h>
#include <isola/misc.h>


#include <scene/mainmenu.h>




int main(int argc, char **argv){

	srand(time(0));
	isolaInit();




	if (mainmenuLoop()) {
		/* triakontadisLoop(); */
	}




	isolaQuit();
	return 0;
}




