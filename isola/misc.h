#ifndef MISC_H
#define MISC_H




#define isolaLITSTR_(X) #X
#define isolaVARSTR_(X) isolaLITSTR_(X)


#define isolaARRAYSIZE_(x) (sizeof(x)/sizeof(x[0]))


/* linux only */
#define isolaCLEARTERMINAL_() SDL_Log("\033[2J");;
	



#endif
