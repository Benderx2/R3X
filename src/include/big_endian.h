#ifndef __BIG_ENDIAN_H
#define __BIG_ENDIAN_H
#ifdef R3X_BIG_ENDIAN
#ifdef __GNUC__
#define BIG_ENDIAN_INT(x) (uint32_t)__builtin_bswap32(x)
#else 
#error "Fuck this compiler, use a GNU compliant one"
#endif
#endif
#endif