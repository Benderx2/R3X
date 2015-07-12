#ifndef __R3X_VIDEO_H
#ifdef REX_GRAPHICS
#include <system.h>
#include <nt_malloc.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <X11/Xlib.h>
void rx_video_init(unsigned int screen_width, unsigned int screen_height);
void rx_write(uint8_t* pixels_to_write, unsigned int s_offset, unsigned int size);
void rx_render(void);
#endif
#endif
