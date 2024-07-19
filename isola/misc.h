#ifndef MISC_H
#define MISC_H




#define isolaSTRL_(X) #X
#define isolaSTR_(X) isolaSTRL_(X)


#define isolaARRAYSIZE_(x) (sizeof(x)/sizeof(x[0]))


#define isolaCLEARLINUXTERMINAL_() SDL_Log("\033[2J");;
	



#endif
