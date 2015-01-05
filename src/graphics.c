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
#include <X11/Xlib.h>
#include <unistd.h>
#include <r3x_graphics.h>
#include <r3x_version.h>
#include <nt_malloc.h>
extern bool UseServer;
extern int ScreenWidth;
extern int ScreenHeight;
void Update(SDL_Surface*);
Graphics_t* InitGraphics(void)
{
	XInitThreads();
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
	graphics->FontSize = 16; // using 16x16 fonts...
	graphics->CharMaxW = graphics->Width / graphics->FontSize;
	graphics->CharMaxH = graphics->Height / graphics->FontSize;
	graphics->FontScale = 1.0f;
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
