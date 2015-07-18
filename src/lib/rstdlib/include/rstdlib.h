/*
 * Standard include file for t++ programs
*/

#ifndef __RSTDLIB_H
#define __RSTDLIB_H

/* standard library functions defined in rstdlib.so */
/* note: the vm has a special case for "rstdlib" */

native(r_strlen, "rstdlib", 1)
native(r_strcmp, "rstdlib", 2)
native(r_strcpy, "rstdlib", 2)
native(r_strcat, "rstdlib", 2)
native(r_memset, "rstdlib", 3)
native(r_memcpy, "rstdlib", 3)
native(r_fopen, "rstdlib", 2)
native(r_fclose, "rstdlib", 1)
native(r_fread, "rstdlib", 3)
native(r_fwrite, "rstdlib", 3)
native(r_fflush, "rstdlib", 1)
native(r_getenv, "rstdlib", 2)
native(r_setenv, "rstdlib", 3)
native(r_getapiversion, "rstdlib", 0)
native(r_exitlib, "rstdlib", 0)

/* math functions */

native(r_sin, "rstdlib", 1)
native(r_cos, "rstdlib", 1)
native(r_tan, "rstdlib", 1)
native(r_sec, "rstdlib", 1)
native(r_cosec, "rstdlib", 1)
native(r_cot, "rstdlib", 1)
native(r_sinh, "rstdlib", 1)
native(r_cosh, "rstdlib", 1)
native(r_tanh, "rstdlib", 1)
native(r_sech, "rstdlib", 1)
native(r_cosech, "rstdlib", 1)
native(r_coth, "rstdlib", 1)
native(r_asin, "rstdlib", 1)
native(r_acos, "rstdlib", 1)
native(r_atan, "rstdlib", 1)
native(r_asec, "rstdlib", 1)
native(r_acosec, "rstdlib", 1)
native(r_acot, "rstdlib", 1)
native(r_asinh, "rstdlib", 1)
native(r_acosh, "rstdlib", 1)
native(r_atanh, "rstdlib", 1)
native(r_asech, "rstdlib", 1)
native(r_acosech, "rstdlib", 1)
native(r_acoth, "rstdlib", 1)
native(r_exp, "rstdlib", 1)
native(r_log, "rstdlib", 1)
native(r_log10, "rstdlib", 1)
native(r_pow, "rstdlib", 2)
native(r_fabs, "rstdlib", 1)
native(r_floor, "rstdlib", 1)
native(r_ceil, "rstdlib", 1)
native(r_fmod, "rstdlib", 2)

/* socket functions */
native(r_connect, "rstdlib", 2)
native(r_send, "rstdlib", 2)
native(r_receive, "rstdlib", 3)
native(r_receive_no_wait, "rstdlib", 3)

#endif
