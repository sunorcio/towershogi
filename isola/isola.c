#include "isola.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>




#ifdef ISOLA_DBG
signed char isolaErrorSDL(int SDLfunction){

	if(*SDL_GetError() == 0){return 0;}

	if(SDLfunction<0){ SDL_Log("%s",SDL_GetError());}
	SDL_ClearError();
	return -1;
}
signed char isolaErrorGL(void){

	GLenum error = glGetError();
	if (error == GL_NO_ERROR){ return 0; }

	do{ SDL_Log("GLerror : %s", gluErrorString(error));}
	while( (error = glGetError()) != GL_NO_ERROR );
	return -1;
}

 #if(  defined(GLEW_KHR_debug) || defined(GLEW_ARB_debug_output)  )
static void debugCallback(unsigned int source, unsigned int type,
		unsigned int id, unsigned int severity,
		int length, const char* message, const void* userParam){

	SDL_Log("%s",message);
	return;
}
 #endif
#else
signed char isolaErrorSDL(int SDLfunction){return 0;}
signed char isolaErrorGL(void){return 0;}
#endif




SDL_GLContext* isolaContext = {0};
SDL_Window* isolaWindow = {0};    

struct ISOLA_Context isolaInfoContext = {0};
struct ISOLA_Window isolaInfoWindow = {0};
struct ISOLA_Display isolaInfoDisplay = {0};
ISOLA_State isolaInfoState = {0};


static char* isolaShaderSrc = {0};
static FILE* isolaLog = {0};




void isolaGetWindow(void){

	SDL_GetWindowPosition(isolaWindow, &isolaInfoWindow.xpos,
						  &isolaInfoWindow.ypos);
	SDL_GetWindowSize(isolaWindow, &isolaInfoWindow.width,
					  &isolaInfoWindow.height);
	if (isolaInfoWindow.width > isolaInfoWindow.height) {
		isolaInfoWindow.yratio = 1.;
		isolaInfoWindow.xratio = (double)isolaInfoWindow.width/
									isolaInfoWindow.height;
	}else {
		isolaInfoWindow.xratio = 1.;
		isolaInfoWindow.yratio = (double)isolaInfoWindow.height/
									isolaInfoWindow.width;
	}
	isolaInfoWindow.flags = SDL_GetWindowFlags(isolaWindow);
	isolaInfoWindow.displayIndex = SDL_GetWindowDisplayIndex(isolaWindow);
	SDL_GetWindowDisplayMode(isolaWindow, &isolaInfoWindow.displayMode);
	SDL_GetDesktopDisplayMode(isolaInfoWindow.displayIndex,
								&isolaInfoWindow.desktopDisplayMode);
}

void isolaGetDisplay(void){

	int i,j;
	int buff;

	buff = SDL_GetNumVideoDisplays();
	isolaInfoDisplay.displayModeCount = calloc(buff+1, sizeof(int));
	for(i = 0;i<buff;i++){
		isolaInfoDisplay.displayModeCount[i] = SDL_GetNumDisplayModes(
												isolaInfoWindow.displayIndex);
	}

	for(i = 0;isolaInfoDisplay.displayModeCount[i]==0;i++){
		buff += isolaInfoDisplay.displayModeCount[i];
	}
	isolaInfoDisplay.displayModes = malloc(buff*sizeof(SDL_DisplayMode));

	buff = 0;
	for(i = 0;isolaInfoDisplay.displayModeCount[i]==0;i++){
		for(j = 0;j<isolaInfoDisplay.displayModeCount[i];j++){
			SDL_GetDisplayMode(i,j,&isolaInfoDisplay.displayModes[buff+j]);
		}
		buff += isolaInfoDisplay.displayModeCount[i];
	}
}

