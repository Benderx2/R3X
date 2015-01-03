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
#ifndef R3X_GRAPHICS_H
#define R3X_GRAPHICS_H
#define _GRAPHICS_SDL
#ifdef _GRAPHICS_SDL
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
typedef struct font {
	GLuint display_list;
	GLuint mat_list;
	int box_w; int box_h;
	int w[256];
	GLuint texture; 
} font_t;
typedef struct Graphics {
	SDL_Surface* Screen;
	char* TextBuf;
	int TextOffset;
	int FontSize;
	float FontScale;
	int CharMaxW;
	int CharMaxH;
	const SDL_VideoInfo* VideoInfo;
	font_t* font;
	FILE* Console;
	int Height;
	int Width;
	int Depth;
	// Used for graphix
	int ScreenX;
	int ScreenY;
	int SDLFlags;
} Graphics_t;
font_t* loadfont(char* s);
bool text3D( font_t * f, const char* txt, ...);
bool text( int x, int y, float scale, font_t * f, char* txt);
bool vm_puts(font_t* font, char* txt, Graphics_t* Graphics);
Graphics_t* InitGraphics(void);
void GLUpdate(void);
void vm_putc(char a, Graphics_t* Graphics);
bool gl_text_update(Graphics_t* Graphics);
bool freefont(font_t * f);
#endif
#endif
