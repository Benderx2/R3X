#include <r3x_video.h>

uint8_t* rx_pixels = NULL;
unsigned int s_width = 0; unsigned int s_height = 0;

void rx_video_init(unsigned int screen_width, unsigned int screen_height) {
	rx_pixels = nt_malloc(screen_width*screen_height*sizeof(uint8_t));
	memset(rx_pixels, 0, screen_width*screen_height*sizeof(uint8_t));
	s_width = screen_width;
	s_height = screen_height;
}
void rx_render(void) {
	glDisable(GL_DEPTH_TEST);
	glDrawPixels(s_width, s_height, GL_RGB, GL_UNSIGNED_BYTE, rx_pixels);
	glEnable(GL_DEPTH_TEST);
	//SDL_GL_SwapBuffers();
}
void rx_write(uint8_t* pixels_to_write, unsigned int s_offset, unsigned int size) {
	memcpy((void*)((uintptr_t)rx_pixels + s_offset), pixels_to_write, size);
}
