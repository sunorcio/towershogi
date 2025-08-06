#ifndef ISOLA_CONFIG_H
#define ISOLA_CONFIG_H




/* isola configuration _____________________________________________________ */

/* alternatively, set -DISOLA_DBG flag */
#define ISOLA_CONFIG_DEBUG    1 /* [1^0] */
/* input stderr to file (./isola.log) */
#define ISOLA_CONFIG_LOG      0 /* [1^0] */


/* init parameters for SDL context creation ________________________________ */

#define ISOLA_WINDOWTITLE    "isola_example"
/* width and height to 0 avoids flickering if already handling window size */
#define ISOLA_WINDOWWIDTH     0
#define ISOLA_WINDOWHEIGHT    0
#define ISOLA_GAMEPAD         0 /* [1^0] */
#define ISOLA_DEPTH           1 /* [1^0] */
#define ISOLA_STENCIL         0 /* [1^0] */
#define ISOLA_ALPHA           1 /* [1^0] */
#define ISOLA_DOUBLEBUFFER    1 /* [1^0] */
#define ISOLA_MSANTIALIASING  0 /* [0^2^4^8^16] (requires doublebuffer) */
#define ISOLA_VSYNC           0 /* [1^0] */
#define ISOLA_MAJOR_VERSION   3 /* gl version ( 4.6 == MAJ 4, MIN 6 ) */
#define ISOLA_MINOR_VERSION   0
/* SDL_GL_CONTEXT_PROFILE_[CORE^COMPATIBILITY] (only since version 3.2) */
#define ISOLA_PROFILE         SDL_GL_CONTEXT_PROFILE_CORE
#define ISOLA_GLSLCHARMAX     10000 /* default glsl buffer size */

/* _________________________________________________________________________ */




#endif