static void isolaGetContext(void){

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_FRONT_LEFT,
									GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
									&isolaInfoContext.fbdefRedsize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_FRONT_LEFT,
									GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
									&isolaInfoContext.fbdefGreensize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_FRONT_LEFT,
									GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
									&isolaInfoContext.fbdefBluesize);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_FRONT_LEFT,
									GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
									&isolaInfoContext.fbdefAlphasize);
#if ISOLA_DEPTH
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_DEPTH,
									GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
									&isolaInfoContext.fbdefDepthsize);
#endif
#if ISOLA_STENCIL
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_STENCIL,
									GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
									&isolaInfoContext.fbdefStencilsize);
#endif
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
									GL_FRONT_LEFT,
									GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
									&isolaInfoContext.fbdefColorencoding);

	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,
			&isolaInfoContext.maxVertices);
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES,
			&isolaInfoContext.maxIndices);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,
			&isolaInfoContext.maxAttrib);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			&isolaInfoContext.maxVertexUniforms);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			&isolaInfoContext.maxFragmentUniforms);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			&isolaInfoContext.maxTexCombinedUnits);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,
			&isolaInfoContext.maxTexUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE,
			&isolaInfoContext.maxTexSize);
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE,
			&isolaInfoContext.max3DTexSize);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			&isolaInfoContext.maxCubeTexSize);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS,
			&isolaInfoContext.maxDrawBuffers);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,
			&isolaInfoContext.maxColorAttachments);
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,
			&isolaInfoContext.maxRenderbufferSize);

	isolaInfoContext.cpuCount = SDL_GetCPUCount();
	isolaInfoContext.systemRAM = SDL_GetSystemRAM();
	isolaInfoContext.cacheSize = SDL_GetCPUCacheLineSize();
}

void isolaGetState(void){

	int state;

	isolaInfoState = 0x0000;
	glGetIntegerv(GL_BLEND, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_BLEND);
	glGetIntegerv(GL_COLOR_LOGIC_OP, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_COLORLOGIC);
	glGetIntegerv(GL_CULL_FACE, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_CULLFACE);
	glGetIntegerv(GL_DEPTH_TEST, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_DEPTHTEST);
	glGetIntegerv(GL_DITHER, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_DITHER);
	glGetIntegerv(GL_DOUBLEBUFFER, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_DOUBLEBUFFER);
	glGetIntegerv(GL_SCISSOR_TEST, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_SCISSORTEST);
	glGetIntegerv(GL_STENCIL_TEST, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_STENCILTEST);
	glGetIntegerv(GL_FRAMEBUFFER_SRGB, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_SRGBFRAMEBUFFER);
	glGetIntegerv(GL_POINT_SMOOTH, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_POINTSMOOTH);
	glGetIntegerv(GL_LINE_SMOOTH, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_LINESMOOTH);
	glGetIntegerv(GL_POLYGON_SMOOTH, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_POLYGONSMOOTH);
	glGetIntegerv(GL_PROGRAM_POINT_SIZE, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_POINTSIZEPROGRAM);
	glGetIntegerv(GL_MULTISAMPLE, &state);
	isolaInfoState = (isolaInfoState | state*ISOLA_STATE_MULTISAMPLE);
}




unsigned int isolaShaderCompile(const char* shaderFile,
		unsigned int shaderType){

	unsigned int shaderObject;
	FILE* f;
	int l;

	f = fopen(shaderFile, "a+");
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty\n");fclose(f);return 0;}
	if(l>ISOLA_GLSLCHARMAX){SDL_Log("Shader exceeds character limit \
		(defined in isola.h)\n");fclose(f);return 0;}
	memset(isolaShaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(f, 0, SEEK_SET);
	fread(isolaShaderSrc, 1, l, f);
	fclose(f);
		
	shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject,1,(const char**)&isolaShaderSrc,0);
	glCompileShader(shaderObject);

#ifdef ISOLA_DBG
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &l);
	if(!l){
		memset(isolaShaderSrc, 0, ISOLA_GLSLCHARMAX);
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &l);
		glGetShaderInfoLog(shaderObject, l, &l, isolaShaderSrc);
		SDL_Log("Compilation failed  :  %s\n\n",isolaShaderSrc);
		return 0;
	}
