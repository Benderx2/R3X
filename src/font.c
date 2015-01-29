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
#include <r3x_graphics.h>
#include <nt_malloc.h>
#ifdef REX_GRAPHICS
char putstring[2];
font_t* loadfont(char* s)
{
    putstring[0] = 0;
    putstring[1] = 0;
    font_t* f = nt_malloc(sizeof(font_t));
    float glyph_x[256][4];
    SDL_Surface* beforeimg = IMG_Load(s);
    SDL_Surface* img = SDL_DisplayFormat(beforeimg);
    
    uint32_t * pixels = (uint32_t*) img->pixels;
    
    f->box_w = img->w/16.0; f->box_h = img->h/16.0;
    int first_x, last_x; bool empty;
    
    for(int bx = 0; bx<16; bx++)
    {
        for(int by = 0; by<16; by++)
        {
            first_x = last_x = 0;
            empty = true;
            for(int x = 0; x<f->box_w; x++)
            {
                for(int y = 0; y<f->box_h; y++)
                {
                    if(pixels[(y+by*f->box_h)*16*f->box_w+(x+bx*f->box_w)] == 0x00FF00FF || pixels[(y+by*f->box_h)*16*f->box_w+(x+bx*f->box_w)] == 0x0000FF00)
                    {
                        pixels[(y+by*f->box_h)*16*f->box_w+(x+bx*f->box_w)] = 0xFF000000;
                    } 
                    else
                    {
                        if(first_x == 0)
                            first_x = x;
                        if(last_x < x)
                            last_x = x;
                        empty = false;
                    }
                }
            }
            if(empty)
                last_x = f->box_w/8.0f;
            
            glyph_x[by*16+bx][0] = bx*f->box_w+first_x;
            glyph_x[by*16+bx][1] = by*f->box_h;
            glyph_x[by*16+bx][2] = bx*f->box_w+last_x+1;
            glyph_x[by*16+bx][3] = (by+1)*f->box_h;
            f->w[by*16+bx] = last_x - first_x+1;
        }
    }
    
    glGenTextures(1, &(f->texture));
    
    glBindTexture(GL_TEXTURE_2D, f->texture);
    glTexImage2D( GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    f->display_list = glGenLists(256);
    f->mat_list = glGenLists(2);
    
    float iw = img->w, ih = img->h;
    
    for(int i = 0; i<256; i++)
    {
        glNewList(f->display_list+i, GL_COMPILE);
            glBegin(GL_QUADS);
                glTexCoord2f(glyph_x[i][2]/iw, glyph_x[i][1]/ih);                 glVertex2f(f->w[i], 0);
                glTexCoord2f(glyph_x[i][0]/iw, glyph_x[i][1]/ih);                 glVertex2f(0, 0);
                glTexCoord2f(glyph_x[i][0]/iw, glyph_x[i][3]/ih);                 glVertex2f(0, f->box_h);
                glTexCoord2f(glyph_x[i][2]/iw, glyph_x[i][3]/ih);                 glVertex2f(f->w[i], f->box_h);
            glEnd();
            glTranslatef( f->w[i]+1, 0.0, 0.0 );
        glEndList();
    }
    
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    glNewList(f->mat_list, GL_COMPILE);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, viewport[2], viewport[3], 0.0f, 1.0f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEndList();
    
    glNewList(f->mat_list+1, GL_COMPILE);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEndList();
    SDL_FreeSurface(beforeimg);
    SDL_FreeSurface(img);
    return f;
}
bool text3D(font_t * f, const char* txt, ...)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, f->texture);
    
    char buff[256]; va_list args;
    va_start(args, txt);
    vsprintf(buff, txt, args);
    va_end(args);
    
    glPushMatrix();
	glScaled(1.0/ (double)(f->box_w), -1.0 / (double)(f->box_h), 1.0);
    glListBase(f->display_list);
    glCallLists(strlen(buff), GL_UNSIGNED_BYTE, buff);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    glPopAttrib();
    
    return true;
}

bool text( int x, int y, float scale, font_t * f, char* txt)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, f->texture);
    
    glCallList(f->mat_list);
    
    glTranslatef(x, y, -2.0f);
    glScalef(scale, scale, scale);
    glListBase(f->display_list);
    glCallLists(strlen(txt), GL_UNSIGNED_BYTE, txt);
    glCallList(f->mat_list+1);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    
    glPopAttrib();
    
    return true;
}

bool freefont(font_t * f)
{
    nt_free(f);
    return true;
}
void vm_putc(char a, Graphics_t* Graphics)
{
	if(a == '\n'){
		int ScreenY = Graphics->TextOffset / Graphics->CharMaxW;
		ScreenY++;
		Graphics->TextOffset = ScreenY * Graphics->CharMaxW;
	}
	if(Graphics->TextOffset < ((Graphics->Height/Graphics->FontSize)*(Graphics->Width/Graphics->FontSize))){
		if(a != 0 && a != '\n'){
			Graphics->TextBuf[Graphics->TextOffset] = a;
			Graphics->TextOffset++;
		}
	}
	else {
		// Scroll the screen by 1 line
		glClearColor(DefaultRGBA_r, DefaultRGBA_g, DefaultRGBA_b, DefaultRGBA_a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLUpdate(); 
		glClearColor(DefaultRGBA_r, DefaultRGBA_g, DefaultRGBA_b, DefaultRGBA_a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GLUpdate(); 
        for(int i = 0; i < (Graphics->CharMaxH-1)*(Graphics->CharMaxW); i++){
            Graphics->TextBuf[i] = Graphics->TextBuf[i+Graphics->CharMaxW];
        }
		memset(Graphics->TextBuf + (Graphics->CharMaxH - 1)*Graphics->CharMaxW, 0, Graphics->CharMaxW);
		Graphics->TextOffset -=  ((Graphics->Width)/(Graphics->FontSize));
		if(a != 0 && a != '\n'){
			Graphics->TextBuf[Graphics->TextOffset] = a;
			Graphics->TextOffset++;
		}
	}
}
bool vm_puts(font_t* font, char* txt, Graphics_t* Graphics)
{
	GLUpdate();
	for(unsigned int i = 0; i < strlen(txt); i++)
	{
		vm_putc(txt[i], Graphics);
	}
	gl_text_update(Graphics);
	return true;
	
}
bool gl_text_update(Graphics_t* Graphics) {
	int x = 0; int y = 0;
	for(int i = 0; i < Graphics->CharMaxH * Graphics->CharMaxW; i++){
		putstring[0] = Graphics->TextBuf[i];
		putstring[1] = 0;
		if(putstring[0] != 0){
			text(x, y, Graphics->FontScale, Graphics->font, (char*)&putstring);
		}
			x += Graphics->FontSize; 
			if(x >= Graphics->Width){
				y += Graphics->FontSize;
				x = 0;
			}
	}
	GLUpdate();
	return true;
}
#endif
