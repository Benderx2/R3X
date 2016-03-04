#define REX_GRAPHICS
#define GL_TRUE 0
#define GL_FALSE 1
#define SO_NAME "rxgl.so"

#include <virtual-machine.h>
#include <nt_malloc.h>

/* gl implementation for r3x */

GLenum gbn_list[] = {
	GL_POINTS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_TRIANGLES,
	GL_QUADS,
	GL_QUAD_STRIP,
	GL_POLYGON
};
GLenum gcls_list[] = {
	GL_COLOR_BUFFER_BIT, 
	GL_DEPTH_BUFFER_BIT, 
	GL_ACCUM_BUFFER_BIT, 
	GL_STENCIL_BUFFER_BIT
};

GLenum gcf_list[] = {
	GL_FRONT, 
	GL_BACK, 
	GL_FRONT_AND_BACK
};

GLenum genbl_list[] = {
	GL_ALPHA_TEST,
	GL_AUTO_NORMAL,
	GL_BLEND,
	GL_CULL_FACE,
	GL_DEPTH_TEST,
	GL_DITHER,
	GL_FOG,
	GL_LIGHTING,
	GL_LINE_SMOOTH,
	GL_SCISSOR_TEST,
	GL_STENCIL_TEST
};
GLenum gmtxm_list[] = {
	 GL_MODELVIEW, 
	 GL_PROJECTION, 
	 GL_TEXTURE
};