#endif

	return shaderObject;
}

unsigned int isolaShaderProgram(const char* vertShaderFile,
		const char* fragShaderFile){

	unsigned int sp;
	unsigned int vs, fs;
	int l;

	sp = glCreateProgram();
	vs = isolaShaderCompile(vertShaderFile,GL_VERTEX_SHADER);
	fs = isolaShaderCompile(fragShaderFile,GL_FRAGMENT_SHADER);
	glAttachShader(sp,vs);
	glAttachShader(sp,fs);
	
	glLinkProgram(sp);

#ifdef ISOLA_DBG
	glGetProgramiv(sp, GL_LINK_STATUS, &l);
	if(!l){
		memset(isolaShaderSrc, 0, ISOLA_GLSLCHARMAX);
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &l);
		glGetShaderInfoLog(sp, l, &l, isolaShaderSrc);
		SDL_Log("Compilation failed  :  %s\n\n",isolaShaderSrc);
		return 0;
	}
#endif

	glDetachShader(sp,vs);
	glDetachShader(sp,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return sp;
}


char* isolaShaderSrcLoad(const char* shaderFile){

	FILE* f;
	int l;
	char* shaderSrc;

	shaderSrc = calloc(ISOLA_GLSLCHARMAX+1, sizeof(char));
	f = fopen(shaderFile, "a+");
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty\n");fclose(f);return 0;}
	if(l>ISOLA_GLSLCHARMAX){SDL_Log("Shader exceeds character limit \
		(defined in isola.h)\n");fclose(f);return 0;}
	memset(shaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(f, 0, SEEK_SET);
	(void)fread(shaderSrc, 1, l, f);
	fclose(f);

	return shaderSrc;
}

unsigned char isolaShaderSrcCompare(char* shaderSrc,
		const char* shaderFile){

	int l;
	FILE* f;

	f = fopen(shaderFile, "a+");
	fseek(f, 0, SEEK_END);
	l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty\n");fclose(f);return 0;}
	if(l>ISOLA_GLSLCHARMAX){SDL_Log("Shader exceeds character limit \
			(defined in isola.h)\n");fclose(f);return 0;}
	memset(isolaShaderSrc, 0, ISOLA_GLSLCHARMAX);
	fseek(f, 0, SEEK_SET);
	fread(isolaShaderSrc, 1, l, f);
	fclose(f);
	
	if(strcmp(shaderSrc, isolaShaderSrc)){
		memset(shaderSrc, 0, ISOLA_GLSLCHARMAX);
		memcpy(shaderSrc, isolaShaderSrc, l);
		return 1;
	}

	return 0;
}




static void contextPromt(void){

	int maj, min, flags, prof;

	SDL_Log("\n");
	SDL_Log("Vendor        : %s", glGetString(GL_VENDOR));
	SDL_Log("Renderer      : %s", glGetString(GL_RENDERER));
	SDL_Log("Version       : %s", glGetString(GL_VERSION));
	SDL_Log("GLSL          : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


	SDL_Log("\n");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
	SDL_Log("Context(SDL)  : %d.%d", maj, min);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log("core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log("compatibility profile");
	}

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
	if(flags & SDL_GL_CONTEXT_DEBUG_FLAG){
		SDL_Log("Debug context");
	}

	SDL_Log("\n");
	glGetIntegerv(GL_MAJOR_VERSION, &maj);
	glGetIntegerv(GL_MINOR_VERSION, &min);
	SDL_Log("Context(GL)   : %d.%d", maj, min);
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &prof);
	if(prof == GL_CONTEXT_CORE_PROFILE_BIT){
		SDL_Log("core profile");
	}else if(prof == GL_CONTEXT_COMPATIBILITY_PROFILE_BIT){
		SDL_Log("compatibility profile");
	}

	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if(flags & GL_CONTEXT_FLAG_DEBUG_BIT){
		SDL_Log("Debug context");
	}

	SDL_Log("\n\n\n");
}

void isolaInit(void){

	unsigned int contextFlags = 0;

	isolaShaderSrc = calloc(ISOLA_GLSLCHARMAX+1, sizeof(char));

#if ISOLA_LOG
	isolaLog = freopen("isola.log","a+",stderr);
#endif
	
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER |
			ISOLA_GAMEPAD*(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) );


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ISOLA_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ISOLA_MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, ISOLA_PROFILE);

