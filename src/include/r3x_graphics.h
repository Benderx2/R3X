#ifndef __R3X_GRAPHICS_H
#define __R3X_GRAPHICS_H
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