GLenum gtx2d_fmtlist[] = {
	GL_COLOR_INDEX, 
	GL_RED, 
	GL_GREEN, 
	GL_BLUE, 
	GL_ALPHA, 
	GL_RGB, 
	GL_BGR, 
	GL_RGBA, 
	GL_BGRA, 
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA
};
GLenum gtx2d_typelist[] = {
	GL_UNSIGNED_BYTE, 
	GL_BYTE,  
	GL_UNSIGNED_SHORT, 
	GL_SHORT, 
	GL_UNSIGNED_INT,
	GL_INT, 
	GL_FLOAT
};
r3x_cpu_t* CPU = NULL;
void Start(r3x_cpu_t* _CPU) {
	CPU = _CPU;
}
uint32_t r_glBegin(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 8) {
		glBegin(gbn_list[arg1]);
	}
	return (uint32_t)-1;
}
uint32_t r_glClear(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 4) {
		glClear(gcls_list[arg1]);
	}
	return (uint32_t)-1;
}
uint32_t r_glClearColor(void) {
	uint32_t r = GetArgument(CPU,1,4);
	uint32_t g = GetArgument(CPU,2,4);
	uint32_t b = GetArgument(CPU,3,4);
	uint32_t a = GetArgument(CPU,4,4);
	glClearColor(*(float*)&r, *(float*)&g, *(float*)&b, *(float*)&a);
	return 0;
}
uint32_t r_glColor4f(void) {
	uint32_t r = GetArgument(CPU,1,4);
	uint32_t g = GetArgument(CPU,2,4);
	uint32_t b = GetArgument(CPU,3,4);
	uint32_t a = GetArgument(CPU,4,4);
	glColor4f(*(float*)&r, *(float*)&g, *(float*)&b, *(float*)&a);
	return 0;
}
uint32_t r_glColor3f(void) {
	uint32_t r = GetArgument(CPU,1,3);
	uint32_t g = GetArgument(CPU,2,3);
	uint32_t b = GetArgument(CPU,3,3);
	glColor3f(*(float*)&r, *(float*)&g, *(float*)&b);
	return 0;
}
uint32_t r_glCullFace(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 3) {
		glCullFace(gcf_list[arg1]);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glEnable(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 11) {
		glEnable(genbl_list[arg1]);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glDisable(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 11) {
		glDisable(genbl_list[arg1]);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glEnd(void) {
	glEnd();
	return 0;
}
uint32_t r_glFrustum(void) {
	uint32_t l = GetArgument(CPU,1,6);
	uint32_t r = GetArgument(CPU,2,6);
	uint32_t b = GetArgument(CPU,3,6);
	uint32_t t = GetArgument(CPU,4,6);
	uint32_t n = GetArgument(CPU,5,6);
	uint32_t f = GetArgument(CPU,6,6);
	glFrustum(*(float*)&l, *(float*)&r, *(float*)&b, *(float*)&t, *(float*)&n, *(float*)&f);
	return 0;
}
uint32_t r_glLoadIdentity(void) {
	glLoadIdentity();
	return 0;
}
uint32_t r_glMatrixMode(void) {
	uint32_t arg1 = GetArgument(CPU,1,1);
	if(arg1 < 3) {
		glMatrixMode(gmtxm_list[arg1]);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glRotatef(void) {
	uint32_t a = GetArgument(CPU,1,4);
	uint32_t x = GetArgument(CPU,2,4);
	uint32_t y = GetArgument(CPU,3,4);
	uint32_t z = GetArgument(CPU,4,4);
	glRotated(*(float*)&a,*(float*)&x,*(float*)&y,*(float*)&z);
	return 0;
}
uint32_t r_glRotated(void) {
	return r_glRotatef();
}
uint32_t r_glScalef(void) {
	uint32_t x = GetArgument(CPU,1,3);
	uint32_t y = GetArgument(CPU,2,3);
	uint32_t z = GetArgument(CPU,3,3);
	glScalef(x,y,z);
	return 0;
}
uint32_t r_glScaled(void) {
	return r_glScalef();
}
uint32_t r_glScissor(void) {
	uint32_t x = GetArgument(CPU,1,4);
	uint32_t y = GetArgument(CPU,2,4);
	uint32_t w = GetArgument(CPU,3,4);
	uint32_t h = GetArgument(CPU,4,4);
	glScissor(*(float*)&x,*(float*)&y,*(float*)&w,*(float*)&h);
	return 0;
}
uint32_t r_glTexCoord2f(void) {
	uint32_t s = GetArgument(CPU, 1,2);
	uint32_t t = GetArgument(CPU, 2,2);
	glTexCoord2f(*(float*)&s, *(float*)&t);
	return 0;
}
uint32_t r_glTexCoord3f(void) {
	uint32_t s = GetArgument(CPU, 1,3);
	uint32_t t = GetArgument(CPU, 2,3);
	uint32_t u = GetArgument(CPU, 3,3);
	glTexCoord3f(*(float*)&s, *(float*)&t, *(float*)&u);
	return 0;
}
uint32_t r_glTexCoord4f(void) {
	uint32_t s = GetArgument(CPU, 1,4);
	uint32_t t = GetArgument(CPU, 2,4);
	uint32_t u = GetArgument(CPU, 3,4);
	uint32_t v = GetArgument(CPU, 4,4);
	glTexCoord4f(*(float*)&s, *(float*)&t, *(float*)&u, *(float*)&v);
	return 0;
}
uint32_t r_glTexImage2D(void) {
	uint32_t t = GetArgument(CPU, 1,9);
	uint32_t l = GetArgument(CPU, 2,9);
	uint32_t i = GetArgument(CPU, 3,9);
	uint32_t w = GetArgument(CPU, 4,9);
	uint32_t h = GetArgument(CPU, 5,9);
	uint32_t b = GetArgument(CPU, 6,9);
	uint32_t f = GetArgument(CPU, 7,9);
	uint32_t tp = GetArgument(CPU,8,9);
	void* d = GetLinearAddress(CPU, GetArgument(CPU,9,9));
	if(t == 0 && i < 11 && f < 11 && tp < 7) {
		glTexImage2D(GL_TEXTURE_2D, l, gtx2d_fmtlist[i], w, h, b, gtx2d_fmtlist[f], gtx2d_typelist[tp], d);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glTexSubImage2D(void) {
	uint32_t t = GetArgument(CPU, 1,9);
	uint32_t l = GetArgument(CPU, 2,9);
	uint32_t x = GetArgument(CPU, 3,9);
	uint32_t y = GetArgument(CPU, 4,9);
	uint32_t w = GetArgument(CPU, 5,9);
	uint32_t h = GetArgument(CPU, 6,9);
	uint32_t f = GetArgument(CPU, 7,9);
	uint32_t tp = GetArgument(CPU,8,9);
	void* d = GetLinearAddress(CPU, GetArgument(CPU,9,9));
	if(t == 0 && f < 11 && tp < 7) {
		glTexSubImage2D(GL_TEXTURE_2D, l, x, y, w, h, gtx2d_fmtlist[f], gtx2d_typelist[tp], d);
		return 0;
	}
	return (uint32_t)-1;
}
uint32_t r_glTranslatef(void) {
	uint32_t x = GetArgument(CPU,1,3);
	uint32_t y = GetArgument(CPU,2,3);
	uint32_t z = GetArgument(CPU,3,3);
	glTranslatef(*(float*)&x,*(float*)&y,*(float*)&z);
	return 0;
}
uint32_t r_glTranslated(void) {
	return r_glTranslatef();
}
uint32_t r_glVertex2f(void) {
	uint32_t x = GetArgument(CPU,1,2);
	uint32_t y = GetArgument(CPU,2,2);
	glVertex2f(*(float*)&x,*(float*)&y);
	return 0;
}
uint32_t r_glVertex3f(void) {
	uint32_t x = GetArgument(CPU,1,3);
	uint32_t y = GetArgument(CPU,2,3);
	uint32_t z = GetArgument(CPU,3,3);
	//printf("x: %f, y: %f, z: %f\n", *(float*)&x, *(float*)&y, *(float*)&z);
	glVertex3f(*(float*)&x,*(float*)&y,*(float*)&z);
	return 0;
}
uint32_t r_glVertex4f(void) {
	uint32_t x = GetArgument(CPU,1,4);
	uint32_t y = GetArgument(CPU,2,4);
	uint32_t z = GetArgument(CPU,3,4);
	uint32_t w = GetArgument(CPU,4,4);
	glVertex4f(*(float*)&x,*(float*)&y,*(float*)&z,*(float*)&w);
	return 0;
}
uint32_t r_glViewport(void) {
	uint32_t x = GetArgument(CPU,1,4);
	uint32_t y = GetArgument(CPU,2,4);
	uint32_t w = GetArgument(CPU,3,4);
	uint32_t h = GetArgument(CPU,4,4);
	glViewport(x,y,w,h);
	return 0;
}