#ifdef ISOLA_DBG
	contextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
#ifdef __APPLE__
	contextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, contextFlags);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);

#if(ISOLA_ALPHA)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
#endif

#if(ISOLA_DEPTH)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
#else
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
#endif

#if(ISOLA_STENCIL)
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
#else
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
#endif

#if(!ISOLA_DOUBLEBUFFER)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
#endif

#if(ISOLA_MSANTIALIASING)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, ISOLA_MSANTIALIASING);
#endif

	isolaWindow = SDL_CreateWindow( ISOLA_WINDOWTITLE,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			480, 360, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );

	if (!isolaWindow) {
		isolaErrorSDL(-1);
		SDL_Log("window creation failed, using default sdl hints");
		SDL_Log("\n");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 3);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 2);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_STEREO, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		isolaWindow = SDL_CreateWindow( "isola", SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED, 480, 360,
				SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if (!isolaWindow) {
			SDL_Log("window creation failed (again)");
			SDL_Log("\n");
		}
	}

	isolaContext = SDL_GL_CreateContext(isolaWindow);
	SDL_GL_MakeCurrent(isolaWindow, isolaContext);


	/* glewExperimental = GL_TRUE; */
	glewInit();


#ifdef ISOLA_DBG
	contextPromt();
#endif

#ifdef ISOLA_DBG
	 if( GLEW_KHR_debug || GLEW_ARB_debug_output ){
	 glDebugMessageCallbackARB(&debugCallback, 0);
	 }
#endif


#if(ISOLA_DEPTH)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearDepth(1);
#else
	glDisable(GL_DEPTH_TEST);
#endif

#if(ISOLA_STENCIL)
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xff);
	glStencilFuncSeparate(GL_FRONT_AND_BACK,GL_ALWAYS,0xff,0xff);
	glStencilOpSeparate(GL_FRONT_AND_BACK,GL_KEEP,GL_KEEP,GL_REPLACE);
	glClearStencil(0);
#else
	glDisable(GL_STENCIL_TEST);
#endif

#if(ISOLA_ALPHA)
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
	/* glDepthMask(GL_FALSE); */
#else
	glDisable(GL_BLEND);
#endif

#if(ISOLA_MSANTIALIASING)
	glEnable(GL_MULTISAMPLE);
#else
	glDisable(GL_MULTISAMPLE);
#endif


	glClearColor(0.75f,0.5f,0.75f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(isolaWindow);


#ifdef ISOLA_DBG
	isolaGetContext();
	isolaGetState();
#endif
	isolaGetWindow();
	isolaGetDisplay();
	SDL_GL_SetSwapInterval(ISOLA_VSYNC);
}
void isolaQuit(void){

#ifdef ISOLA_DBG
	if( isolaErrorGL() || isolaErrorSDL(-1) ){
		SDL_Log("Uncaught errors left");
	}
#endif


#if ISOLA_LOG
	fclose(isolaLog);
#endif
	free(isolaShaderSrc);
	free(isolaInfoDisplay.displayModeCount);
	free(isolaInfoDisplay.displayModes);
	SDL_GL_DeleteContext(isolaContext);
	SDL_DestroyWindow(isolaWindow);
	SDL_Quit();
}





