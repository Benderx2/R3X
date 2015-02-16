/*
Copyright (c) 2015 Benderx2, 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of R3X nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifdef REX_GRAPHICS
#include <r3x_cpu.h>
#include <r3x_graphics.h>
#include <r3x_version.h>
#include <nt_malloc.h>
#define MAX_SCREEN_WIDTH 1024
#define MAX_SCREEN_HEIGHT 768
#define MIN_SCREEN_WIDTH 320
#define MIN_SCREEN_HEIGHT 200
extern r3x_cpu_t* r3_cpu;
extern bool UseServer;
extern unsigned int ScreenWidth;
extern unsigned int ScreenHeight;
/*double DefaultRGBA_r = 0.656875f;
double DefaultRGBA_g = 0.1f;
double DefaultRGBA_b = 0.40625f;
double DefaultRGBA_a = 1.0f;*/
double DefaultRGBA_g = 0.0f;
double DefaultRGBA_r = 0.0f;
double DefaultRGBA_a = 1.0f;
double DefaultRGBA_b = 0.0f;
void Update(SDL_Surface*);
Graphics_t* InitGraphics(void)
{
	XInitThreads();
	if(ScreenWidth > MAX_SCREEN_WIDTH || ScreenHeight > MAX_SCREEN_HEIGHT || ScreenHeight < MIN_SCREEN_HEIGHT || ScreenWidth < MIN_SCREEN_WIDTH) {
		printf("Error: Screen size is either too high or too low. Max: 1024*768, Min: 320*200\n");
		exit(EXIT_FAILURE);
	}
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
	graphics->Width = ScreenWidth; 
	graphics->Height = ScreenHeight; 
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
	if(UseServer == false){
		SDL_WM_SetCaption(R3X_SYSTEM_VERSION , NULL);
	} else {
		char buffer[33];
		sprintf(buffer, "%d", getpid());
		SDL_WM_SetCaption(nt_concat("Running as client, process ID: ", buffer), NULL);
	}
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	graphics->ScreenX = 0;
	graphics->ScreenY = 0;
	graphics->FontWidth = FontWidth; // using 16x16 fonts...
	graphics->FontHeight = FontHeight;
	graphics->CharMaxW = graphics->Width / graphics->FontWidth;
	graphics->CharMaxH = graphics->Height / graphics->FontHeight;
	graphics->FontScale = FontScale;//!0.50f;
	graphics->TextBuf = nt_malloc(graphics->Width*graphics->Height*sizeof(uint8_t));
	graphics->TextOffset = 0;
	glClearColor(DefaultRGBA_r, DefaultRGBA_g, DefaultRGBA_b, DefaultRGBA_a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUpdate();
	glClearColor(DefaultRGBA_r, DefaultRGBA_g, DefaultRGBA_b, DefaultRGBA_a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUpdate();
	SDL_EnableUNICODE(SDL_ENABLE);
	return graphics;
}
void GL_ClearScreen(void){
	glClearColor(DefaultRGBA_r, DefaultRGBA_g, DefaultRGBA_b, DefaultRGBA_a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GLUpdate(void)
{
	SDL_GL_SwapBuffers();
}
#endif
