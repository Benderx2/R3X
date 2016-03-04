#ifndef __RXGL_H
#define __RXGL_H
/*
 * This is not a C/C++ header!
 * It is to be used with R3X applications 
 * that are written in T++.
*/
#define GL_POINTS 0
#define GL_LINES 1
#define GL_LINE_STRIP 2
#define GL_LINE_LOOP 3
#define GL_TRIANGLES 4
#define GL_QUADS 5
#define GL_QUAD_STRIP 6
#define GL_POLYGON 7

#define GL_FRONT 0 
#define GL_BACK 1
#define GL_FRONT_AND_BACK 2

#define GL_ALPHA_TEST 0
#define GL_AUTO_NORMAL 1
#define GL_BLEND 2
#define GL_CULL_FACE 3
#define GL_DEPTH_TEST 4
#define GL_DITHER 5
#define GL_FOG 6
#define GL_LIGHTING 7
#define GL_LINE_SMOOTH 8
#define GL_SCISSOR_TEST 9
#define GL_STENCIL_TEST 10

#define GL_COLOR_BUFFER_BIT 0
#define GL_DEPTH_BUFFER_BIT 1
#define GL_ACCUM_BUFFER_BIT 2
#define GL_STENCIL_BUFFER_BIT 3

#define GL_MODELVIEW 0
#define GL_PROJECTION 1 
#define GL_TEXTURE 2

#define GL_TEXTURE_2D 0

#define GL_COLOR_INDEX 0 
#define GL_RED 1
#define GL_GREEN 2
#define GL_BLUE 3
#define GL_ALPHA 4
#define GL_RGB 5
#define GL_BGR 6
#define GL_RGBA 7
#define GL_BGRA 8
#define GL_LUMINANCE 9
#define GL_LUMINANCE_ALPHA 10

#define GL_UNSIGNED_BYTE 0
#define GL_BYTE 1
#define GL_UNSIGNED_SHORT 2
#define GL_SHORT 3
#define GL_UNSIGNED_INT 4
#define GL_INT 5
#define GL_FLOAT 6

// Declare native GL functions
native(r_glBegin, "rxgl.so", 1)
native(r_glClear, "rxgl.so", 1)
native(r_glClearColor, "rxgl.so", 4)
native(r_glColor4f, "rxgl.so", 4)
native(r_glColor3f, "rxgl.so", 3)
native(r_glEnd, "rxgl.so", 0)
native(r_glFrustum, "rxgl.so", 6)
native(r_glLoadIdentity, "rxgl.so", 0)
native(r_glMatrixMode, "rxgl.so", 1)
native(r_glRotated, "rxgl.so", 4)
native(r_glRotatef, "rxgl.so", 4)
native(r_glScalef, "rxgl.so", 3)
native(r_glScaled, "rxgl.so", 3)
native(r_glScissor, "rxgl.so", 4)
native(r_glTexCoord2f, "rxgl.so", 2)
native(r_glTexCoord3f, "rxgl.so", 3)
native(r_glTexCoord4f, "rxgl.so", 4)
native(r_glTexImage2D, "rxgl.so", 9)
native(r_glTexSubImage2D, "rxgl.so", 9)
native(r_glTranslatef, "rxgl.so", 3)
native(r_glTranslated, "rxgl.so", 3)
native(r_glVertex2f, "rxgl.so", 2)
native(r_glVertex3f, "rxgl.so", 3)
native(r_glVertex4f, "rxgl.so", 4)
native(r_glViewport, "rxgl.so", 4)

#endif
