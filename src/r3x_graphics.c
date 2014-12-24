#include <r3x_graphics.h>
#include <r3x_version.h>
#include <nt_malloc.h>
void Update(SDL_Surface*);
Graphics_t* InitGraphics(void)
{
	// F**k SDL, we gotta use stdio output redirection...
	Graphics_t* graphics = nt_malloc(sizeof(Graphics_t));
	graphics->Width = 0;
	graphics->Height = 0;
	graphics->Depth = 0;
	graphics->SDLFlags = 0;
	graphics->Screen = NULL;
	graphics->Console = stdout;
	// Initialise SDL...
	 if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        /* Failed, exit. */
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        exit(1);
        }
	graphics->VideoInfo = SDL_GetVideoInfo();
	if(!graphics->VideoInfo){
		fprintf(stderr, "Can't gather Video Information: %s\n", SDL_GetError());
		exit(1);
	}
	graphics->Width = 640;
	graphics->Height = 480;
	graphics->Depth = graphics->VideoInfo->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	graphics->SDLFlags = SDL_OPENGL;
	if((graphics-> Screen = SDL_SetVideoMode(graphics->Width, graphics->Height, graphics->Depth, graphics->SDLFlags)) == 0){
		 fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		 exit(1);
	}
	SDL_WM_SetCaption(R3X_SYSTEM_VERSION , NULL);
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	graphics->ScreenX = 0;
	graphics->ScreenY = 0;
	graphics->FontSize = 16; // using 16x16 fonts...
	graphics->CharMaxW = graphics->Width / graphics->FontSize;
	graphics->CharMaxH = graphics->Height / graphics->FontSize;
	graphics->FontScale = 2.0f;
	graphics->TextBuf = nt_malloc(graphics->CharMaxH * graphics->CharMaxW);
	graphics->TextOffset = 0;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUpdate();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUpdate();
	SDL_EnableUNICODE(SDL_ENABLE);
	return graphics;
}

void GLUpdate(void)
{
	SDL_GL_SwapBuffers();
}
