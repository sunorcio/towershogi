#ifndef MISC_H
#define MISC_H




#define isolaLITSTR_(X) #X
#define isolaVARSTR_(X) isolaLITSTR_(X)


#define isolaARRAYSIZE_(x) (sizeof(x)/sizeof(x[0]))


/* linux only */
#define isolaCLEARTERMINAL_() SDL_Log("\033[2J");;




static unsigned long isola_rotl64(unsigned long x,unsigned char n){
	return (x<<n | (x>>(64-n)));
}

static unsigned long isola_rotr64(unsigned long x,unsigned char n){
	return (x>>n | (x<<(64-n)));
}

static unsigned int isola_rotl32(unsigned int x,unsigned char n){
	return (x<<n | (x>>(32-n)));
}

static unsigned int isola_rotr32(unsigned int x,unsigned char n){
	return (x>>n | (x<<(32-n)));
}

static unsigned short isola_rotl16(unsigned short x,unsigned char n){
	return (x<<n | (x>>(16-n)));
}

static unsigned short isola_rotr16(unsigned short x,unsigned char n){
	return (x>>n | (x<<(16-n)));
}

static unsigned char isola_rotl8(unsigned char x,unsigned char n){
	return (x<<n | (x>>(8-n)));
}

static unsigned char isola_rotr8(unsigned char x,unsigned char n){
	return (x>>n | (x<<(8-n)));
}
	



#endif
