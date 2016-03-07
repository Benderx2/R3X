#include <rstdlib.h>
#include "rxgl.h"

/* gl example for r3x */
function main(0) 
	print "clearing screen"
	// gl clear color with white.
	@r_glClearColor(1.0, 1.0, 1.0, 1.0)
	// clear color and depth buffers
	@r_glClear(GL_COLOR_BUFFER_BIT)
	@r_glClear(GL_DEPTH_BUFFER_BIT)
	// draw 4 triangles
	@r_glBegin(GL_TRIANGLES)
	@r_glColor3f(0.8, 0, 0)
	@r_glVertex3f(0, 0, 0)
	@r_glVertex3f(1.0, 0, 0)
	@r_glVertex3f(0, 1.0, 0)
	@r_glEnd()
	
	@r_glBegin(GL_TRIANGLES)
	@r_glColor3f(0.8, 0.8, 0)
	@r_glVertex3f(0, 0, 0)
	@r_glVertex3f(sub_f(0, 1.0), 0, 0)
	@r_glVertex3f(0, sub_f(0, 1.0), 0)
	@r_glEnd()
	
	@r_glBegin(GL_TRIANGLES)
	@r_glColor3f(0, 0, 0.8)
	@r_glVertex3f(0, 0, 0)
	@r_glVertex3f(sub_f(0, 1.0), 0, 0)
	@r_glVertex3f(0, 1.0, 0)
	@r_glEnd()
	
	@r_glBegin(GL_TRIANGLES)
	@r_glColor3f(0, 0.8, 0)
	@r_glVertex3f(0, 0, 0)
	@r_glVertex3f(1.0, 0 , 0)
	@r_glVertex3f(0, sub_f(0, 1.0), 0)
	@r_glEnd()
	// call vm specific function to swap buffers
	asm "syscall SYSCALL_GLUPDATE"
	// jump into debugger
	asm "break"
	print "program execution complete"
	end
endf
